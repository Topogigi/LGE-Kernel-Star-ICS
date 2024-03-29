/*
 * Gadget Driver for Android
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* #define DEBUG */
/* #define VERBOSE_DEBUG */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/utsname.h>
#include <linux/platform_device.h>

#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>
#include <linux/usb/gadget.h>

#include "gadget_chips.h"

/*
 * Kbuild is not very cooperative with respect to linking separately
 * compiled library objects into one module.  So for now we won't use
 * separate compilation ... ensuring init/exit sections work to shrink
 * the runtime footprint, and giving us at least some parts of what
 * a "gcc --combine ... part1.c part2.c part3.c ... " build would.
 */
#define CONFIG_LGE_USB_GADGET_DRIVER
#define CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN	//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> autorun
//#define CONFIG_USB_SUPPORT_LGE_ANDROID_CHARGEMODE //LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Charge only mode	

#include "usbstring.c"
#include "config.c"
#include "epautoconf.c"
#include "composite.c"

#include "f_mass_storage.c"
/*
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN	//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> autorun
#include "u_lgeusb.c"
#include "f_cdrom_storage.c"
#endif
*/
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN
#include "u_lgeusb.h"
#include "f_cdrom_storage.c"
#endif



#include "u_serial.c"
#include "f_acm.c"
#include "f_serial.c"
//LGE_CHANGE_S seokjae.yoon@lge.com 2012/02/13: ==> for NMEA
#if defined(CONFIG_MACH_BSSQ)
#include "f_serial_gps.c"
#endif
//LGE_CHANGE_E seokjae.yoon@lge.com 2012/02/13: ==> for NMEA
#include "f_adb.c"
#include "f_mtp.c"
#include "f_accessory.c"

#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_CHARGEMODE	//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Charge only mode test
#include "f_charge_only.c"
#endif

#ifdef CONFIG_LGE_USB_GADGET_DRIVER
#include "f_ecm.c"
#include "u_ether.c"
#else
#define USB_ETH_RNDIS y
#include "f_rndis.c"
#include "rndis.c"
#include "u_ether.c"
#endif

MODULE_AUTHOR("Mike Lockwood");
MODULE_DESCRIPTION("Android Composite USB Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

static const char longname[] = "Gadget Android";

#if defined(CONFIG_USB_ANDROID_NDIS) || defined(CONFIG_USB_ANDROID_RNDIS)
static u8 ndis_hostaddr[ETH_ALEN];
#endif

/* Default vendor and product IDs, overridden by userspace */

#ifdef CONFIG_LGE_USB_GADGET_DRIVER
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB dec define start
//LGE_CHANGE_S seokjae.yoon@lge.com 2012/02/13: ==>  Change PID for KS1103
#if defined(CONFIG_MACH_BSSQ)
#define VENDOR_ID		0x1004
#define PRODUCT_ID		0x61A3
#define BCD_DEVICE		0x0229
//LGE_CHANGE_E seokjae.yoon@lge.com 2012/02/13: ==>  Change PID for KS1103

#else

#define VENDOR_ID		0x1004
#define PRODUCT_ID		0x61FC // 0x61F1
#define BCD_DEVICE		0x0229
#endif

#define ACM_ENABLED	0x01
#define SERIAL_ENABLED	0x02
#define NMEA_ENABLED	0x04

#define FSG_LUN			1 // 2
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB dec define end
#endif

struct android_usb_function {
	char *name;
	void *config;

	struct device *dev;
	char *dev_name;
	struct device_attribute **attributes;

	/* for android_dev.enabled_functions */
	struct list_head enabled_list;

	/* Optional: initialization during gadget bind */
	int (*init)(struct android_usb_function *, struct usb_composite_dev *);
	/* Optional: cleanup during gadget unbind */
	void (*cleanup)(struct android_usb_function *);

	int (*bind_config)(struct android_usb_function *, struct usb_configuration *);

	/* Optional: called when the configuration is removed */
	void (*unbind_config)(struct android_usb_function *, struct usb_configuration *);
	/* Optional: handle ctrl requests before the device is configured */
	int (*ctrlrequest)(struct android_usb_function *,
					struct usb_composite_dev *,
					const struct usb_ctrlrequest *);
};

struct android_dev {
	struct android_usb_function **functions;
	struct list_head enabled_functions;
	struct usb_composite_dev *cdev;
	struct device *dev;

	bool enabled;
	bool connected;
	bool sw_connected;
	struct work_struct work;

#ifdef CONFIG_LGE_USB_GADGET_DRIVER
        int serial_flags; //LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> Serial flages acm&serial
#endif
};

static struct class *android_class;
static struct android_dev *_android_dev;
static int android_bind_config(struct usb_configuration *c);
static void android_unbind_config(struct usb_configuration *c);

/* string IDs are assigned dynamically */
#define STRING_MANUFACTURER_IDX		0
#define STRING_PRODUCT_IDX		1
#define STRING_SERIAL_IDX		2

static char manufacturer_string[256];
static char product_string[256];
static char serial_string[256];

/* String Table */
static struct usb_string strings_dev[] = {
	[STRING_MANUFACTURER_IDX].s = manufacturer_string,
	[STRING_PRODUCT_IDX].s = product_string,
	[STRING_SERIAL_IDX].s = serial_string,
	{  }			/* end of list */
};

static struct usb_gadget_strings stringtab_dev = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings_dev,
};

static struct usb_gadget_strings *dev_strings[] = {
	&stringtab_dev,
	NULL,
};

//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB dec define start
static struct usb_device_descriptor device_desc = {
	.bLength              = sizeof(device_desc),
	.bDescriptorType      = USB_DT_DEVICE,
	.bcdUSB               = __constant_cpu_to_le16(0x0200),
	.bDeviceClass         = USB_CLASS_MISC, // LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==>  USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass      = 0x02,
	.bDeviceProtocol      = 0x01,
	.idVendor             = __constant_cpu_to_le16(VENDOR_ID),
	.idProduct            = __constant_cpu_to_le16(PRODUCT_ID),
	.bcdDevice            = __constant_cpu_to_le16(BCD_DEVICE),
	.bNumConfigurations   = 1,
};

static struct usb_configuration android_config_driver = {
	.label		= "android",
	.unbind		= android_unbind_config,
	.bConfigurationValue = 1,
	.bmAttributes	= USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_WAKEUP,
	.bMaxPower	= 0xFA, /* 500ma */
};
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB dec define end

static void android_work(struct work_struct *data)
{
	struct android_dev *dev = container_of(data, struct android_dev, work);
	struct usb_composite_dev *cdev = dev->cdev;
	char *disconnected[2] = { "USB_STATE=DISCONNECTED", NULL };
	char *connected[2]    = { "USB_STATE=CONNECTED", NULL };
	char *configured[2]   = { "USB_STATE=CONFIGURED", NULL };
	char **uevent_envp = NULL;
	unsigned long flags;

	spin_lock_irqsave(&cdev->lock, flags);
        if (cdev->config)
		uevent_envp = configured;
	else if (dev->connected != dev->sw_connected)
		uevent_envp = dev->connected ? connected : disconnected;
	dev->sw_connected = dev->connected;
	spin_unlock_irqrestore(&cdev->lock, flags);

	if (uevent_envp) {
		kobject_uevent_env(&dev->dev->kobj, KOBJ_CHANGE, uevent_envp);
		pr_info("%s: sent uevent %s\n", __func__, uevent_envp[0]);
	} else {
		pr_info("%s: did not send uevent (%d %d %p)\n", __func__,
			 dev->connected, dev->sw_connected, cdev->config);
	}
}


/*-------------------------------------------------------------------------*/
/* Supported functions initialization */

static int adb_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
	return adb_setup();
}

static void adb_function_cleanup(struct android_usb_function *f)
{
	adb_cleanup();
}

static int adb_function_bind_config(struct android_usb_function *f, struct usb_configuration *c)
{
	return adb_bind_config(c);
}

static struct android_usb_function adb_function = {
	.name		= "adb",
	.init		= adb_function_init,
	.cleanup	= adb_function_cleanup,
	.bind_config	= adb_function_bind_config,
};

#ifdef CONFIG_LGE_USB_GADGET_DRIVER
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB Acm porting start
/* Serial ports*/
#define MAX_LG_SERIAL_INSTANCES  2
/* Restrict Max acm instances to 1 for consistent LG serial port behavior */
#define MAX_ACM_INSTANCES 1 

            
struct acm_function_config {
	int instances;
};

static int acm_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
	int ret = 0;
	struct android_dev *dev = _android_dev;
            
	f->config = kzalloc(sizeof(struct acm_function_config), GFP_KERNEL);

	if (!f->config)
		return -ENOMEM;
	if(!dev->serial_flags)
	{
		ret = gserial_setup(cdev->gadget, MAX_ACM_INSTANCES+MAX_LG_SERIAL_INSTANCES);
		printk("[msg usb]  android .c >> %s : gserial_setup\n" , __func__ );
	}

	if(ret == 0) 
		dev->serial_flags |= ACM_ENABLED;
		
        /* default ACM instances to 1 for LG configuration*/
        ((struct acm_function_config *)f->config)->instances = MAX_ACM_INSTANCES;

	return ret;
}

static void acm_function_cleanup(struct android_usb_function *f)
{
        struct android_dev *dev = _android_dev;
        dev->serial_flags &= ~ACM_ENABLED;
        if(!dev->serial_flags) 
		gserial_cleanup();

	kfree(f->config);
	f->config = NULL;
}

static int acm_function_bind_config(struct android_usb_function *f, struct usb_configuration *c)
{
	int i;
	int ret = 0;
	struct acm_function_config *config = f->config;
	
	for (i = 0; i < config->instances; i++) {
		ret = acm_bind_config(c, i);
		if (ret) {
			pr_err("Could not bind acm%u config\n", i);
			break;
		}
	}

	return ret;
}

static ssize_t acm_instances_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct acm_function_config *config = f->config;
	return sprintf(buf, "%d\n", config->instances);
}

static ssize_t acm_instances_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct acm_function_config *config = f->config;
	int value;

	sscanf(buf, "%d", &value);
	if (value > MAX_ACM_INSTANCES)
		value = MAX_ACM_INSTANCES;
	config->instances = value;
	return size;
}

static DEVICE_ATTR(instances, S_IRUGO | S_IWUSR, acm_instances_show, acm_instances_store);
static struct device_attribute *acm_function_attributes[] = { &dev_attr_instances, NULL };

static struct android_usb_function acm_function = {
	.name		= "acm",
	.init		= acm_function_init,
	.cleanup	= acm_function_cleanup,
	.bind_config	= acm_function_bind_config,
	.attributes	= acm_function_attributes,
};
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB Acm porting end

//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB Serial poring start
static int serial_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
    int ret = 0;
    struct android_dev *dev = _android_dev;

    if(!dev->serial_flags)
	        ret = gserial_setup(cdev->gadget, MAX_ACM_INSTANCES+MAX_LG_SERIAL_INSTANCES);

	if(ret == 0) 
       	dev->serial_flags |= SERIAL_ENABLED;
	
    printk("[msg usb]  android .c >> %s\n" , __func__ );
    return ret;
}
static int serial_function_bind_config(struct android_usb_function *f,struct usb_configuration *c)
{
	return gser_bind_config(c, MAX_LG_SERIAL_INSTANCES-1);
}

static void serial_function_cleanup(struct android_usb_function *f)
{
        struct android_dev *dev = _android_dev;

        dev->serial_flags &= ~SERIAL_ENABLED;
        if(!dev->serial_flags)
        gserial_cleanup();
}

static struct android_usb_function serial_function = {
        .name = "serial",
        .init = serial_function_init,
        .cleanup = serial_function_cleanup,
        .bind_config = serial_function_bind_config,
};
//LGE_CHANGE_S  youngjae.ko@lge.com 2011/12/23:  ==> USB Serial poring end

#if defined(CONFIG_MACH_BSSQ)
//LGE_CHANGE_S  seokjae.yoon@lge.com 2012/02/13:  ==> USB Serial_gps poring start
static int serial_gps_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
    int ret = 0;
    struct android_dev *dev = _android_dev;

    if(!dev->serial_flags)
	        ret = gserial_setup(cdev->gadget, MAX_ACM_INSTANCES+MAX_LG_SERIAL_INSTANCES);

	if(ret == 0) 
       	dev->serial_flags |= NMEA_ENABLED;
	
    printk("[msg usb]  android .c >> %s\n" , __func__ );
    return ret;
}

static int serial_gps_function_bind_config(struct android_usb_function *f,struct usb_configuration *c)
{
	return lge_android_serial_gps_bind_config(c, MAX_LG_SERIAL_INSTANCES);
}

static void serial_gps_function_cleanup(struct android_usb_function *f)
{
        struct android_dev *dev = _android_dev;

        dev->serial_flags &= ~SERIAL_ENABLED;
        if(!dev->serial_flags)
        gserial_cleanup();
}

static struct android_usb_function serial_gps_function = {
        .name = "serial_gps",
        .init = serial_gps_function_init,
        .cleanup = serial_gps_function_cleanup,
	.bind_config = serial_gps_function_bind_config,
};
//LGE_CHANGE_E  seokjae.yoon@lge.com 2012/02/13:  ==> USB Serial_gps poring start
#endif
#endif

#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_CHARGEMODE	 //LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Charge only mode test	
//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> USB Charge only poring start
static int charge_only_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{	
    	return 0;
}
static int charge_only_function_bind_config(struct android_usb_function *f,struct usb_configuration *c)
{
	return charge_only_bind_config(c);
}

static void charge_only_function_cleanup(struct android_usb_function *f)
{

}

static struct android_usb_function charge_only_function = {
        .name = "charge_only",
        .init = charge_only_function_init,
        .cleanup = charge_only_function_cleanup,
        .bind_config = charge_only_function_bind_config,
};
//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> USB Charge only poring end
#endif

static int mtp_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
	return mtp_setup();
}

static void mtp_function_cleanup(struct android_usb_function *f)
{
	mtp_cleanup();
}

static int mtp_function_bind_config(struct android_usb_function *f, struct usb_configuration *c)
{
	return mtp_bind_config(c, false);
}

static int ptp_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
	/* nothing to do - initialization is handled by mtp_function_init */
	return 0;
}

static void ptp_function_cleanup(struct android_usb_function *f)
{
	/* nothing to do - cleanup is handled by mtp_function_cleanup */
}

static int ptp_function_bind_config(struct android_usb_function *f, struct usb_configuration *c)
{
	return mtp_bind_config(c, true);
}

static int mtp_function_ctrlrequest(struct android_usb_function *f,
						struct usb_composite_dev *cdev,
						const struct usb_ctrlrequest *c)
{
	return mtp_ctrlrequest(cdev, c);
}

static struct android_usb_function mtp_function = {
	.name		= "mtp",
	.init		= mtp_function_init,
	.cleanup	= mtp_function_cleanup,
	.bind_config	= mtp_function_bind_config,
	.ctrlrequest	= mtp_function_ctrlrequest,
};

/* PTP function is same as MTP with slightly different interface descriptor */
static struct android_usb_function ptp_function = {
	.name		= "ptp",
	.init		= ptp_function_init,
	.cleanup	= ptp_function_cleanup,
	.bind_config	= ptp_function_bind_config,
};

#ifdef CONFIG_LGE_USB_GADGET_DRIVER
static struct ecm_function_config {
	u8      ethaddr[ETH_ALEN];
	u32     vendorID;
	char	manufacturer[256];
	bool	wceis;
};

static int ecm_function_init(struct android_usb_function *f, struct usb_composite_dev *cdev)
{
	int ret;
	struct ecm_function_config *ecm;

	f->config = kzalloc(sizeof(struct ecm_function_config), GFP_KERNEL);
	if (!f->config)
		return -ENOMEM;

	ecm = f->config;
	ret = gether_setup_name(cdev->gadget, ecm->ethaddr, "usb");
	if (ret) {
		pr_err("%s: gether_setup failed\n", __func__);
		return ret;
	}
  
	return 0;
}

static void ecm_function_cleanup(struct android_usb_function *f)
{
	gether_cleanup();
	kfree(f->config);
	f->config = NULL;
}

static int ecm_function_bind_config(struct android_usb_function *f,
					struct usb_configuration *c)
{
	struct ecm_function_config *ecm = f->config;

	if (!ecm) {
		pr_err("%s: ecm_pdata\n", __func__);
		return -1;
	}

	pr_info("%s MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
		ecm->ethaddr[0], ecm->ethaddr[1], ecm->ethaddr[2],
		ecm->ethaddr[3], ecm->ethaddr[4], ecm->ethaddr[5]);

	return ecm_bind_config(c, ecm->ethaddr);
}

static void ecm_function_unbind_config(struct android_usb_function *f,
						struct usb_configuration *c)
{
}

static ssize_t ecm_manufacturer_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *config = f->config;
	return sprintf(buf, "%s\n", config->manufacturer);
}

static ssize_t ecm_manufacturer_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *config = f->config;

	if (size >= sizeof(config->manufacturer))
		return -EINVAL;
	if (sscanf(buf, "%s", config->manufacturer) == 1)
		return size;
	return -1;
}

static DEVICE_ATTR(manufacturer, S_IRUGO | S_IWUSR, ecm_manufacturer_show,
						    ecm_manufacturer_store);

static ssize_t ecm_ethaddr_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *ecm = f->config;
	return sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",
		ecm->ethaddr[0], ecm->ethaddr[1], ecm->ethaddr[2],
		ecm->ethaddr[3], ecm->ethaddr[4], ecm->ethaddr[5]);
}

static ssize_t ecm_ethaddr_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *ecm = f->config;

	if (sscanf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",
		    (int *)&ecm->ethaddr[0], (int *)&ecm->ethaddr[1],
		    (int *)&ecm->ethaddr[2], (int *)&ecm->ethaddr[3],
		    (int *)&ecm->ethaddr[4], (int *)&ecm->ethaddr[5]) == 6)
		return size;
	return -EINVAL;
}

static DEVICE_ATTR(ethaddr, S_IRUGO | S_IWUSR, ecm_ethaddr_show,
					       ecm_ethaddr_store);

static ssize_t ecm_vendorID_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *config = f->config;
	return sprintf(buf, "%04x\n", config->vendorID);
}

static ssize_t ecm_vendorID_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct ecm_function_config *config = f->config;
	int value;

	if (sscanf(buf, "%04x", &value) == 1) {
		config->vendorID = value;
		return size;
	}
	return -EINVAL;
}

static DEVICE_ATTR(vendorID, S_IRUGO | S_IWUSR, ecm_vendorID_show,
						ecm_vendorID_store);

static struct device_attribute *ecm_function_attributes[] = {
	&dev_attr_manufacturer,
	&dev_attr_ethaddr,
	&dev_attr_vendorID,
	NULL
};

static struct android_usb_function ecm_function = {
	.name		= "ecm",
	.init		= ecm_function_init,
	.cleanup	= ecm_function_cleanup,
	.bind_config	= ecm_function_bind_config,
	.unbind_config	= ecm_function_unbind_config,
	.attributes	= ecm_function_attributes,
};

#endif

struct mass_storage_function_config {
	struct fsg_config fsg;
	struct fsg_common *common;
};

static int mass_storage_function_init(struct android_usb_function *f,
					struct usb_composite_dev *cdev)
{
	struct mass_storage_function_config *config;
	struct fsg_common *common;
	int err;
	int i = 0;
	char SetLunName[5];
	
	config = kzalloc(sizeof(struct mass_storage_function_config),
								GFP_KERNEL);
	printk("[msg usb]  android .c >> %s : \n" , __func__ );
	if (!config)
		return -ENOMEM;

#ifdef CONFIG_LGE_USB_GADGET_DRIVER	//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==>  Lun Count binding
	config->fsg.nluns = FSG_LUN;
	for(i = 0; i < config->fsg.nluns;i++)
		config->fsg.luns[i].removable = 1;
#else
	config->fsg.nluns = 1;
	config->fsg.luns[0].removable = 1;
#endif

	common = fsg_common_init(NULL, cdev, &config->fsg);
	if (IS_ERR(common)) {
		kfree(config);
		return PTR_ERR(common);
	}
	
#ifdef CONFIG_LGE_USB_GADGET_DRIVER	//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==>  Lun Count binding
	for(i = 0; i < config->fsg.nluns;i++)
	{
#if 0 //dongho70.kim@lge.com for Fixed
		if(i == 0)
			sprintf(&SetLunName, "lun");
		else
		{
			memset(&SetLunName, 0, sizeof(SetLunName));
			sprintf(&SetLunName, "lun%d",i);
		}
#else
        if(i == 0)
			sprintf(SetLunName, "lun");
		else
		{
			memset(SetLunName, 0, sizeof(SetLunName));
			sprintf(SetLunName, "lun%d",i);
		}
#endif
		err = sysfs_create_link(&f->dev->kobj,
					&common->luns[i].dev.kobj,
					SetLunName);
		if (err) 
		{
			kfree(config);
			return err;
		}
	}
#else
	err = sysfs_create_link(&f->dev->kobj,
				&common->luns[0].dev.kobj,
				"lun");
	if (err) {
		kfree(config);
		return err;
	}
#endif
	config->common = common;
	f->config = config;
	return 0;
}

static void mass_storage_function_cleanup(struct android_usb_function *f)
{
	kfree(f->config);
	f->config = NULL;
}

static int mass_storage_function_bind_config(struct android_usb_function *f,
						struct usb_configuration *c)
{
	struct mass_storage_function_config *config = f->config;
	return fsg_bind_config(c->cdev, c, config->common);
}

static ssize_t mass_storage_inquiry_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct mass_storage_function_config *config = f->config;
	return sprintf(buf, "%s\n", config->common->inquiry_string);
}

static ssize_t mass_storage_inquiry_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct mass_storage_function_config *config = f->config;
	if (size >= sizeof(config->common->inquiry_string))
		return -EINVAL;
	if (sscanf(buf, "%s", config->common->inquiry_string) != 1)
		return -EINVAL;
	return size;
}

static DEVICE_ATTR(inquiry_string, S_IRUGO | S_IWUSR,
					mass_storage_inquiry_show,
					mass_storage_inquiry_store);

static struct device_attribute *mass_storage_function_attributes[] = {
	&dev_attr_inquiry_string,
	NULL
};

static struct android_usb_function mass_storage_function = {
	.name		= "mass_storage",
	.init		= mass_storage_function_init,
	.cleanup	= mass_storage_function_cleanup,
	.bind_config	= mass_storage_function_bind_config,
	.attributes	= mass_storage_function_attributes,
};
/*
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN ////LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Autorun mode test
struct AutoRun_function_config {
	struct AutoRun_config fsg;
	struct AutoRun_common *common;
};

static int AutoRun_function_init(struct android_usb_function *f,
					struct usb_composite_dev *cdev)
{
	struct AutoRun_function_config *config;
	struct AutoRun_common *common;
	int err;
	char SetLunName[4];
	
	config = kzalloc(sizeof(struct AutoRun_function_config),
								GFP_KERNEL);
	printk("[msg usb]  android .c >> %s : \n" , __func__ );
	if (!config)
		return -ENOMEM;
	
	config->fsg.Fsg_lunNum = FSG_LUN;
	config->fsg.nluns = 1;
	config->fsg.luns[0].removable = 1;
	config->fsg.luns[0].cdrom = 1;
	
	common = AutoRun_common_init(NULL, cdev, &config->fsg);
	if (IS_ERR(common)) {
		kfree(config);
		return PTR_ERR(common);
	}

	memset(&SetLunName, 0, sizeof(SetLunName));
	sprintf(&SetLunName, "lun%d",FSG_LUN);
	err = sysfs_create_link(&f->dev->kobj,
				&common->luns[0].dev.kobj,
				SetLunName);
	if (err) 
	{
		kfree(config);
		return err;
	}

	config->common = common;
	f->config = config;
	return 0;
}

static void AutoRun_function_cleanup(struct android_usb_function *f)
{
	kfree(f->config);
	f->config = NULL;
}

static int AutoRun_function_bind_config(struct android_usb_function *f,
						struct usb_configuration *c)
{
	struct AutoRun_function_config *config = f->config;
	return AutoRun_bind_config(c->cdev, c, config->common);
}

static ssize_t AutoRun_inquiry_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct AutoRun_function_config *config = f->config;
	return sprintf(buf, "%s\n", config->common->inquiry_string);
}

static ssize_t AutoRun_inquiry_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct AutoRun_function_config *config = f->config;
	if (size >= sizeof(config->common->inquiry_string))
		return -EINVAL;
	if (sscanf(buf, "%s", config->common->inquiry_string) != 1)
		return -EINVAL;
	return size;
}

static DEVICE_ATTR(AutoRun_inquiry_string, S_IRUGO | S_IWUSR,
					AutoRun_inquiry_show,
					AutoRun_inquiry_store);

static struct device_attribute *AutoRun_function_attributes[] = {
	&dev_attr_AutoRun_inquiry_string,
	NULL
};

static struct android_usb_function AutoRun_function = {
	.name		= "usb_cdrom_storage", //"AutoRun",
	.init		= AutoRun_function_init,
	.cleanup	= AutoRun_function_cleanup,
	.bind_config	= AutoRun_function_bind_config,
	.attributes	= AutoRun_function_attributes,
};
#endif
*/

#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN
struct cdrom_storage_function_config {
	struct cdrom_fsg_config fsg;
	struct cdrom_fsg_common *common;
};

static int cdrom_storage_function_init(struct android_usb_function *f,
					struct usb_composite_dev *cdev)
{
	struct cdrom_storage_function_config *config;
	struct cdrom_fsg_common *common;
	int err;

	config = kzalloc(sizeof(struct cdrom_storage_function_config),
								GFP_KERNEL);
	if (!config)
		return -ENOMEM;
	
	config->fsg.vendor_name = "LGE";
	config->fsg.product_name = "CDROM storage";

	config->fsg.nluns = 1;
	config->fsg.luns[0].removable = 1;
	config->fsg.luns[0].cdrom = 1;

	common = cdrom_fsg_common_init(NULL, cdev, &config->fsg);
	if (IS_ERR(common)) {
		kfree(config);
		return PTR_ERR(common);
	}

	err = sysfs_create_link(&f->dev->kobj,
				&common->luns[0].dev.kobj,
				"lun");
	if (err) {
		cdrom_fsg_common_release(&common->ref);
		kfree(config);
		return err;
	}

	config->common = common;
	f->config = config;
	return 0;
}

static void cdrom_storage_function_cleanup(struct android_usb_function *f)
{
	kfree(f->config);
	f->config = NULL;
}

static int cdrom_storage_function_bind_config(struct android_usb_function *f,
						struct usb_configuration *c)
{
	struct cdrom_storage_function_config *config = f->config;
	return cdrom_fsg_bind_config(c->cdev, c, config->common);
}

static ssize_t cdrom_storage_inquiry_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct cdrom_storage_function_config *config = f->config;
	return snprintf(buf, PAGE_SIZE, "%s\n", config->common->inquiry_string);
}

static ssize_t cdrom_storage_inquiry_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct android_usb_function *f = dev_get_drvdata(dev);
	struct cdrom_storage_function_config *config = f->config;
	if (size >= sizeof(config->common->inquiry_string))
		return -EINVAL;
	if (sscanf(buf, "%28s", config->common->inquiry_string) != 1)
		return -EINVAL;
	return size;
}

static DEVICE_ATTR(cdrom_inquiry_string, S_IRUGO | S_IWUSR,
					cdrom_storage_inquiry_show,
					cdrom_storage_inquiry_store);

static struct device_attribute *cdrom_storage_function_attributes[] = {
	&dev_attr_cdrom_inquiry_string,
	NULL
};

static struct android_usb_function cdrom_storage_function = {
	.name		= "cdrom_storage",
	.init		= cdrom_storage_function_init,
	.cleanup	= cdrom_storage_function_cleanup,
	.bind_config	= cdrom_storage_function_bind_config,
	.attributes	= cdrom_storage_function_attributes,
};
#endif



static int accessory_function_init(struct android_usb_function *f,
					struct usb_composite_dev *cdev)
{
	return acc_setup();
}

static void accessory_function_cleanup(struct android_usb_function *f)
{
	acc_cleanup();
}

static int accessory_function_bind_config(struct android_usb_function *f,
						struct usb_configuration *c)
{
	return acc_bind_config(c);
}

static int accessory_function_ctrlrequest(struct android_usb_function *f,
						struct usb_composite_dev *cdev,
						const struct usb_ctrlrequest *c)
{
	return acc_ctrlrequest(cdev, c);
}

static struct android_usb_function accessory_function = {
	.name		= "accessory",
	.init		= accessory_function_init,
	.cleanup	= accessory_function_cleanup,
	.bind_config	= accessory_function_bind_config,
	.ctrlrequest	= accessory_function_ctrlrequest,
};


static struct android_usb_function *supported_functions[] = {
	&acm_function,
	&serial_function,
//LGE_CHANGE_S seokjae.yoon@lge.com 2012/02/13: ==> for NMEA
#if defined(CONFIG_MACH_BSSQ)
	&serial_gps_function,
//LGE_CHANGE_S seokjae.yoon@lge.com 2012/02/13: ==> for NMEA
#endif
	&ecm_function,
	&mass_storage_function,
/*
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN //LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Autorun mode test
	&AutoRun_function,
#endif	
*/	
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_AUTORUN //LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Autorun mode test
	&cdrom_storage_function,
#endif	
	&accessory_function,
	&mtp_function,
	&ptp_function,
	&adb_function,
#ifdef CONFIG_USB_SUPPORT_LGE_ANDROID_CHARGEMODE //LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Charge only mode test
	&charge_only_function,
#endif
	NULL
};

static int android_init_functions(struct android_usb_function **functions,
				  struct usb_composite_dev *cdev)
{
	struct android_dev *dev = _android_dev;
	struct android_usb_function *f;
	struct device_attribute **attrs;
	struct device_attribute *attr;
	int err;
	int index = 0;

	for (; (f = *functions++); index++) {
		f->dev_name = kasprintf(GFP_KERNEL, "f_%s", f->name);
		f->dev = device_create(android_class, dev->dev,
				MKDEV(0, index), f, f->dev_name);
		if (IS_ERR(f->dev)) {
			pr_err("%s: Failed to create dev %s", __func__,
							f->dev_name);
			err = PTR_ERR(f->dev);
			goto err_create;
		}

		if (f->init) {
			err = f->init(f, cdev);
			if (err) {
				pr_err("%s: Failed to init %s", __func__,
								f->name);
				goto err_out;
			}
		}

		attrs = f->attributes;
		if (attrs) {
			while ((attr = *attrs++) && !err)
				err = device_create_file(f->dev, attr);
		}
		if (err) {
			pr_err("%s: Failed to create function %s attributes",
					__func__, f->name);
			goto err_out;
		}
	}
	return 0;

err_out:
	device_destroy(android_class, f->dev->devt);
err_create:
	kfree(f->dev_name);
	return err;
}

static void android_cleanup_functions(struct android_usb_function **functions)
{
	struct android_usb_function *f;

	while (*functions) {
		f = *functions++;

		if (f->dev) {
			device_destroy(android_class, f->dev->devt);
			kfree(f->dev_name);
		}

		if (f->cleanup)
			f->cleanup(f);
	}
}

static int
android_bind_enabled_functions(struct android_dev *dev,
			       struct usb_configuration *c)
{
	struct android_usb_function *f;
	int ret;

	list_for_each_entry(f, &dev->enabled_functions, enabled_list) {
		printk("[msg usb] android.c >> %s : f-> name =%s \n" , __func__, f->name );
		ret = f->bind_config(f, c);
		if (ret) {
			pr_err("%s: %s failed", __func__, f->name);
			return ret;
		}
	}
	return 0;
}

static void
android_unbind_enabled_functions(struct android_dev *dev,
			       struct usb_configuration *c)
{
	struct android_usb_function *f;

	list_for_each_entry(f, &dev->enabled_functions, enabled_list) {
		if (f->unbind_config)
			f->unbind_config(f, c);
	}
}

static int android_enable_function(struct android_dev *dev, char *name)
{
	struct android_usb_function **functions = dev->functions;
	struct android_usb_function *f;
	while ((f = *functions++)) {
		if (!strcmp(name, f->name)) {
			list_add_tail(&f->enabled_list, &dev->enabled_functions);
			return 0;
		}
	}
	return -EINVAL;
}

/*-------------------------------------------------------------------------*/
/* /sys/class/android_usb/android%d/ interface */

static ssize_t
functions_show(struct device *pdev, struct device_attribute *attr, char *buf)
{
	struct android_dev *dev = dev_get_drvdata(pdev);
	struct android_usb_function *f;
	char *buff = buf;

	list_for_each_entry(f, &dev->enabled_functions, enabled_list)
		buff += sprintf(buff, "%s,", f->name);
	if (buff != buf)
		*(buff-1) = '\n';
	return buff - buf;
}

static ssize_t
functions_store(struct device *pdev, struct device_attribute *attr,
			       const char *buff, size_t size)
{
	struct android_dev *dev = dev_get_drvdata(pdev);
	char *name;
	char buf[256], *b;
	int err;

	INIT_LIST_HEAD(&dev->enabled_functions);

	strncpy(buf, buff, sizeof(buf));
	b = strim(buf);

	while (b) {
		name = strsep(&b, ",");
		printk("[usb msg] android.c >> %s , name=%s \n", __func__ ,name );
		if (name) {
			err = android_enable_function(dev, name);
			if (err)
				pr_err("android_usb: Cannot enable '%s'", name);
		}
	}

	return size;
}

static ssize_t enable_show(struct device *pdev, struct device_attribute *attr,
			   char *buf)
{
	struct android_dev *dev = dev_get_drvdata(pdev);
	return sprintf(buf, "%d\n", dev->enabled);
}

static ssize_t enable_store(struct device *pdev, struct device_attribute *attr,
			    const char *buff, size_t size)
{
	struct android_dev *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev = dev->cdev;
	int enabled = 0;

	sscanf(buff, "%d", &enabled);
	if (enabled && !dev->enabled) {
		/* update values in composite driver's copy of device descriptor */
		cdev->desc.idVendor = device_desc.idVendor;
		cdev->desc.idProduct = device_desc.idProduct;
		cdev->desc.bcdDevice = device_desc.bcdDevice;
		cdev->desc.bDeviceClass = device_desc.bDeviceClass;
		cdev->desc.bDeviceSubClass = device_desc.bDeviceSubClass;
		cdev->desc.bDeviceProtocol = device_desc.bDeviceProtocol;
		usb_add_config(cdev, &android_config_driver,
					android_bind_config);
		usb_gadget_connect(cdev->gadget);
		dev->enabled = true;
	} else if (!enabled && dev->enabled) {
		usb_gadget_disconnect(cdev->gadget);
		usb_remove_config(cdev, &android_config_driver);
		dev->enabled = false;
	} else {
		pr_err("android_usb: already %s\n",
				dev->enabled ? "enabled" : "disabled");
	}
	return size;
}

static ssize_t state_show(struct device *pdev, struct device_attribute *attr,
			   char *buf)
{
	struct android_dev *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev = dev->cdev;
	char *state = "DISCONNECTED";
	unsigned long flags;

	if (!cdev)
		goto out;

	spin_lock_irqsave(&cdev->lock, flags);
        if (cdev->config)
		state = "CONFIGURED";
	else if (dev->connected)
		state = "CONNECTED";
	spin_unlock_irqrestore(&cdev->lock, flags);
out:
	return sprintf(buf, "%s\n", state);
}

#define DESCRIPTOR_ATTR(field, format_string)				\
static ssize_t								\
field ## _show(struct device *dev, struct device_attribute *attr,	\
		char *buf)						\
{									\
	return sprintf(buf, format_string, device_desc.field);		\
}									\
static ssize_t								\
field ## _store(struct device *dev, struct device_attribute *attr,	\
		const char *buf, size_t size)		       		\
{									\
	int value;					       		\
	if (sscanf(buf, format_string, &value) == 1) {			\
		device_desc.field = value;				\
		return size;						\
	}								\
	return -1;							\
}									\
static DEVICE_ATTR(field, S_IRUGO | S_IWUSR, field ## _show, field ## _store);

#define DESCRIPTOR_STRING_ATTR(field, buffer)				\
static ssize_t								\
field ## _show(struct device *dev, struct device_attribute *attr,	\
		char *buf)						\
{									\
	return sprintf(buf, "%s", buffer);				\
}									\
static ssize_t								\
field ## _store(struct device *dev, struct device_attribute *attr,	\
		const char *buf, size_t size)		       		\
{									\
	if (size >= sizeof(buffer)) return -EINVAL;			\
	if (sscanf(buf, "%s", buffer) == 1) {			       	\
		return size;						\
	}								\
	return -1;							\
}									\
static DEVICE_ATTR(field, S_IRUGO | S_IWUSR, field ## _show, field ## _store);


DESCRIPTOR_ATTR(idVendor, "%04x\n")
DESCRIPTOR_ATTR(idProduct, "%04x\n")
DESCRIPTOR_ATTR(bcdDevice, "%04x\n")
DESCRIPTOR_ATTR(bDeviceClass, "%d\n")
DESCRIPTOR_ATTR(bDeviceSubClass, "%d\n")
DESCRIPTOR_ATTR(bDeviceProtocol, "%d\n")
DESCRIPTOR_STRING_ATTR(iManufacturer, manufacturer_string)
DESCRIPTOR_STRING_ATTR(iProduct, product_string)
DESCRIPTOR_STRING_ATTR(iSerial, serial_string)

static DEVICE_ATTR(functions, S_IRUGO | S_IWUSR, functions_show, functions_store);
static DEVICE_ATTR(enable, S_IRUGO | S_IWUSR, enable_show, enable_store);
static DEVICE_ATTR(state, S_IRUGO, state_show, NULL);

static struct device_attribute *android_usb_attributes[] = {
	&dev_attr_idVendor,
	&dev_attr_idProduct,
	&dev_attr_bcdDevice,
	&dev_attr_bDeviceClass,
	&dev_attr_bDeviceSubClass,
	&dev_attr_bDeviceProtocol,
	&dev_attr_iManufacturer,
	&dev_attr_iProduct,
	&dev_attr_iSerial,
	&dev_attr_functions,
	&dev_attr_enable,
	&dev_attr_state,
	NULL
};

/*-------------------------------------------------------------------------*/
/* Composite driver */

static int android_bind_config(struct usb_configuration *c)
{
	struct android_dev *dev = _android_dev;
	int ret = 0;

	ret = android_bind_enabled_functions(dev, c);
	if (ret)
		return ret;

	return 0;
}

static void android_unbind_config(struct usb_configuration *c)
{
	struct android_dev *dev = _android_dev;

	android_unbind_enabled_functions(dev, c);
}

static int android_bind(struct usb_composite_dev *cdev)
{
	struct android_dev *dev = _android_dev;
	struct usb_gadget	*gadget = cdev->gadget;
	int			gcnum, id, ret;

	usb_gadget_disconnect(gadget);

	printk("[msg usb]  android .c >> %s :start \n" , __func__ );
	ret = android_init_functions(dev->functions, cdev);
	if (ret)
		return ret;

	/* Allocate string descriptor numbers ... note that string
	 * contents can be overridden by the composite_dev glue.
	 */
	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_MANUFACTURER_IDX].id = id;
	device_desc.iManufacturer = id;

	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_PRODUCT_IDX].id = id;
	device_desc.iProduct = id;
#ifdef CONFIG_LGE_USB_GADGET_DRIVER		//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:
	/* Default strings - should be updated by userspace */
	strncpy(manufacturer_string, "Android", sizeof(manufacturer_string) - 1);
	strncpy(product_string, "Android", sizeof(product_string) - 1);
#else
	/* Default strings - should be updated by userspace */

	strncpy(manufacturer_string, "LG Electronics Inc.", sizeof(manufacturer_string) - 1);
	strncpy(product_string, "LGE Android Phone", sizeof(product_string) - 1);

	strncpy(serial_string, "0123456789ABCDEF", sizeof(serial_string) - 1);
//	strncpy(serial_string, "LG_ANDROID_M3MPCS_GB_", sizeof(serial_string) - 1);
#endif
	id = usb_string_id(cdev);
	if (id < 0)
		return id;
	strings_dev[STRING_SERIAL_IDX].id = id;
	device_desc.iSerialNumber = id;

	gcnum = usb_gadget_controller_number(gadget);
	if (gcnum >= 0)
		device_desc.bcdDevice = cpu_to_le16(0x0200 + gcnum);
	else {
		/* gadget zero is so simple (for now, no altsettings) that
		 * it SHOULD NOT have problems with bulk-capable hardware.
		 * so just warn about unrcognized controllers -- don't panic.
		 *
		 * things like configuration and altsetting numbering
		 * can need hardware-specific attention though.
		 */
		pr_warning("%s: controller '%s' not recognized\n",
			longname, gadget->name);
		device_desc.bcdDevice = __constant_cpu_to_le16(0x9999);
	}

#if defined(CONFIG_USB_ANDROID_NDIS ) || defined(CONFIG_USB_ANDROID_RNDIS)
		/* set up network link layer */
		ret = gether_setup(cdev->gadget, ndis_hostaddr);
		if (ret && (ret != -EBUSY)) 
		{
			gether_cleanup();
			return ret;
		}
		printk(KERN_ERR "gether, ret =%d,  USB0 = %x-%x-%x-%x-%x-%x- \n",
			 ret ,	 ndis_hostaddr[0],ndis_hostaddr[1],ndis_hostaddr[2],
				ndis_hostaddr[3],ndis_hostaddr[4],ndis_hostaddr[5]);
#endif

	usb_gadget_set_selfpowered(gadget);
	dev->cdev = cdev;
	printk("[msg usb]  android .c >> %s :end \n" , __func__ );

	return 0;
}

static int android_usb_unbind(struct usb_composite_dev *cdev)
{
	struct android_dev *dev = _android_dev;

	cancel_work_sync(&dev->work);
	android_cleanup_functions(dev->functions);
	return 0;
}

static struct usb_composite_driver android_usb_driver = {
	.name		= "android_usb",
	.dev		= &device_desc,
	.strings	= dev_strings,
	.unbind		= android_usb_unbind,
};

static int
android_setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *c)
{
	struct android_dev		*dev = _android_dev;
	struct usb_composite_dev	*cdev = get_gadget_data(gadget);
	struct usb_request		*req = cdev->req;
	struct android_usb_function	*f;
	int value = -EOPNOTSUPP;
	unsigned long flags;

	req->zero = 0;
	req->complete = composite_setup_complete;
	req->length = 0;
	gadget->ep0->driver_data = cdev;
	printk("[msg usb]  android .c >> %s : \n" , __func__ );

	list_for_each_entry(f, &dev->enabled_functions, enabled_list) {
		if (f->ctrlrequest) {
			value = f->ctrlrequest(f, cdev, c);
			if (value >= 0)
				break;
		}
	}

	/* Special case the accessory function.
	 * It needs to handle control requests before it is enabled.
	 */
	if (value < 0)
		value = acc_ctrlrequest(cdev, c);

	if (value < 0)
		value = composite_setup(gadget, c);

	spin_lock_irqsave(&cdev->lock, flags);
	if (!dev->connected) {
		dev->connected = 1;
		schedule_work(&dev->work);
	}
	else if (c->bRequest == USB_REQ_SET_CONFIGURATION && cdev->config) {
		schedule_work(&dev->work);
	}
	spin_unlock_irqrestore(&cdev->lock, flags);

	return value;
}

static void android_disconnect(struct usb_gadget *gadget)
{
	struct android_dev *dev = _android_dev;
	struct usb_composite_dev *cdev = get_gadget_data(gadget);
	unsigned long flags;

	composite_disconnect(gadget);

	spin_lock_irqsave(&cdev->lock, flags);
	dev->connected = 0;
	schedule_work(&dev->work);
	spin_unlock_irqrestore(&cdev->lock, flags);
}

static int android_create_device(struct android_dev *dev)
{
	struct device_attribute **attrs = android_usb_attributes;
	struct device_attribute *attr;
	int err;
	printk("[msg usb]  android .c >> %s : \n" , __func__ );

	dev->dev = device_create(android_class, NULL,
					MKDEV(0, 0), NULL, "android0");
	if (IS_ERR(dev->dev))
		return PTR_ERR(dev->dev);

	dev_set_drvdata(dev->dev, dev);

	while ((attr = *attrs++)) {
		err = device_create_file(dev->dev, attr);
		if (err) {
			device_destroy(android_class, dev->dev->devt);
			return err;
		}
	}
	return 0;
}


static int __init init(void)
{
	struct android_dev *dev;
	int err;

	android_class = class_create(THIS_MODULE, "android_usb");
	if (IS_ERR(android_class))
		return PTR_ERR(android_class);

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;
	
	printk("[msg usb]  android .c >> %s : \n" , __func__ );
#ifdef CONFIG_LGE_USB_GADGET_DRIVER//LGE_CHANGE_S  youngjae.ko@lge.com 2012/01/12:  ==> Serial flages acm&serial	
	dev->serial_flags = 0x00; 
#endif	
	dev->functions = supported_functions;
	INIT_LIST_HEAD(&dev->enabled_functions);
	INIT_WORK(&dev->work, android_work);

	err = android_create_device(dev);
	if (err) {
		class_destroy(android_class);
		kfree(dev);
		return err;
	}

	_android_dev = dev;

	/* Override composite driver functions */
	composite_driver.setup = android_setup;
	composite_driver.disconnect = android_disconnect;

	return usb_composite_probe(&android_usb_driver, android_bind);
}
module_init(init);

static void __exit cleanup(void)
{
	usb_composite_unregister(&android_usb_driver);
	class_destroy(android_class);
	kfree(_android_dev);
	_android_dev = NULL;
}
module_exit(cleanup);
