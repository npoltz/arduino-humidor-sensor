#include <ESP8266WiFi.h>

#define LED LED_BUILTIN

//SSID and Password of your WiFi router
const char* ssid = "";
const char* password = "";

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED,HIGH);
    delay(200);
    Serial.print(".");
    digitalWrite(LED,LOW);
    delay(200);
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  digitalWrite(LED, LOW);
  delay(2000);
  digitalWrite(LED, HIGH);
  delay(500);
}