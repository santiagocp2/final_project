#include <Arduino.h>
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "GFButton.h"
#include <iostream>
#include <string>


const char* ntpServer = "co.pool.ntp.org";
const long  gmtOffset_sec = -21600;
const int   daylightOffset_sec = 3600;

String GOOGLE_SCRIPT_ID = "AKfycbxFLX_uFlcGxuSDfiTaIjyHwp4GoixVkuq5r4a2P1v-mT_JhAZpnqDQScQj|9mhl8YcP-Q";

const char* ssid = "WCORTAZAR";
const char* password =  "XH1357HSFZ";

float latitude , longitude;
String  latitude_string , longitiude_string;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
WiFiClient espClient;

#define CH5 21
#define led 19

int count = 0;
double lon;
double lat;

char string[15];

GFButton button(5);

String coordinates = "";

void setup() {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(CH5, INPUT);
  pinMode(led , OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);

}

void check_gps(){
  while (SerialGPS.available() > 0) {
      if (gps.encode(SerialGPS.read()))
      {
          latitude = gps.location.lat();
          latitude_string = String(latitude , 6);
          longitude = gps.location.lng();
          longitiude_string = String(longitude , 6);
          Serial.print("Latitude = ");
          Serial.println(latitude_string);
          Serial.print("Longitude = ");
          Serial.println(longitiude_string);
        }
    }
}
void reconnect(){
    check_gps();
    Serial.println(longitiude_string);
    Serial.println(longitiude_string);
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    count = count + 1;
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    String lon_str,lat_str;
    if(latitude_string.length() != 0 && longitiude_string.length() != 0){
      lon_str = longitiude_string;
      lat_str = latitude_string;
    }else{
      lon_str = String(lon);
      lat_str = String(lat);
    }
    if (count%2 != 0 && count > 1){
      coordinates = coordinates +",";
    }
    coordinates = coordinates + "{"+ lon_str +","+lat_str+ "}"; 
    String union_coordinates = "{"+coordinates+"}";
    String geometry = "{'type': 'FeatureCollection','features': [{'type': 'Feature','properties': {},'geometry': {type': 'LineString',coordinates':"+ union_coordinates +"}}]}";
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"lon=" + lon_str + "&lat=" + lat_str+"&name=Muestra_"+String(count)+"&color=green"+"&description=Lugar-donde-se-tomo-la-muestra-"+String(count)+"-en-la-fecha-"+asString;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
}
int ch;
void loop() {
  // ch=pulseIn(CH5, HIGH); 
  // if(ch >= 1900 && ch <= 1930){
  //   digitalWrite(led,HIGH);
  //   reconnect();
  //   digitalWrite(led,LOW);
  // }
  if (button.isPressed()) {
    digitalWrite(led,HIGH);
    reconnect();
    digitalWrite(led,LOW);
  }
  check_gps();
  delay(4000);
}