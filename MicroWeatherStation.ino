/*
  Project:   MicroWeatherStation
  Source:    http://github.com/pa3hcm/microWeatherStation
  Author:    Ernest Neijenhuis PA3HCM
*/


// SSD1306 OLED display setup, using I2C
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_WIDTH  128  // OLED display width, in pixels
#define OLED_HEIGHT 64   // OLED display height, in pixels
#define OLED_RESET  -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_ADDR   0x3C // I2C address
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

// DHT22 temp/RH sensor setup
#include <SimpleDHT.h>
#define DHT22_PIN 3
SimpleDHT22 dht22(DHT22_PIN);


void setup() {
  Serial.begin(9600);
  if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

}


void loop() {
  // Read data from DHT22 sensor
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(2000);
    return;
  }

  // Basic screen layout
  oled.clearDisplay();
  oled.drawRect(0, 8, oled.width(), oled.height() - 8, SSD1306_WHITE);
  oled.setTextSize(1);      // Normal 1:1 pixel scale
  oled.setTextColor(SSD1306_WHITE); // Draw white text
  oled.cp437(true);         // Use full 256 char 'Code Page 437' font
  oled.setCursor(0, 0);
  oled.print(F("Micro Weather Station"));
  oled.display();
  
  // Print temperature
  char tempStr[6];
  dtostrf(temperature, 3, 1, tempStr);
  oled.setCursor(12, 16);
  oled.setTextSize(2);
  oled.print(tempStr);
  oled.print(" ");
  oled.write(248);
  oled.print("C");
  oled.display();  

  // Print relative humidity
  char humStr[6];
  dtostrf(humidity, 3, 1, humStr);
  oled.setCursor(12, 40);
  oled.setTextSize(2);
  oled.print(humStr);
  oled.print(" %RH");

  oled.display();  
  
  // DHT22 sampling rate is 0.5HZ.
  delay(2500);
}
