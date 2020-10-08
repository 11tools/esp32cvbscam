#include <Wire.h>
#include "twi.h"
#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "I2SCamera.h"
#include <Adafruit_ILI9341.h>       // Hardware-specific library



const int SIOD = 26; //SDA
const int SIOC = 27; //SCL

const int VSYNC = 25;
const int HREF = 23;

const int PCLK = 22;
const int XCLK = 0;

const int D0 = 5;
const int D1 = 18;
const int D2 = 19;
const int D3 = 21;
const int D4 = 36;
const int D5 = 39;
const int D6 = 34;
const int D7 = 35;

#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   16  // Chip select control pin
#define TFT_DC   15  // Data Command control pin
#define TFT_RST   2 

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);

unsigned char cmd5150[][2] = 
{

  {0x00, 0x00},

  {0x03, 0x0d},
 
  {0x0D, 0x40}, 
  
};

void init_tvp5150()
{
  int a;
  for( a = 0 ; a < sizeof(cmd5150)/2;a++)
  {
    twi_device_write(0XB8,cmd5150[a][0],cmd5150[a][1]);
  }
}


void read_id(){

  Serial.print("read tvp5150 id =");
  uint8_t id = twi_device_read(0xB9,0x80);
  Serial.println(id,HEX);
  Serial.print("\n");

}


void read_vsync(){
  Serial.print("Waiting for VSYNC...");  
  pinMode(VSYNC, INPUT);
  while(!digitalRead(VSYNC));
  while(digitalRead(VSYNC));
  Serial.print(" done");
}



void init_tvinput(){
  int xres = 160;
  int yres = 120;
  I2SCamera::init(xres, yres, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
}


void init_tft(){
  tft.begin();
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);   
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setTextSize(1);
  tft.println("Hello World!");
  
  Serial.print("init esp lcd \n");


}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("This is esp32 twi demo \n");


  //twi_init(14,15);
  twi_init(26,27);
  Serial.print("init tvp5150 use 26 27 \n");
  init_tvp5150();
  init_tvinput();
  init_tft();
  I2SCamera::i2sRun();

  

}


void dumpaframe(unsigned char * frame, int xres, int yres)
{
  int i = 0;
  for(int x = 0; x < xres; x++)
    for(int y = 0; y < yres; y++)
    {
      i = y * xres + x;
      unsigned char c = frame[i];
      Serial.print(" ");
      Serial.println(c,HEX);
      Serial.print(" ");
    }  
}



void loop() {
  // put your main code here, to run repeatedly:
  unsigned char addr =twi_addr_probe();
  Serial.print("get tvp5150 address = 0x");
  Serial.println(addr,HEX);
  Serial.print("\n");
  delay(2000);
  //read_vsync();


}
