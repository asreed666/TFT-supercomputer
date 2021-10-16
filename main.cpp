/* Hello World! for the Emwin TFT Library */

#include "mbed.h"
#include "GUI.h"
#include "cy8ckit_028_tft.h"
#include "PinDetect.h"

PinDetect pb1(SWITCH2);
uint8 counter = 0;
AnalogIn lightLevel(P10_0);
Serial          pc(USBTX, USBRX);

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
    pc.printf("tft demo\r\n");


    GUI_SetFont(GUI_FONT_8X16X2X2);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Hello World!", 160, 40);
    while(1) {
        
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispHexAt(counter, 160, 100, 2);
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispDecAt(counter, 160, 70, 3);
        for(int pos = 0; pos < 8; pos++) {
            if ((counter >> pos & 0x01) == 1) {
                GUI_SetColor(GUI_BLUE);
            }
            else {
                GUI_SetColor(GUI_DARKBLUE);
            }
//            GUI_FillRect(208 - (16*pos), 140, 224 - (2+16 * pos), 156);
            GUI_FillCircle(216 - (16*pos), 148, 8);
        }
        GUI_SetColor(GUI_WHITE);
        GUI_SetTextAlign(GUI_TA_HCENTER);
        GUI_DispBinAt(counter++, 160, 160, 8);
        GUI_DispStringAt("Light is: ", 40, 200);
        GUI_DispDec(readLightLevel(), 3);
        GUI_DispString("%");
        ThisThread::sleep_for(500);

    }
}



