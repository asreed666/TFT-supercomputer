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
//#define  RED_FADE       1
//#define  GREEN_FADE     1
//#define  BLUE_FADE      1

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

    bool     RED_FADE =   0;
    bool     GREEN_FADE = 1;
    bool     BLUE_FADE =  0;
    uint32_t array[NUMLEDS_X][NUMLEDS_Y];
    uint32_t shade;
    int32_t decrement = 0x41;

    for (int j=0; j < NUMLEDS_Y; j++) {
        for (int i=0; i < NUMLEDS_X; i++) {
            array[i][j]=(RED_FADE?0x04ff0000:0)|(GREEN_FADE?0x0200ff00:0)|(BLUE_FADE?0x010000ff:0);
            GUI_SetColor((array[i][j])&0x00ffffff);
        }
    }
    while(1) {
        int ledx = rand()%NUMLEDS_X;
        int ledy = rand()%NUMLEDS_Y;

        if ((array[ledx][ledy])&0x04000000) {  // dim Blue
            shade = ((array[ledx][ledy])>>16)&0xff;
            if (array[ledx][ledy]&0x08000000) shade = (shade + decrement) & 0xfc;
            else shade = (shade - decrement) & 0xfc;
            array[ledx][ledy]=(array[ledx][ledy])&0xff00ffff;
            shade = shade << 16;
            array[ledx][ledy] = shade + array[ledx][ledy];
        }
        if ((array[ledx][ledy])&0x02000000) {  // dim Green
            shade = ((array[ledx][ledy])>>8)&0xff;
            if (array[ledx][ledy]&0x08000000) shade = (shade + decrement) & 0xfc;
            else shade = (shade - decrement) & 0xfc;
            array[ledx][ledy]=(array[ledx][ledy])&0xffff00ff;
            shade = shade << 8;
            array[ledx][ledy] = (uint32_t)shade + (uint32_t)array[ledx][ledy];
        }
        if ((array[ledx][ledy])&0x01000000) {  // dim Red
            shade = (array[ledx][ledy])&0xff;
            if (array[ledx][ledy]&0x08000000) shade = (shade + decrement) & 0xfc;
            else shade = (shade - decrement) & 0xfc;
            array[ledx][ledy]=(array[ledx][ledy])&0xffffff00;
            array[ledx][ledy] = (array[ledx][ledy] + shade);
        }
        

        if ((array[ledx][ledy]&0x00ffffff)==0) {
            if ((array[ledx][ledy]&0x08000000)) array[ledx][ledy] = (array[ledx][ledy]^0x08000000);
            else array[ledx][ledy] = (array[ledx][ledy]^0x08000000)^((rand()%2)?0x04000000:0)^((rand()%2)?0x02000000:0)^((rand()%2)?0x01000000:0);
//            uint32_t currentColour =(RED_FADE?0x00ff0000:0)|(GREEN_FADE?0x0000ff00:0)|(BLUE_FADE?0x000000ff:0);
//            GUI_SetColor(currentColour);
//            GUI_FillCircle(LED_RADIUS+(ledx)*LED_DIAMETER, TITLE_OFFSET+LED_DIAMETER*(ledy), LED_RADIUS);
        }
        else {
             shade = (array[ledx][ledy])&0x00ffffff;
             GUI_SetColor(shade);
             GUI_FillCircle(LED_RADIUS+(ledx)*LED_DIAMETER, TITLE_OFFSET+LED_DIAMETER*(ledy), LED_RADIUS);
             if ((ledx==0)&&(ledy==0))
                printf("%08x\n", array[ledx][ledy]);
             
        }
    }
}



