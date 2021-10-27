/* Hello World! for the Emwin TFT Library */

#include "mbed.h"
#include "GUI.h"
#include "cy8ckit_028_tft.h"

#define LED_DIAMETER    (16)
#define LED_RADIUS      (LED_DIAMETER/2)
#define DISPLAY_X       (320)
#define DISPLAY_Y       (240)
#define NUMLEDS_X       (DISPLAY_X/LED_DIAMETER)
#define NUMLEDS_Y       (DISPLAY_Y/LED_DIAMETER)
#define LED_OFF         (0)
#define LED_ON          (1)
#define TITLE_OFFSET    (16+LED_RADIUS)
#undef  COLOURED_LEDS

void Display_Init(void)
{
    /* Initialise EmWin driver*/
    GUI_Init();

    /* Set font size, foreground and background Colours */
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);

    /* Clear screen and print splash screen */
    GUI_Clear();
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("CITY3032 Super Computer", 160, 0);
}

int main()
{
    /* Initialise display */
    Display_Init();

    printf("Starting Supercomputer\r\n");

    GUI_SetFont(GUI_FONT_8X16X2X2);
//    GUI_SetColor(GUI_DARKGREEN);
    uint32_t array[NUMLEDS_X][NUMLEDS_Y];
    for (int j=0; j < NUMLEDS_Y; j++) {
        for (int i=0; i < NUMLEDS_X; i++) {
            array[i][j]=GUI_WHITE;
            GUI_SetColor(GUI_WHITE);
//            GUI_DrawCircle(LED_RADIUS+(i)*LED_DIAMETER, TITLE_OFFSET+(j*LED_DIAMETER), LED_RADIUS);
        }
    }
    while(1) {
        int ledx = rand()%NUMLEDS_X;
        int ledy = rand()%NUMLEDS_Y;
        array[ledx][ledy] = (array[ledx][ledy] - 0x00101010)&0x00f0f0f0;  // dim the led
        if (array[ledx][ledy]==0) {
#ifdef COLOURED_LEDS
             uint32_t colour = rand()%4;
             switch(colour) {
                case 0: 
                    GUI_SetColor(GUI_LIGHTBLUE);
                    array[ledx][ledy] = GUI_LIGHTBLUE;
                    break;
                case 1: 
                    GUI_SetColor(GUI_LIGHTGREEN);
                    array[ledx][ledy] = GUI_LIGHTGREEN;
                    break;
                case 2: 
                    array[ledx][ledy] = GUI_LIGHTRED;
                    GUI_SetColor(GUI_LIGHTRED);
                    break;
                default: 
                    GUI_SetColor(GUI_YELLOW);
                    array[ledx][ledy] = GUI_YELLOW|0x808080;
                    break;
            }
#else
                    GUI_SetColor(GUI_LIGHTGREEN);
#endif
             GUI_FillCircle(LED_RADIUS+(ledx)*LED_DIAMETER, TITLE_OFFSET+LED_DIAMETER*(ledy), LED_RADIUS);
//             array[ledx][ledy] = LED_ON;
        }
        else {
//             array[ledx][ledy]=array[ledx][ledy] >> 1;  // dim the led
             GUI_SetColor(array[ledx][ledy]);
             GUI_FillCircle(LED_RADIUS+(ledx)*LED_DIAMETER, TITLE_OFFSET+LED_DIAMETER*(ledy), LED_RADIUS);
             
//             GUI_SetColor(GUI_DARKGREEN);
//             GUI_DrawCircle(LED_RADIUS+(ledx)*LED_DIAMETER, TITLE_OFFSET+LED_DIAMETER*(ledy), LED_RADIUS);
//             GUI_FillCircle(8+(ledx)*16, 24+16*(ledy), 8);
//             array[ledx][ledy] = LED_OFF;
        }
    }
}



