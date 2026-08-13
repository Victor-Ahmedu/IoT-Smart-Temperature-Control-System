#include <SPI.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//====================================================
// DHT11
//====================================================
#define DHTPIN  D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


//====================================================
// RELAYS
//====================================================

#define HUMIDIFIER_RELAY D0
#define LIGHT_RELAY      D1
#define FAN_RELAY        D6

// Relay module is ACTIVE LOW
//
// LOW  = ON
// HIGH = OFF


//====================================================
// TFT
//====================================================

#define TFT_CS  D8
#define TFT_DC  D2
#define TFT_RST -1

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


//====================================================
// SYSTEM STATES
//====================================================

// true  = Bulb ON / Fan OFF
// false = Bulb OFF / Fan ON

bool bulbState = true;


// Humidifier state
// true  = ON
// false = OFF

bool humidifierState = false;


//====================================================
// SETUP
//====================================================

void setup()
{
  Serial.begin(115200);

  //==================================================
  // DHT
  //==================================================

  dht.begin();


  //==================================================
  // TFT INITIALIZATION
  //==================================================

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);


  //==================================================
  // IMPORTANT
  //==================================================
  //
  // Configure relay pins AFTER TFT initialization.
  //
  // This is especially important for D6 because D6
  // is also the ESP8266 hardware SPI MISO pin.
  //
  // The ST7735 does not need MISO for our display
  // operations, so we can use D6 as the fan output.
  //
  //==================================================

  pinMode(HUMIDIFIER_RELAY, OUTPUT);
  pinMode(LIGHT_RELAY, OUTPUT);
  pinMode(FAN_RELAY, OUTPUT);


  //==================================================
  // INITIAL RELAY STATES
  //==================================================

  // Bulb ON
  digitalWrite(LIGHT_RELAY, LOW);

  // Fan OFF
  digitalWrite(FAN_RELAY, HIGH);

  // Humidifier OFF
  digitalWrite(HUMIDIFIER_RELAY, HIGH);


  //==================================================
  // TFT HEADER
  //==================================================

  tft.fillRect(0, 0, 160, 18, ST77XX_BLUE);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  tft.setCursor(18, 5);
  tft.print("SMART ENVIRONMENT");


  //==================================================
  // TFT STATIC LABELS
  //==================================================

  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(5, 25);
  tft.print("Temp:");

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(5, 40);
  tft.print("Humidity:");

  tft.setTextColor(ST77XX_WHITE);

  tft.setCursor(5, 60);
  tft.print("Humidifier:");

  tft.setCursor(5, 75);
  tft.print("Fan:");

  tft.setCursor(5, 90);
  tft.print("Light:");

  tft.setCursor(5, 110);
  tft.print("Status:");
}


//====================================================
// LOOP
//====================================================

void loop()
{
  //==================================================
  // READ DHT11
  //==================================================

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  //==================================================
  // SENSOR ERROR
  //==================================================

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read DHT11!");

    tft.fillRect(85, 25, 70, 95, ST77XX_BLACK);

    tft.setCursor(85, 25);
    tft.setTextColor(ST77XX_RED);
    tft.print("ERROR");

    delay(2000);

    return;
  }


  //==================================================
  // TEMPERATURE CONTROL
  //==================================================

  /*
      BELOW 30°C
      ----------------
      Bulb ON
      Fan OFF


      ABOVE 33°C
      ----------------
      Bulb OFF
      Fan ON


      30°C - 33°C
      ----------------
      KEEP PREVIOUS STATE
  */

  if (temperature < 30.0)
  {
    bulbState = true;
  }
  else if (temperature > 33.0)
  {
    bulbState = false;
  }


  //==================================================
  // APPLY BULB + FAN STATE
  //==================================================

  if (bulbState)
  {
    // Bulb ON
    digitalWrite(LIGHT_RELAY, LOW);

    // Fan OFF
    digitalWrite(FAN_RELAY, HIGH);
  }
  else
  {
    // Bulb OFF
    digitalWrite(LIGHT_RELAY, HIGH);

    // Fan ON
    digitalWrite(FAN_RELAY, LOW);
  }


  //==================================================
  // HUMIDIFIER CONTROL
  //==================================================

  if (humidity < 50.0)
  {
    humidifierState = true;
  }
  else if (humidity > 80.0)
  {
    humidifierState = false;
  }


  //==================================================
  // APPLY HUMIDIFIER STATE
  //==================================================

  if (humidifierState)
  {
    digitalWrite(HUMIDIFIER_RELAY, LOW);
  }
  else
  {
    digitalWrite(HUMIDIFIER_RELAY, HIGH);
  }


  //==================================================
  // TFT TEMPERATURE
  //==================================================

  tft.fillRect(85, 25, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 25);
  tft.setTextColor(ST77XX_YELLOW);

  tft.print(temperature, 1);
  tft.print(" C");


  //==================================================
  // TFT HUMIDITY
  //==================================================

  tft.fillRect(85, 40, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 40);
  tft.setTextColor(ST77XX_CYAN);

  tft.print(humidity, 1);
  tft.print(" %");


  //==================================================
  // TFT HUMIDIFIER
  //==================================================

  tft.fillRect(85, 60, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 60);

  if (humidifierState)
  {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("ON");
  }
  else
  {
    tft.setTextColor(ST77XX_RED);
    tft.print("OFF");
  }


  //==================================================
  // TFT FAN
  //==================================================

  tft.fillRect(85, 75, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 75);

  if (bulbState)
  {
    // Bulb ON → Fan OFF
    tft.setTextColor(ST77XX_RED);
    tft.print("OFF");
  }
  else
  {
    // Bulb OFF → Fan ON
    tft.setTextColor(ST77XX_GREEN);
    tft.print("ON");
  }


  //==================================================
  // TFT LIGHT
  //==================================================

  tft.fillRect(85, 90, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 90);

  if (bulbState)
  {
    // Bulb ON
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("ON");
  }
  else
  {
    // Bulb OFF
    tft.setTextColor(ST77XX_RED);
    tft.print("OFF");
  }


  //==================================================
  // TFT HUMIDITY STATUS
  //==================================================

  tft.fillRect(85, 110, 70, 10, ST77XX_BLACK);

  tft.setCursor(85, 110);

  if (humidity > 80.0)
  {
    tft.setTextColor(ST77XX_RED);
    tft.print("HIGH HUM");
  }
  else if (humidity > 50.0)
  {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("NORMAL");
  }
  else
  {
    tft.setTextColor(ST77XX_RED);
    tft.print("LOW HUM");
  }


  //==================================================
  // SERIAL MONITOR
  //==================================================

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);

  Serial.print(" C   Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("%   ");

  Serial.print("Humidifier: ");
  Serial.print(humidifierState ? "ON" : "OFF");

  Serial.print("   Fan: ");
  Serial.print(bulbState ? "OFF" : "ON");

  Serial.print("   Bulb: ");
  Serial.println(bulbState ? "ON" : "OFF");


  //==================================================
  // WAIT
  //==================================================

  delay(1000);
}
