/**
   The MIT License (MIT)

   Copyright (c) 2018 by ThingPulse, Daniel Eichhorn
   Copyright (c) 2018 by Fabrice Weinberg

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   ThingPulse invests considerable time and money to develop these open source libraries.
   Please support us by buying our products (and not the clones) from
   https://thingpulse.com

*/
#include <XYMjoystickOut.h>

XYMjoystickOut st1(27, 26, 25); // 实例化joystick摇杆对象1，定义x轴、y轴、中间按键引脚
XYMjoystickOut st2(4, 2, 15);  // 实例化joystick摇杆对象2，定义x轴、y轴、中间按键引脚

// Include the correct display library

// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
// OR #include "SH1106Wire.h"   // legacy: #include "SH1106.h"

// For a connection via I2C using brzo_i2c (must be installed) include:
// #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Brzo.h"
// OR #include "SH1106Brzo.h"

// For a connection via SPI include:
// #include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// OR #include "SH1106SPi.h"


// Optionally include custom images
#include "image.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
char *pin = "1234"; //建立连接时的密码，不设置与hc05连接不上
uint8_t pin_len = 4;

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
// SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.
// SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);  // ADDRESS, SDA, SCL, OLEDDISPLAY_GEOMETRY  -  Extra param required for 128x32 displays.
// SH1106Wire display(0x3c, SDA, SCL);     // ADDRESS, SDA, SCL

// Initialize the OLED display using brzo_i2c:
// SSD1306Brzo display(0x3c, D3, D5);  // ADDRESS, SDA, SCL
// or
// SH1106Brzo display(0x3c, D3, D5);   // ADDRESS, SDA, SCL

// Initialize the OLED display using SPI:
// D5 -> CLK
// D7 -> MOSI (DOUT)
// D0 -> RES
// D2 -> DC
// D8 -> CS
// SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
// or
// SH1106Spi display(D0, D2);       // RES, DC


#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;
char data[100]="init";
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  SerialBT.setPin(pin,pin_len);    
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  st1.setxyTH(700);     // 设置st1摇杆作为方向按键使用的触发偏离阈值
  st2.setxyTH(700);    // 设置st2摇杆作为方向按键使用的触发偏离阈值
  Serial.print("<==手柄初始化完成==>\n请两个摇杆同时向上推使设备启用\nwaiting...\n");
  while (!st1.upbtn || !st2.upbtn)
  {
    st1.data_update(); // 获取st1摇杆的实时状态
    st2.data_update(); // 获取st2摇杆的实时状态
  }
  Serial.print("设备启用...\n");


}

void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Hello world");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128,
                             "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }
  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i = 1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i * 3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i * 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void drawImageDemo() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  display.clear();
  st1.data_update(); // 获取st1摇杆的实时状态
  st2.data_update(); // 获取st2摇杆的实时状态

 /*输出st1摇杆的x、y、中键原始值*/
  // Serial.print("st2.x: ");
  // Serial.print(st2.xval);
  // Serial.print("\tst2.y: ");
  // Serial.print(st2.yval);
  //  Serial.print("\tst2.middle: ");
  // Serial.println(st2.mval);

  /*输出st1摇杆作为方向键的实时状态*/
   if (st1.upbtn)
     {Serial.println("st1.upbtn");
  SerialBT.println("st1.upbtn");
     }if (st1.dwbtn)
  {   Serial.println("st1.dwbtn");
   SerialBT.println("st1.dwbtn");
 }if (st1.lfbtn)
     {Serial.println("st1.lfbtn");
   SerialBT.println("st1.lfbtn");
     }if (st1.rgbtn)
     {Serial.println("st1.rgbtn");
   SerialBT.println("st1.rgbtn");
     }if (st1.mbtn)
     {Serial.println("st1.mbtn");
   SerialBT.println("st1.mbtn");
     }

     if (st2.upbtn)
     {Serial.println("st2.upbtn");
  SerialBT.println("st2.upbtn");
     }
     if (st2.dwbtn)
  {   Serial.println("st2.dwbtn");
   SerialBT.println("st2.dwbtn");
 }
 if (st2.lfbtn)
     {Serial.println("st2.lfbtn");
   SerialBT.println("st2.lfbtn");
     }
     if (st2.rgbtn)
     {Serial.println("st2.rgbtn");
   SerialBT.println("st2.rgbtn");
     }
     if (st2.mbtn)
     {Serial.println("st2.mbtn");
   SerialBT.println("st2.mbtn");
     }


  /*输出st1摇杆作为按键使用的按下情况*/
 if (st1.upbtn_press)
   Serial.println("st1.upbtn_press");
 if (st1.dwbtn_press)
    Serial.println("st1.dwbtn_press");
 if (st1.lfbtn_press)
   Serial.println("st1.lfbtn_press");
  if (st1.rgbtn_press)
    Serial.println("st1.rgbtn_press");
  if (st1.mbtn_press)
    Serial.println("st1.mbtn_press");

  /*输出st1摇杆作为按键使用的释放情况*/
 if (st1.upbtn_release)
   Serial.println("st1.upbtn_release");
 if (st1.dwbtn_release)
    Serial.println("st1.dwbtn_release");
 
  if (st1.lfbtn_release)
 
    Serial.println("st1.lfbtn_release");
 
  if (st1.rgbtn_release)
 
    Serial.println("st1.rgbtn_release");
 
  if (st1.mbtn_release)
 
    Serial.println("st1.mbtn_release");
 
  //draw the current demo method
 // demos[demoMode]();
    display.setColor(WHITE);
    display.drawString(2,2,"hello");
    // 每个方向一行
  if (st1.upbtn)
    display.drawString(10, 18, "up");
  if (st1.dwbtn)
    display.drawString(10, 24, "dw");
  if (st1.lfbtn)
    display.drawString(5, 24, "lf");
  if (st1.rgbtn)
    display.drawString(15, 24, "rg");

  if (st2.upbtn)
    display.drawString(55, 18, "up");
  if (st2.dwbtn)
    display.drawString(55, 24, "dw");
  if (st2.lfbtn)
    display.drawString(50, 24, "lf");
  if (st2.rgbtn)
    display.drawString(60, 24, "rg");
 // display.setFont(ArialMT_Plain_10);
  //display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 54, String(millis()));
  // write the buffer to the display
  display.display();

 // if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
 //   demoMode = (demoMode + 1)  % demoLength;
 //   timeSinceLastModeSwitch = millis();
 // }
  //counter++;
  if (SerialBT.available()) {
    
    Serial.write(SerialBT.read());
    
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  
  delay(10);
}
