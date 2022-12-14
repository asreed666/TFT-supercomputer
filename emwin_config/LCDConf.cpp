/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.46 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software  has been licensed to  Cypress Semiconductor Corporation,
whose registered  office is situated  at 198 Champion Ct. San Jose, CA 
95134 USA  solely for the  purposes of creating  libraries for Cypress
PSoC3 and  PSoC5 processor-based devices,  sublicensed and distributed
under  the  terms  and  conditions  of  the  Cypress  End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              Cypress Semiconductor Corp, 198 Champion Ct., San Jose, CA 95134, USA
Licensed SEGGER software: emWin
License number:           GUI-00319
License model:            Services and License Agreement, signed June 10th, 2009
Licensed platform:        Any Cypress platform (Initial targets are: PSoC3, PSoC5)
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2009-06-12 - 2022-07-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUIDRV_FlexColor.h"

#include "cy8ckit_028_tft.h"


/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//
// Physical display size
//   The display size should be adapted in order to match the size of
//   the target display.
//
#define XSIZE_PHYS 240
#define YSIZE_PHYS 320

//
// Color conversion
//   The color conversion functions should be selected according to
//   the color mode of the target display. Details can be found in
//   the chapter "Colors" in the emWin user manual.
//
#define COLOR_CONVERSION GUICC_M565

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif

/********************************************************************
*
*       CY8CKIT_028_TFT_InitController
*
* Purpose:
*   Initializes the LCD controller
*
*/
static void CY8CKIT_028_TFT_InitController(void) {
  /* Set up the display controller and put it into operation. If the 
  *  display controller is not initialized by any external routine 
  *  this needs to be adapted by the customer.
  */
	DisplayIntf_Init();

/*
	Cy_GPIO_Set(LCD_RESET_PORT, LCD_RESET_NUM);
	GUI_Delay(20);
	Cy_GPIO_Clr(LCD_RESET_PORT, LCD_RESET_NUM);
	GUI_Delay(100);
	Cy_GPIO_Set(LCD_RESET_PORT, LCD_RESET_NUM);
	GUI_Delay(100);
 */
    LCD_RESET = 1u;
	GUI_Delay(20);
	LCD_RESET = 0u;
	GUI_Delay(100);
	
    LCD_RESET = 1u;
	GUI_Delay(100);

	DisplayIntf_Write8_A0(0x28);
	DisplayIntf_Write8_A0(0x11);	/* Exit Sleep mode */
	GUI_Delay(100);
	DisplayIntf_Write8_A0(0x36);
	DisplayIntf_Write8_A1(0xA0);	/* MADCTL: memory data access control */
	DisplayIntf_Write8_A0(0x3A);
	DisplayIntf_Write8_A1(0x65);	/* COLMOD: Interface Pixel format */
	DisplayIntf_Write8_A0(0xB2);
	DisplayIntf_Write8_A1(0x0C);
	DisplayIntf_Write8_A1(0x0C);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x33);
	DisplayIntf_Write8_A1(0x33);	/* PORCTRK: Porch setting */
	DisplayIntf_Write8_A0(0xB7);
	DisplayIntf_Write8_A1(0x35);	/* GCTRL: Gate Control */
	DisplayIntf_Write8_A0(0xBB);
	DisplayIntf_Write8_A1(0x2B);	/* VCOMS: VCOM setting */
	DisplayIntf_Write8_A0(0xC0);
	DisplayIntf_Write8_A1(0x2C);	/* LCMCTRL: LCM Control */
	DisplayIntf_Write8_A0(0xC2);
	DisplayIntf_Write8_A1(0x01);
	DisplayIntf_Write8_A1(0xFF);	/* VDVVRHEN: VDV and VRH Command Enable */
	DisplayIntf_Write8_A0(0xC3);
	DisplayIntf_Write8_A1(0x11);	/* VRHS: VRH Set */
	DisplayIntf_Write8_A0(0xC4);
	DisplayIntf_Write8_A1(0x20);	/* VDVS: VDV Set */
	DisplayIntf_Write8_A0(0xC6);
	DisplayIntf_Write8_A1(0x0F);	/* FRCTRL2: Frame Rate control in normal mode */
	DisplayIntf_Write8_A0(0xD0);
	DisplayIntf_Write8_A1(0xA4);
	DisplayIntf_Write8_A1(0xA1);	/* PWCTRL1: Power Control 1 */
	DisplayIntf_Write8_A0(0xE0);
	DisplayIntf_Write8_A1(0xD0);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x05);
	DisplayIntf_Write8_A1(0x0E);
	DisplayIntf_Write8_A1(0x15);
	DisplayIntf_Write8_A1(0x0D);
	DisplayIntf_Write8_A1(0x37);
	DisplayIntf_Write8_A1(0x43);
	DisplayIntf_Write8_A1(0x47);
	DisplayIntf_Write8_A1(0x09);
	DisplayIntf_Write8_A1(0x15);
	DisplayIntf_Write8_A1(0x12);
	DisplayIntf_Write8_A1(0x16);
	DisplayIntf_Write8_A1(0x19);	/* PVGAMCTRL: Positive Voltage Gamma control */
	DisplayIntf_Write8_A0(0xE1);
	DisplayIntf_Write8_A1(0xD0);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x05);
	DisplayIntf_Write8_A1(0x0D);
	DisplayIntf_Write8_A1(0x0C);
	DisplayIntf_Write8_A1(0x06);
	DisplayIntf_Write8_A1(0x2D);
	DisplayIntf_Write8_A1(0x44);
	DisplayIntf_Write8_A1(0x40);
	DisplayIntf_Write8_A1(0x0E);
	DisplayIntf_Write8_A1(0x1C);
	DisplayIntf_Write8_A1(0x18);
	DisplayIntf_Write8_A1(0x16);
	DisplayIntf_Write8_A1(0x19);	/* NVGAMCTRL: Negative Voltage Gamma control */
	DisplayIntf_Write8_A0(0x2B);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0xEF);	/* Y address set */
	DisplayIntf_Write8_A0(0x2A);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x00);
	DisplayIntf_Write8_A1(0x01);
	DisplayIntf_Write8_A1(0x3F);	/* X address set */
	GUI_Delay(10);
	DisplayIntf_Write8_A0(0x29);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description
*   Called during the initialization process in order to set up the
*   display driver configuration.
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  //
  // Set the display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  LCD_SetSizeEx    (0, XSIZE_PHYS,   YSIZE_PHYS);
  LCD_SetVSizeEx   (0, VXSIZE_PHYS,  VYSIZE_PHYS);
  //
  // Orientation
  //
  Config.Orientation   = GUI_MIRROR_Y | GUI_SWAP_XY;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite8_A0  = DisplayIntf_Write8_A0;
  PortAPI.pfWrite8_A1  = DisplayIntf_Write8_A1;
  PortAPI.pfWriteM8_A1 = DisplayIntf_WriteM8_A1;
  PortAPI.pfRead8_A1   = DisplayIntf_Read8_A1;
  PortAPI.pfReadM8_A1  = DisplayIntf_ReadM8_A1;
  
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task, the routine needs to be adapted to
*   the display controller. Note that the commands marked
*   "optional" are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Zero based layer index
*   Cmd        - Command to be executed
*   pData      - Pointer to a data structure.
* 
* Return Value:
*   < -1 - Error
*     -1 - The command is not handled.
*      0 - OK.
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  
  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pData);
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine, this needs
    // to be adapted by the customer...
    //
    // ...
    CY8CKIT_028_TFT_InitController();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

