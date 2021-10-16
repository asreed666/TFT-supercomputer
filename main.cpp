/* Hello World! for the Emwin TFT Library */

#include "mbed.h"
#include "GUI.h"
#include "cy8ckit_028_tft.h"
#include "PinDetect.h"

PinDetect pb1(SWITCH2);
uint8 counter = 0;
AnalogIn lightLevel(P10_0);

// Callback routine is interrupt activated by a debounced pb1 hit
void pb1_hit_callback (void)
{
    counter = 0;
}

int readLightLevel() {
    int lightPercent;
    lightPercent = (lightLevel.read_u16()*100)/65535;
    return lightPercent;
    }

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
    pb1.mode(PullUp);
    // Delay for initial pullup to take effect
    ThisThread::sleep_for(10);
    // Setup Interrupt callback functions for a pb hit
    pb1.attach_deasserted(&pb1_hit_callback);

    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency();


    GUI_SetFont(GUI_FONT_8X16X2X2);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Hello World!", 160, 200);
    while(1) {
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispHexAt(counter, 160, 70, 2);
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispDecAt(counter, 160, 130, 3);
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispBinAt(counter++, 160, 100, 8);
        GUI_DispStringAt("Light is: ", 10, 160);
        GUI_DispDec(readLightLevel(), 3);
        GUI_DispString("%");
        ThisThread::sleep_for(500);

    }
}



