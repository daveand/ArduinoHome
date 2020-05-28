#include "DHT.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

#define DHTPIN 2
#define DHTTYPE DHT11

char ssid[] = ROUTER_SSID;        // your network SSID (name)
char pass[] = PASSWORD;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char serverAddress[] = "172.104.136.87";  // server address
int port = 5001;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  dht.begin();
  connectToWifi(ssid, pass);
}

void loop() {

  const char* dateTime = getApiStatus(client);
  String dateTimeString = String(dateTime);

  if (dateTime != "error") {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if(isnan(humidity) || isnan(temperature)) {
      Serial.print("Failed to read sensor");
    }

    float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

    Serial.print("\n");
    Serial.print("Sensor data:\n");
    Serial.print(temperature);
    Serial.print(" grC\n");
    Serial.print(humidity);
    Serial.print(" %\n");
    Serial.print(heatIndex);
    Serial.print("\n\n");

    StaticJsonDocument<200> doc;
    doc["temp"] = temperature;
    doc["humidity"] = humidity;
    doc["heatIndex"] = heatIndex;
    doc["logTime"] = dateTimeString;
    doc["loggedBy"] = "Arduino MKR Wifi1010";

    //serializeJsonPretty(doc, Serial);
    postDataToApi(client, doc);
    delay(3600000);
  }
  


  
}
