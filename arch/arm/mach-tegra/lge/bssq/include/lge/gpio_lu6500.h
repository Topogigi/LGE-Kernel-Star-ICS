const struct tegra_init_gpio_info tegra_init_gpio_info_array[] = {
	{ 'm'-'a',  	2,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,		LHP1}, // BT_EN
	{ 'm'-'a',  	3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,		LHP2}, // WLAN_EN

	/* All GPIO output pins should be defined here */
#if defined (CONFIG_LU6500) || defined (CONFIG_SU880) || defined (CONFIG_KU8800) || defined (CONFIG_LU8800) || defined (CONFIG_KS1103)
	{ 't'-'a',      2,	GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_LOW,		DTB},   // FLASH_LED_TOURCH
	{ 't'-'a',      3,	GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_LOW,		DTB},   // FLASH_LED_INH
	{ 'z'-'a' + 2,  4,	GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_LOW,		DTE},   // FLASH_LED_EN 
#endif
	{ 'f'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	  		LD10}, 	// 1SEG_A_1.2V_EN
	{ 'f'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	  		LD11}, 	// 1SEG_EN
	{ 'u'-'a',  	4,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,			GPU}, 	// VIBE_EN
	{ 'x'-'a',		4,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,	  		SPID}, 	// BT_WAKEUP
	{ 'd'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,	  		DTE}, 	// 8M_RESET_N
	{ 'z'-'a' + 2,	4,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,	  		DTE}, 	// FLASH_LED_EN
	{ 'd'-'a',  	5,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH,		DTA}, 	// VT_CAM_PWDN
	{ 'z'-'a' + 2,	1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,			DTE}, 	// VT_RESET_N
	{ 'o'-'a',  	0,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		UAB}, 	// IPC_MRDY2 ??
	{ 'v'-'a',  	7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LVP0}, 	// LCD_RESET_N
#if defined (CONFIG_KS1001) || defined (CONFIG_KS1103)
	{ 'e'-'a',  	7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD7}, 	// LCD_RESET_N in rev_C
#endif
	{ 'j'-'a',  	3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LHS}, 	// BL_DCDC_SCL ??
	{ 'j'-'a',  	4,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LVS}, 	// BL_DCDC_SDL ??
	{ 'e'-'a',  	1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD1}, 	// I2C2_SW
	{ 'e'-'a',  	3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD3}, 	// LCD_CP_EN ??
	{ 'e'-'a',  	6,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD6}, 	// DMB_EN ??
	{ 'f'-'a',  	2,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD10}, 	// CAM_SUBPM_EN
	{ 'f'-'a',  	7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		LD15}, 	// WM_LDO_EN
	{ 'j'-'a', 		6,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		IRRX}, 	// IPC_MRDY1 ??
	{ 'x'-'a',  	7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		SPIF}, 	// EAR_BIAS_EN
	{ 'i'-'a',  	7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// MUIC_GAUGE_SCL?? IN or OUT??
	{ 'k'-'a',  	4,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// MUIC_GAUGE_SDA?? IN or OUT??

// [START] kangsic.ham 2011.06.15 - GPS UART & GPIO Setting 
#if defined(CONFIG_SU880) || defined(CONFIG_KU8800)
    { 'f'-'a',      5, GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_HIGH,  LD13},   // GPS_RESET_N
    { 'f'-'a',      6, GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_LOW,   LD14},   // GPS_PWR_ON
    { 'm'-'a',      0, GPIO_ENABLE, GPIO_OUTPUT,    GPIO_SLEEP_LOW,   LD16},   // GPS_LNA
#endif
// [END] kangsic.ham 2011.06.15 - GPS UART & GPIO Setting

	/* All wakeup pins should be defined here : gpio input enable */

// 20110409 hyeongwon.oh@lge.com add charger ic [S]
#if defined (CONFIG_LU6500) || defined (CONFIG_SU880) || defined (CONFIG_KU8800) || defined (CONFIG_LU8800)
	{ 's'-'a',      1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,			KBCB},  // CHG_EN_SET_N_AP20
	{ 's'-'a',      2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			KBCB},  // CHG_STATUS_N_AP20
	{ 'c'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			LPW1},  // CHG_PGB_N_AP20
#endif
// 20110409 hyeongwon.oh@lge.com add charger ic [E]
	{ 'o'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UAB}, 	// IPC_SRDY2
#if defined (CONFIG_LU6500) || defined (CONFIG_LU8800)	// 20110622, kangsic.ham
	{ 'j'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		GMC},	// HOOK_DET, only LGU+, kangsic.ham
#endif
	{ 'c'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		GMB},	// BT_HOST_WAKEUP
	{ 's'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	  		KBCB},	// WLAN_HOST_WAKEUP
	{ 'x'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	  		SPIE},	// PROX_OUT
	{ 'w'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIH},	// BATT_LOW_INT
	{ 'w'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIG},	// CHARGER_IRQB ??? cf.CHG_STATUS_N_AP20

	/* tristate group's input pins */
	{ 'f'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LD8}, 	// COMPASS_INT
	{ 'f'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LD12}, 	// MOTION_INT
#if defined (CONFIG_KS1001) || defined (CONFIG_KS1103)
	{ 'f'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LSC0}, 	// GYRO_INT_N in rev_C
#endif
#if !defined (CONFIG_KS1103)
	{ 'b'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LSC0}, 	// GYRO_INT_N
#endif

	/* group tristate or input */
	{ 'o'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			UAB}, 	// 1SEG_INT(DMB_INT)
//wonhee.jeong@lge.com 20110611 for Rev Change in TDMB [S]
	{ 'e'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW, 		LD5},	// DMB_INT in LU6500 Rev.D and X2
//wonhee.jeong@lge.com 20110611 for Rev Change in TDMB [E]
	{ 'e'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	  		LD2}, 	// AUDIO_INT_N ??
	{ 'm'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW,	  		LVP1}, 	// LCD_MAKER_ID
	{ 'n'-'a',		7,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW,	  		HDINT}, // GAUGE_INT
	{ 'u'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW,	  		GPU}, 	// MUIC_INT_N
#if defined (CONFIG_SU880) || defined (CONFIG_KU8800) || defined (CONFIG_LU8800) || defined (CONFIG_KS1103)
	{ 'u'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH,	  		GPU}, 	// MUIC_INT
#else
	{ 'u'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW,	  		GPU}, 	// SLIDE_DETECT
#endif
	{ 'u'-'a',		6,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW,	  		GPU}, 	// IPC_SRDY1 ???
	{ 'x'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIE}, 	// TOUCH_INT_N
	{ 'i'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATB}, 	// MICROSD_DET_N
	{ 'k'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// THERMAL_IRQ
	{ 'g'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// VOL_KEY_UP
	{ 'g'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// VOL_KEY_DOWN
#if defined (CONFIG_KS1103)
	{ 'b'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// EARJACK_SENSE
#else
	{ 'g'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW, 		ATC},	// EARJACK_SENSE
#endif
// 20110721 hyeongwon.oh@lge.com NC input LOW [S]
#if defined (CONFIG_LU6500) 
	{ 'k'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// NC
	{ 'g'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// NC
	{ 'g'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// NC
	{ 'g'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// NC
	{ 'g'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// NC
#endif 
// 20110721 hyeongwon.oh@lge.com NC input LOW [E]
};

const struct tegra_init_gpio_info tegra_sleep_gpio_info_array[] = {
//	{ 0xFF,         0,	SFIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			PMC},	// PMC ?????????
	{ 'o'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UAA}, 	// UART1
	{ 'o'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UAA}, 	// UART1
	{ 'o'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UAA}, 	// UART1
	{ 'o'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UAA}, 	// UART1
	{ 'o'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UAB}, 	// IPC_SRDY2
	{ 'o'-'a',		6,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		UAB}, 	// NC
	{ 'o'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		UAB}, 	// NC
	{ 'o'-'a',		0,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	UAB},	// IPC_MRDY2
	{ 'y'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UDA}, 	// SPI1
	{ 'y'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UDA}, 	// SPI1
	{ 'y'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UDA}, 	// SPI1
	{ 'y'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UDA}, 	// SPI1
	{ 'p'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP3}, 	// DAP3
	{ 'p'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP3}, 	// DAP3
	{ 'p'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP3}, 	// DAP3
	{ 'p'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP3}, 	// DAP3
	{ 'z'-'a',		0,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_CLK
	{ 'z'-'a',		1,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_CMD
	{ 'y'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_SDIO[0]
	{ 'y'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_SDIO[1]
	{ 'y'-'a',		5,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_SDIO[2]
	{ 'y'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	SDIO1},	// WLAN_SDIO[3]
	{ 'v'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		UAC}, 	// RESET_MDM
	{ 'v'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		UAC}, 	// MDM_PWR_ON
	{ 'v'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UAC}, 	// AP_PWN_ON
	{ 'v'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		UAC}, 	// MDM_RESET_FLAG
	{ 'b'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LPW0}, 	// SPI3
	{ 'c'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		LPW1}, 	// NC
	{ 'c'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LPW2}, 	// SPI3
	{ 'z'-'a',		2,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LSDI}, 	// NC
	{ 'n'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LSDA}, 	// NC
	{ 'n'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	LCSN},	// LCD_CS
	{ 'n'-'a',		6,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LDC}, 	// NC
	{ 'z'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LSCK}, 	// NC
	{ 'w'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LM0}, 	// SPI3
	{ 'v'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		LVP0}, 	// NC
	{ 'z'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		LSC1}, 	// SPI3
	{ 'w'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			LM1}, 	// NC
	{ 'j'-'a',		1,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	LSPI},	// LCD_TE
	{ 'b'-'a',		3,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LSC0}, 	// GYRO_INT_N
	{ 'j'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			LHS}, 	// BL_DCDC_SCL
	{ 'j'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			LVS}, 	// BL_DCDC_SDA
	{ 'e'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			LD0}, 	// EARJACK_SENSE
	{ 'e'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		LD1}, 	// I2C2_SW
	{ 'e'-'a',		2,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			LD2}, 	// AUDIO_INT_N
	{ 'e'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		LD3}, 	// LCD_CP_EN
	{ 'e'-'a',		4,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		LD4}, 	// DMB_RESET/
	{ 'e'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW, 			LD5},	// DMB_INT in LU6500 Rev.D and X2
	{ 'e'-'a',		6,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		LD6}, 	// DMB_EN
	{ 'e'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		LD7}, 	// LCD_RESET_N
	{ 'f'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LD8}, 	// COM_INT
	{ 'f'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		LD9}, 	// MDM_USB_VBUS_EN
	{ 'f'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	  	LD10}, 	// CAM_SUBPM_EN
	{ 'f'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	LD11}, 	// VIBE_EN
	{ 'f'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LD12}, 	// ACCELERO_INT
	{ 'f'-'a',		5,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	LD13}, 	// KEY_LED_RESET
	{ 'f'-'a',		6,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	LD14}, 	// NC
	{ 'f'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		LD15}, 	// WM_LDO_EN
	{ 'm'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	LD16}, 	// NC
	{ 'm'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	 		LD17}, 	// NC
	{ 'm'-'a', 		2,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,		LHP1},	// BT_EN
	{ 'm'-'a', 		3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,		LHP2},	// WLAN_EN
	{ 'm'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			LVP1}, 	// LCD_MAKER_ID
	{ 'm'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	 		LHP0}, 	// NC
	{ 'm'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	 		LDI}, 	// NC
	{ 'm'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	 		LPP}, 	// NC
	{ 'n'-'a',		7,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			HDINT}, // GAUGE_INT
	{ 't'-'a',		0,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_PCLK
	{ 't'-'a',		1,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	CSUS},	// CAM_MCLK
	{ 't'-'a',		4,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	DTA}, 	// 5M_CAM_VCM_EN
	{ 'd'-'a', 		5,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 	 	DTA}, 	// VT_CAM_PWDN
	{ 'l'-'a',		0,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[0]
	{ 'l'-'a',		1,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[1]
	{ 'l'-'a',		2,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[2]
	{ 'l'-'a',		3,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[3]
	{ 'l'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[4]
	{ 'l'-'a',		5,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[5]
	{ 'l'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[6]
	{ 'l'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTD},	// VT_DATA[7]
	{ 't'-'a', 		2,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,   		DTB},   // VT_RESET/
	{ 't'-'a', 		3,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,   		DTB},   // 5M_RESET/
	{ 'd'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTC},	// VT_VSYNC
	{ 'd'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	DTC},	// VT_HSYNC
	{ 'z'-'a' + 2,		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			DTF}, 	// CAM_I2C_SCL
	{ 'z'-'a' + 2,		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			DTF}, 	// CAM_I2C_SDA
	{ 'z'-'a' + 2,		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,		DTE}, 	// NC
	{ 'z'-'a' + 2,		4,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,		DTE}, 	// FLASH_LED_EN
	{ 'z'-'a' + 2,		5,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,		DTE}, 	// NC
	{ 'd'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,		DTE}, 	// NC
	{ 'a'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT,  	GPIO_SLEEP_LOW,	 	DTE}, 	// NC
	{ 'j'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	IRRX},	// IPC_MRDY1
	{ 'j'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,  	GPIO_SLEEP_LOW,	 		IRTX}, 	// NC
	{ 'w'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UCA}, 	// BT_UART
	{ 'w'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UCA}, 	// BT_UART
	{ 'c'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UCB}, 	// BT_UART
	{ 'a'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		UCB}, 	// BT_UART
	{ 'u'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		GPU}, 	// NC
	{ 'u'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		GPU}, 	// NC
	{ 'u'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		GPU}, 	// UART_SW
	{ 'u'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		GPU}, 	// VIBE_PWM
	{ 'u'-'a',		4,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,		GPU}, 	// NC
	{ 'u'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			GPU}, 	// SLIDE_DETECT
	{ 'u'-'a',		6,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			GPU}, 	// IPC_SRDY1
	{ 'c'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			RM}, 	// GEN1_I2C_SCA
	{ 'c'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			RM}, 	// GEN1_I2C_SCL
	{ 'p'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP4}, 	// BT_PCM
	{ 'p'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP4}, 	// BT_PCM
	{ 'p'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP4}, 	// BT_PCM
	{ 'p'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP4}, 	// BT_PCM
	{ 'r'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCA}, 	// KEY_R[0]
	{ 'r'-'a',		1,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCA}, 	// KEY_R[1]
	{ 'r'-'a',		2,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCA}, 	// KEY_R[2]
	{ 'r'-'a',		3,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCD}, 	// KEY_R[3]
	{ 'r'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCD}, 	// KEY_R[4]
	{ 'r'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCD}, 	// KEY_R[5]
	{ 'r'-'a',		6,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCD}, 	// KEY_R[6]
	{ 'r'-'a',		7,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCB}, 	// KEY_R[7]
	{ 's'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	  		KBCB}, 	// WLAN_HOST_WAKEUP
	{ 's'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_INIT_ONLY_LOW,		KBCB},	// CHG_EN_SET_N_AP25
	{ 's'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	  		KBCB}, 	// CHG_STATUS_N_AP25
	{ 'q'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCC}, 	// KEY_C[0]
	{ 'q'-'a',		1,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCC}, 	// KEY_C[1]
	{ 'q'-'a',		2,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCF}, 	// KEY_C[2]
	{ 'q'-'a',		3,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCF}, 	// KEY_C[3]
	{ 'q'-'a',		4,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCF}, 	// KEY_C[4]
	{ 'q'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			KBCF}, 	// KEY_C[5]
	{ 'z'-'a' + 2,	0,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	PMC},	// CLK32K
	{ 'z'-'a',  		5,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,		PMC}, 	// NC
	{ 'z'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			I2CP}, 	// PWR_I2C_SCL
	{ 'z'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			I2CP}, 	// PWR_I2C_SCA
	{ 'u'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW, 		GPU7}, 	// JTAG_RTCK_AP25
	{ 'n'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP1}, 	// DAP1
	{ 'n'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP1}, 	// DAP1
	{ 'n'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP1}, 	// DAP1
	{ 'n'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP1}, 	// DAP1
	{ 'w'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	CDEV1},	// AUDIO_MCLK
	{ 'k'-'a',  	5,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW,		SPDO}, 	// NC
	{ 'k'-'a',  	6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			SPDI}, 	// NC
	{ 'w'-'a',		5,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	CDEV2},	// AUDIO_MCLK2
	{ 'a'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP2}, 	// DAP2
	{ 'a'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP2}, 	// DAP2
	{ 'a'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP2}, 	// DAP2
	{ 'a'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		DAP2}, 	// DAP2
	{ 'x'-'a',		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIA}, 	// SPI2
	{ 'x'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIB}, 	// SPI2
	{ 'x'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIC}, 	// SPI2
	{ 'x'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		SPIC}, 	// SPI2
	{ 'x'-'a',		4,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_LOW,		SPID}, 	// BT_WAKEUP
	{ 'x'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	  		SPIE}, 	// PROX_OUT
	{ 'x'-'a',		6,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			SPIE}, 	// TOUCH_INT_N
	{ 'x'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_LOW, 		SPIF}, 	// EAR_BIAS_EN
	{ 'w'-'a',		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		SPIG}, 	// MUIC_INT/
	{ 'w'-'a',		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		SPIH}, 	// BATT_LOW_INT
	{ 'a'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDD},	// MICROSD_CLK
	{ 'a'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDD},	// MICROSD_CMD
	{ 'b'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDC},	// MICROSD_DATA0
	{ 'b'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDC},	// MICROSD_DATA1
	{ 'b'-'a',		5,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDC},	// MICROSD_DATA2
	{ 'b'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_HIGH,	SDC},	// MICROSD_DATA3
	{ 'd'-'a', 		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			SLXA}, 	// NC
	{ 'd'-'a', 		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			SLXK}, 	// NC
	{ 'd'-'a', 		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			SLXC}, 	// NC
	{ 'd'-'a', 		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			SLXD}, 	// NC
	{ 'v'-'a', 		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			GPV}, 	// NC
	{ 'i'-'a',		5,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			ATB}, 	// MICROSD_DET_N
	{ 'i'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			ATC}, 	// MUIC_GAUGE_SCL
	{ 'k'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			ATC}, 	// GMI_ADV_N
	{ 'k'-'a',		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// PULL_DOWN
	{ 'j'-'a', 		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			GMD}, 	// NC
	{ 'j'-'a', 		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			GMD}, 	// NC
	{ 'k'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		ATC}, 	// NC
	{ 'k'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			ATC}, 	// MUIC_GAUGE_SDA
	{ 'k'-'a',		2,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_LOW, 			ATC}, 	// THERMAL_IRQ
	{ 'g'-'a',		0,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			ATC}, 	// VOL_KEY_UP
	{ 'g'-'a',		1,	GPIO_ENABLE, GPIO_INPUT, 	GPIO_SLEEP_HIGH, 			ATC}, 	// VOL_KEY_DOWN
	{ 'g'-'a',		2,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_HIGH, 		ATC}, 	// WLAN_WAKEUP/
	{ 'g'-'a',		3,	GPIO_ENABLE, GPIO_OUTPUT, 	GPIO_SLEEP_HIGH, 		ATC}, 	// NC
	{ 'g'-'a',		4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// PULL_DOWN
	{ 'g'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// PULL_DOWN
	{ 'g'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// PULL_DOWN
	{ 'g'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		ATC}, 	// PULL_DOWN
	{ 'h'-'a', 		0,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			ATD}, 	// NC
	{ 'h'-'a', 		1,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			ATD}, 	// NC
	{ 'h'-'a', 		2,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			ATD}, 	// NC
	{ 'h'-'a', 		3,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			ATD}, 	// NC
	{ 'h'-'a',		4,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	ATE},	// PULL DOWN(DNI)
	{ 'h'-'a',		5,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	ATE},	// PULL DOWN
	{ 'h'-'a',		6,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	ATE},	// PULL DOWN
	{ 'h'-'a',		7,	GPIO_ENABLE,	GPIO_INPUT,	GPIO_SLEEP_LOW,	ATE},	// PULL DOWN
	{ 'j'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,	 		GMC}, 	// HOOK_DET, only LGU+, kangsic.ham
	{ 'b'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		GMC}, 	// NC
	{ 'b'-'a',		1,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		GMC}, 	// NC
	{ 'k'-'a',		7,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		GMC}, 	// NC
	{ 'i'-'a',		0,	GPIO_ENABLE, GPIO_OUTPUT,	GPIO_SLEEP_HIGH, 		ATC}, 	// NC
	{ 'i'-'a',		1, 	 GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH,			ATC},	// GMI_OE_N
	{ 'i'-'a',  	4,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,			ATA}, 	// NC
	{ 't'-'a',		5,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			PTA}, 	// GEN2_I2C_SCL
	{ 't'-'a',		6,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_HIGH, 			PTA}, 	// GEN2_I2C_SCA
	{ 'c'-'a',		7,	GPIO_ENABLE, GPIO_INPUT,	GPIO_SLEEP_LOW,	 		GMB},	// BT_HOST_WAKEUP
};
