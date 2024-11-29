#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <reloc.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <esede.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <MQTT.h>

#define DHTPIN 13       // Pin al que está conectado el DHT11
#define DHTTYPE DHT11
#define TFT_CS    5     // Pin Chip Select
#define TFT_RST   4     // Pin Reset
#define TFT_DC    2     // Pin Data/Command

// Crea el objeto de la pantalla ST7789
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
DS1307_RTC RTC;
MicroSD MSD;
Adafruit_BMP280 bmp;
DHT dht(DHTPIN, DHTTYPE);
String temp;
MQTT mqtt;

JsonDocument doc;
String json;

int minuto;
int contador = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("");
  Serial.print(F("Iniciando reloj... "));
  RTC.RTC_init(__DATE__, __TIME__);
  //RTC.RTC_init();
  Serial.println(F("Terminado"));
  Serial.print(F("Iniciando microSD... "));
  MSD.MicroSD_init();
  Serial.println(F("Terminado"));

  Serial.print(F("Iniciando sensor... "));
  if (!bmp.begin(0x76))
  {  // Cambia a 0x77 si tu sensor usa esa dirección
    while (1);  // Detiene el programa si el sensor no es detectado
  }
  Serial.println(F("Terminado"));
  tft.init(240, 240);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(3);
  tft.setCursor(168, 4);
  tft.print("ESLI");

  tft.setTextSize(1);
  tft.setCursor(168, 30);
  tft.print("Laboratorios");

  tft.fillRect(0, 90, 238, 2, ST77XX_BLUE);

  tft.setCursor(24, 224);
  tft.print("Carlos Sebastian Herrera Delgado");

  RTC.Time();

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 24);
  tft.print(RTC.fecha2);

  tft.setTextSize(5);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 46);
  tft.print(RTC.hora);

  minuto = RTC.min;
  
  doc["hora"] = RTC.hora;
  doc["fecha"] = RTC.fecha2;
  doc["temperatura"] = 0;
  doc["presion"] = 0;
  doc["altitud"] = 0;
  doc["humedad"] = 0;
  doc["mq"] = 0;
  serializeJson(doc, json);
  MSD.writeFile(json);

  Serial.print(F("Iniciando MQTT... "));
  mqtt.setup_WiFi ( );
  mqtt.set_MQTT_server ( );
  mqtt.set_MQTT_callback (  );
  Serial.println(F("Terminado"));
}

void loop()
{
  RTC.Time();
  float temperatura = bmp.readTemperature();      // Temperatura en grados Celsius
  float presion = bmp.readPressure() / 100.0F;    // Presión en hPa
  float altitud = bmp.readAltitude(1013.25);
  float voltaje = (analogRead(34) / 4095.0) * 330;
  float humedad = dht.readHumidity();
  doc["temperatura"] = temperatura;
  doc["presion"] = presion;
  doc["altitud"] = altitud;
  doc["humedad"] = humedad;
  doc["mq"] = voltaje;
  if (minuto != RTC.min)
  {
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0, 24);
    tft.fillRect(0, 24, 96, 16, ST77XX_BLACK);
    tft.print(RTC.fecha2);

    tft.setTextSize(5);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(0, 46);
    tft.fillRect(0, 46, 240, 40, ST77XX_BLACK);
    tft.print(RTC.hora);
    minuto = RTC.min;
  }

  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  
  tft.setCursor(0, 100);
  tft.fillRect(0, 100, 240, 16, ST77XX_BLACK);
  temp = "Temperatura: " + (String)temperatura + " C";
  tft.print(temp);

  tft.setCursor(0, 120);
  tft.fillRect(0, 120, 240, 16, ST77XX_BLACK);
  temp = "Presion: " + (String)presion + " hPa";
  tft.print(temp);

  tft.setCursor(0, 140);
  tft.fillRect(0, 140, 240, 16, ST77XX_BLACK);
  temp = "Altitud: " + (String)altitud + " m";
  tft.print(temp);

  tft.setCursor(0, 180);
  tft.fillRect(0, 180, 240, 16, ST77XX_BLACK);
  temp = "Humedad: " + (String)humedad + "%";
  tft.print(temp);

  tft.setCursor(0, 160);
  tft.fillRect(0, 160, 240, 16, ST77XX_BLACK);
  temp = "MQ: " + (String)voltaje + "%";
  tft.print(temp);

  contador++;
  if (contador == 5)
  {
    doc["hora"] = RTC.hora;
    doc["fecha"] = RTC.fecha2;
    serializeJson(doc, json);
    if (!mqtt.conectado())
    {
      mqtt.reconnect_MQTT();
    }
    mqtt.publish_MQTT(json);
    MSD.writeFile(json);
    contador = 0;
  }
  delay(400);
}
