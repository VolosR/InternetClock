#include <TFT_eSPI.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include "bigFont.h"
#include "middleFont.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

ESP32Time rtc(0); 

//colours
unsigned short back=TFT_WHITE;
unsigned short darkG=0x2945;
unsigned short lightG=0xBDF7;
unsigned short red=0x9820;
unsigned short segCol[4]={darkG,darkG,red,red};

//time zone  and wifi
int zone=2;
const char* ntpServer = "pool.ntp.org";          
String ssid = "xxx";
String password = "xxxxxxx";

// digits in clock
String digits[4]={"0","2","1","3"};
int n=0;  // counter for seconds

void connectWifi()
{ 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Can't connect to wifi!");
  }
}

void setTime()
{
  configTime(3600*zone, 0, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
}

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_RED);
  sprite.createSprite(320,170);

    connectWifi();
    setTime();

     //brightness
     ledcSetup(0, 10000, 8);
     ledcAttachPin(38, 0);
     ledcWrite(0, 120);
}

void draw()
  {
    sprite.fillSprite(TFT_WHITE); 
    sprite.loadFont(bigFont);
    sprite.setTextDatum(4);
    
    for(int i=0;i<4;i++){
    sprite.fillSmoothRoundRect(21+(i*70),37,68,100,10,segCol[i],back);
    sprite.fillRect(21+(i*70),86,68,2,lightG);
    sprite.setTextColor(back,segCol[i]);
    sprite.drawString(digits[i],21+(i*70)+33,90);
    sprite.drawWedgeLine(21+(i*70)+33,144,21+(i*70)+33,152,2,2,lightG,back);
    if(i%2==0)
    sprite.drawWedgeLine(21+(i*70)+33,152,21+(i*70)+42,152,2,2,lightG,back);
    else
    sprite.drawWedgeLine(21+(i*70)+33,152,21+(i*70)+24,152,2,2,lightG,back);
    }

    for(int i=0;i<56;i++)
    sprite.fillRect(21+(i*5),26,3,3,darkG);

    sprite.unloadFont();
    sprite.loadFont(middleFont);
    sprite.setTextColor(darkG,back);
    sprite.drawString("hrs",90,152);
    sprite.drawString("min",230,152);

    sprite.pushSprite(0,0);
  }

void loop() {

   digits[0]=rtc.getTime().charAt(0); 
   digits[1]=rtc.getTime().charAt(1); 
   digits[2]=rtc.getTime().charAt(3); 
   digits[3]=rtc.getTime().charAt(4); 
  
  draw();
  delay(1000);

  n++;
  if(n==6800)
  {
    n=0;
    setTime();
  }
}
