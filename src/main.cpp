#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define SLEEP_TIME_SECONDS 900 // 15 minutes
#define ALLOWED_CONNECT_CYCLES 20
#define ALLOWED_READ_CYCLES 40
#define DHT_PIN D4
#define DHT_TYPE DHT22

const String wifiSSID = "";
const String wifiPassword = "";

const IPAddress hostIp(10, 0, 0, 250);
const int hostPort = 5020;

const char* uri = "http://10.0.0.250:5020/v1/datalog/";
const char* sensorId = "s01";

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {

  Serial.begin(9600);

  WiFi.begin(wifiSSID, wifiPassword);
  
  uint8_t counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    counter++;
    if (counter > ALLOWED_CONNECT_CYCLES) {
      ESP.deepSleep(SLEEP_TIME_SECONDS * 1000000);
    }
  }
  
  WiFiClient wifiClient;

  if (wifiClient.connect(hostIp, hostPort)) {
    dht.begin();
    float humidity;
    float temperature;
    
    do {
      humidity = dht.readHumidity();
      temperature = dht.readTemperature();
      counter++;
      if (counter > ALLOWED_READ_CYCLES) {
        ESP.deepSleep(SLEEP_TIME_SECONDS * 1000000);
      }
    } while (temperature == 85.0 || temperature == (-127.0));

    Serial.println("Humidity: " + String(humidity) + ", Temperature: " + String(temperature));

    HTTPClient httpClient;
    httpClient.begin(wifiClient, uri);
    httpClient.addHeader("Content-Type", "application/json");
    int httpResponseCode = httpClient.POST("{\"sensorId\":\"" + String(sensorId) + 
                                            "\",\"temperature\":\"" + String(temperature) + 
                                            "\",\"relativeHumidity\":\"" + String(humidity) + "\"}");

    Serial.println("HTTP Response code: " + String(httpResponseCode));
      
    httpClient.end(); 
  }

  delay(2000);
  ESP.deepSleep(SLEEP_TIME_SECONDS * 1000000);
}

void loop() {
}