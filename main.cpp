/* Hello World! for the TextLCD Enhanced Library*/

#include "mbed.h"
#include "GUI.h"
#include "cy8ckit_028_tft.h"

void Display_Init(void)
{

    /* Set font size, foreground and background Colours */
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);

    /* Clear screen and print splash screen */
    GUI_Clear();
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("TFT Demo", 160, 20);
}

int main()
{
    /* Initialise EmWin driver*/
    GUI_Init();

    /* Initialise display */
    Display_Init();


    GUI_SetFont(GUI_FONT_8X16X2X2);
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispStringAt("Hello World!", 160, 200);

}



