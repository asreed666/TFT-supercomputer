/* Hello World! for the Emwin TFT Library */

#include <cstring>
#define MQTTClient_QOS2 1

#include <cstdint>
#include "mbed.h"
#include "GUI.h"
#include "cy8ckit_028_tft.h"
#include <MQTTClientMbedOs.h>

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

using ThisThread::sleep_for;

void Display_Init(void) {
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
#define LEDON 0
#define LEDOFF 1
#include "thing_name.h"

WiFiInterface *wifi;

AnalogIn lightLevel(P10_0);
DigitalOut statusLed(LED1);
DigitalOut rxLed(LED2);
DigitalOut lightsR(LED3);
DigitalOut lightsG(LED4);
DigitalOut lightsB(LED5);
uint32_t rxCount = 0;
uint32_t lthresh = 50;

const char *sec2str(nsapi_security_t sec) {
  switch (sec) {
  case NSAPI_SECURITY_NONE:
    return "None";
  case NSAPI_SECURITY_WEP:
    return "WEP";
  case NSAPI_SECURITY_WPA:
    return "WPA";
  case NSAPI_SECURITY_WPA2:
    return "WPA2";
  case NSAPI_SECURITY_WPA_WPA2:
    return "WPA/WPA2";
  case NSAPI_SECURITY_UNKNOWN:
  default:
    return "Unknown";
  }
}
void messageArrived(MQTT::MessageData &md) {
  MQTT::Message &message = md.message;
  uint32_t len = md.message.payloadlen;
  //(&md.message.payload)[len-1] = 0;
  //int i = 0;
  char rxed[len+1];
  strncpy(&rxed[0], (char *) (&md.message.payload)[0], len);
  lthresh = atoi(rxed);
  rxCount++;
  rxLed = !rxLed;
}
int scan_demo(WiFiInterface *wifi) {
  WiFiAccessPoint *ap;

  printf("Scan:\n");

  int count = wifi->scan(NULL, 0);

  if (count <= 0) {
    printf("scan() failed with return value: %d\n", count);
    return 0;
  }

  /* Limit number of network arbitrary to 15 */
  count = count < 15 ? count : 15;

  ap = new WiFiAccessPoint[count];
  count = wifi->scan(ap, count);

  if (count <= 0) {
    printf("scan() failed with return value: %d\n", count);
    return 0;
  }

  for (int i = 0; i < count; i++) {
    printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: "
           "%hhd Ch: %hhd\n",
           ap[i].get_ssid(), sec2str(ap[i].get_security()),
           ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
           ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
           ap[i].get_rssi(), ap[i].get_channel());
  }
  printf("%d networks available.\n", count);

  delete[] ap;
  return count;
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
    int counter = 0;

    while(counter++ < 100) {
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
    char buffer[80];
  uint32_t rc;
  lightsR = LEDOFF;
  lightsG = LEDOFF;
  lightsB = LEDOFF;
  printf("WiFi-MQTT example\n");
  /* Initialise display */
  GUI_Init();
  GUI_Clear();
  GUI_SetFont(GUI_FONT_20B_1);
  GUI_DispStringAt("Telemetry Data", 0, 0);
  GUI_SetFont(GUI_FONT_16B_1);



#ifdef MBED_MAJOR_VERSION
  printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION,
         MBED_PATCH_VERSION);
#endif

  WiFiInterface *wifi = WiFiInterface::get_default_instance();
  if (!wifi) {
    printf("ERROR: No WiFiInterface found.\n");
    return -1;
  }

  int count = scan_demo(wifi);
  if (count == 0) {
    printf("No WIFI APs found - can't continue further.\n");
    return -1;
  }

  printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
  int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                          NSAPI_SECURITY_WPA_WPA2);
  if (ret != 0) {
    printf("\nConnection error: %d\n", ret);
    return -1;
  }

  printf("Success\n\n");
  printf("MAC: %s\n", wifi->get_mac_address());
  printf("IP: %s\n", wifi->get_ip_address());
  printf("Netmask: %s\n", wifi->get_netmask());
  printf("Gateway: %s\n", wifi->get_gateway());
  printf("RSSI: %d\n\n", wifi->get_rssi());

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  data.clientID.cstring = (char *)THING_NAME;
  data.keepAliveInterval = 20;
  data.cleansession = 1;
  data.username.cstring = (char *)"";
  data.password.cstring = (char *)"";
  //    char *host = "10.0.0.2";
  char *host = (char *)MQTT_BROKER;
  uint16_t port = 1883;
  TCPSocket socket;
  MQTTClient client(&socket);
  socket.open(wifi);
  rc = socket.connect(host, port);
  if (rc == 0) {
    printf("Succesful connection of socket to Host %s port %d\n", host, port);
    sprintf(buffer,"Succesful connection of socket to Host %s port %d\n", host, port);
    GUI_DispStringAt(buffer, 0, 40);
  } else {
    printf("Socket connection failed");
  }
  rc = client.connect(data);
  if (rc == 0) {
    printf("Succesful connection of %s to Broker\n", data.clientID.cstring);
    sprintf(buffer, "Succesful connection of %s to Broker\n", data.clientID.cstring);
    GUI_DispStringAt(buffer, 0, 60);
 } else {
    printf("Client connection failed");
  }
  MQTT::Message message{};
  sprintf(buffer, "Hello World! from %s\r\n", THING_NAME);
  message.qos = MQTT::QOS0;
  message.retained = false;
  message.dup = false;
  message.payload = (void *)buffer;
  message.payloadlen = strlen(buffer) + 1;

  rc = client.publish(ANNOUNCE_TOPIC, message);
  if (rc == 0) {
    printf("publish announce worked\n");
    GUI_DispStringAt("publish announce worked\n", 0, 80);
  }
  else {
    printf("publish announce failed %d\n", rc);
  }
  message.payloadlen = 80;
  rc = client.subscribe((char *)LIGHT_THRESH_TOPIC, MQTT::QOS0, messageArrived);
  if (rc != 0)
    printf("Subscription Error %d\n", rc);
  else
    printf("Subscribed to %s\n", LIGHT_THRESH_TOPIC);
  sprintf(buffer, "%d", lthresh);
  message.payload = (void *)buffer;
  message.payloadlen = strlen(buffer) + 1;
  rc = client.publish(LIGHT_THRESH_TOPIC, message);
  if (rc == 0){
    printf("publish light threshold  worked\n");
  }
  else {
    printf("publish light threshold failed %d\n", rc);
  }
  uint32_t lastRxCount = 0;
//     GUI_Init();

    /* Set font size, foreground and background Colours */
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);

    /* Clear screen and print splash screen */
    sleep_for(1000);
    GUI_Clear();
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("CITY3032 MQTT Example", 160, 0);
  while (1) {
    //ThisThread::sleep_for(100);
    statusLed = !statusLed;
    uint16_t lightPercent = lightLevel.read_u16() * 100 / 65535;
    sprintf(buffer, "%d", lightPercent);
    //printf("Hello %d\n", lightPercent);
    GUI_DispStringAt(buffer, 260, 20);
    message.payload = (void *)buffer;
    message.payloadlen = strlen(buffer) + 1;

    //        if (lightDisplay != lastLightDisplay) {
    rc=client.publish(LIGHT_LEVEL_TOPIC, message);

    if (lightPercent < (lthresh-5)) {
        lightsR = LEDON;
        lightsG = LEDOFF;
        lightsB = LEDOFF;
        printf("Light level %d is below the threshold level\n", lightPercent);
    } else if (lightPercent > (lthresh + 5)) {
        lightsR = LEDOFF;
        lightsG = LEDOFF;
        lightsB = LEDON;
        printf("Light level %d is below the threshold level\n", lightPercent);
    } else {
        lightsR = LEDOFF;
        lightsG = LEDON;
        lightsB = LEDOFF;
    }
    if (rc!=0) printf("publish failed\n");
    if (rxCount != lastRxCount) {
      printf("Current subscribed message received count = %d\n", rxCount);
      sprintf(buffer, "Current subscribed message received count = %d\n", rxCount);
      GUI_DispStringAt(buffer, 0, 100);
      printf("lthresh is now = %d\n", lthresh);
      sprintf(buffer, "lthresh is now = %d, ambient light level =", lthresh);
      GUI_DispStringAt(buffer, 0, 20);

      lastRxCount = rxCount;
    }

    client.yield(1000);
  }
  wifi->disconnect();

  printf("\nDone\n");

}