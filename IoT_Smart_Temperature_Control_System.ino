/*
 * ============================================================
 * Project : IoT Smart Temperature Control System
 * Version : 1.0.0
 * Author  : Victor Ahmedu
 * Board   : NodeMCU ESP8266 (ESP-12E)
 * IDE     : Arduino IDE
 *
 * Description
 * ------------------------------------------------------------
 * This project monitors ambient temperature and humidity using
 * a DHT11 sensor and automatically controls a humidifier,
 * ventilation fan and light through relay modules.
 *
 * A 1.8-inch ST7735 TFT display provides real-time sensor
 * readings and system status.
 *
 * Features
 * ------------------------------------------------------------
 * • Temperature Monitoring
 * • Humidity Monitoring
 * • Automatic Light Control
 * • Automatic Fan Control
 * • Automatic Humidifier Control
 * • TFT Display Interface
 * • Serial Monitoring
 * • Temperature Hysteresis Control
 *
 * ============================================================
 */

#include <SPI.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>



//==============================================================
// PIN DEFINITIONS
//==============================================================

// DHT11
#define DHTPIN D4
#define DHTTYPE DHT11

// Relay Pins (Active LOW)
#define HUMIDIFIER_RELAY D0
#define FAN_RELAY         D6
#define LIGHT_RELAY       D1

// TFT Display
#define TFT_CS   D8
#define TFT_DC   D2
#define TFT_RST  -1



//==============================================================
// CONTROL PARAMETERS
//==============================================================

// Temperature limits

const float TEMP_LOW  = 28.0;
const float TEMP_HIGH = 32.0;

// Humidity limit

const float HUMIDITY_THRESHOLD = 20.0;



//==============================================================
// OBJECTS
//==============================================================

DHT dht(DHTPIN, DHTTYPE);

Adafruit_ST7735 tft =
Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);



//==============================================================
// GLOBAL VARIABLES
//==============================================================

float temperature = 0;
float humidity = 0;

bool bulbState = false;
bool fanState = false;
bool humidifierState = false;



//==============================================================
// FUNCTION PROTOTYPES
//==============================================================

void initializeDisplay();

void readSensors();

void controlRelays();

void updateDisplay();

void printSerialStatus();

void updateText(
    int x,
    int y,
    String text,
    uint16_t color
);



//==============================================================
// SETUP
//==============================================================

void setup()
{
    Serial.begin(115200);

    dht.begin();

    pinMode(HUMIDIFIER_RELAY, OUTPUT);
    pinMode(FAN_RELAY, OUTPUT);
    pinMode(LIGHT_RELAY, OUTPUT);

    // Turn OFF all relays (Active LOW)

    digitalWrite(HUMIDIFIER_RELAY, HIGH);
    digitalWrite(FAN_RELAY, HIGH);
    digitalWrite(LIGHT_RELAY, HIGH);

    initializeDisplay();

    Serial.println();
    Serial.println("=========================================");
    Serial.println(" IoT Smart Temperature Control System");
    Serial.println(" System Started Successfully");
    Serial.println("=========================================");
}



//==============================================================
// TFT INITIALIZATION
//==============================================================

void initializeDisplay()
{
    tft.initR(INITR_BLACKTAB);

    tft.setRotation(1);

    tft.fillScreen(ST77XX_BLACK);

    // Header

    tft.fillRect(0,0,160,18,ST77XX_BLUE);

    tft.setTextColor(ST77XX_WHITE);

    tft.setTextSize(1);

    tft.setCursor(15,5);

    tft.print("SMART ENVIRONMENT");



    // Labels

    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(5,25);
    tft.print("Temp:");

    tft.setTextColor(ST77XX_CYAN);
    tft.setCursor(5,40);
    tft.print("Humidity:");

    tft.setTextColor(ST77XX_WHITE);

    tft.setCursor(5,60);
    tft.print("Humidifier:");

    tft.setCursor(5,75);
    tft.print("Fan:");

    tft.setCursor(5,90);
    tft.print("Light:");

    tft.setCursor(5,110);
    tft.print("Status:");
}


//==============================================================
// DISPLAY HELPER
//==============================================================

void updateText(
    int x,
    int y,
    String text,
    uint16_t color
)
{
    tft.fillRect(x,y,70,10,ST77XX_BLACK);

    tft.setCursor(x,y);

    tft.setTextColor(color);

    tft.print(text);
}


/*
 * IoT Smart Temperature Control System
 * Version 1.0.0
 */
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define DHTPIN D4
#define DHTTYPE DHT11

#define HUMIDIFIER_RELAY D0
#define FAN_RELAY D6
#define LIGHT_RELAY D1

#define TFT_CS D8
#define TFT_DC D2
#define TFT_RST -1

const float TEMP_LOW=28.0;
const float TEMP_HIGH=32.0;
const float HUMIDITY_THRESHOLD=20.0;

DHT dht(DHTPIN,DHTTYPE);
Adafruit_ST7735 tft(TFT_CS,TFT_DC,TFT_RST);

float temperature=0, humidity=0;
bool bulbState=false, fanState=false, humidifierState=false;

void updateText(int x,int y,String txt,uint16_t color){
  tft.fillRect(x,y,70,10,ST77XX_BLACK);
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.print(txt);
}

void initializeDisplay(){
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.fillRect(0,0,160,18,ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(15,5); tft.print("SMART ENVIRONMENT");
  tft.setTextColor(ST77XX_YELLOW); tft.setCursor(5,25); tft.print("Temp:");
  tft.setTextColor(ST77XX_CYAN); tft.setCursor(5,40); tft.print("Humidity:");
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5,60); tft.print("Humidifier:");
  tft.setCursor(5,75); tft.print("Fan:");
  tft.setCursor(5,90); tft.print("Light:");
  tft.setCursor(5,110); tft.print("Status:");
}

void readSensors(){
  temperature=dht.readTemperature();
  humidity=dht.readHumidity();
}

void controlRelays(){
  if(temperature<TEMP_LOW) bulbState=true;
  else if(temperature>TEMP_HIGH) bulbState=false;

  fanState=!bulbState;
  humidifierState=(humidity<HUMIDITY_THRESHOLD);

  digitalWrite(LIGHT_RELAY, bulbState?LOW:HIGH);
  digitalWrite(FAN_RELAY, fanState?LOW:HIGH);
  digitalWrite(HUMIDIFIER_RELAY, humidifierState?LOW:HIGH);
}

void updateDisplay(){
  updateText(85,25,String(temperature,1)+" C",ST77XX_YELLOW);
  updateText(85,40,String(humidity,1)+" %",ST77XX_CYAN);
  updateText(85,60,humidifierState?"ON":"OFF",humidifierState?ST77XX_GREEN:ST77XX_RED);
  updateText(85,75,fanState?"ON":"OFF",fanState?ST77XX_GREEN:ST77XX_RED);
  updateText(85,90,bulbState?"ON":"OFF",bulbState?ST77XX_YELLOW:ST77XX_RED);
  updateText(85,110,humidifierState?"LOW HUM":"NORMAL",humidifierState?ST77XX_RED:ST77XX_GREEN);
}

void printSerialStatus(){
  Serial.print("Temperature: ");Serial.print(temperature);Serial.print(" C  ");
  Serial.print("Humidity: ");Serial.print(humidity);Serial.println(" %");
  Serial.print("Light: ");Serial.println(bulbState?"ON":"OFF");
  Serial.print("Fan: ");Serial.println(fanState?"ON":"OFF");
  Serial.print("Humidifier: ");Serial.println(humidifierState?"ON":"OFF");
  Serial.println("---------------------");
}

void setup(){
  Serial.begin(115200);
  dht.begin();
  pinMode(HUMIDIFIER_RELAY,OUTPUT);
  pinMode(FAN_RELAY,OUTPUT);
  pinMode(LIGHT_RELAY,OUTPUT);
  digitalWrite(HUMIDIFIER_RELAY,HIGH);
  digitalWrite(FAN_RELAY,HIGH);
  digitalWrite(LIGHT_RELAY,HIGH);
  initializeDisplay();
}

void loop(){
  readSensors();
  if(isnan(temperature)||isnan(humidity)){
    updateText(85,25,"ERROR",ST77XX_RED);
    delay(2000);
    return;
  }
  controlRelays();
  updateDisplay();
  printSerialStatus();
  delay(1000);
}
