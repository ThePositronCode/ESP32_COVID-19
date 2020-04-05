#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h> 
#include "SSD1306Wire.h"
#include <TimeLib.h>

SSD1306Wire  display(0x3c, 5, 4);
const String url="https://services1.arcgis.com/0MSEUqKaxRlEPj5g/arcgis/rest/services/ncov_cases/FeatureServer/2/query?where=&objectIds=7&time=&geometry=&geometryType=esriGeometryEnvelope&inSR=&spatialRel=esriSpatialRelIntersects&resultType=none&distance=0.0&units=esriSRUnit_Meter&returnGeodetic=false&outFields=*&returnGeometry=true&featureEncoding=esriDefault&multipatchOption=xyFootprint&maxAllowableOffset=&geometryPrecision=&outSR=&datumTransformation=&applyVCSProjection=false&returnIdsOnly=false&returnUniqueIdsOnly=false&returnCountOnly=false&returnExtentOnly=false&returnQueryGeometry=false&returnDistinctValues=false&cacheHint=false&orderByFields=&groupByFieldsForStatistics=&outStatistics=&having=&resultOffset=&resultRecordCount=&returnZ=false&returnM=false&returnExceededLimitFeatures=true&quantizationParameters=&sqlFormat=none&f=pjson&token=";

void setup() {
  Serial.begin(115200);
  display.init();
  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
}

void loop(){
  display.clear();
  display.display();
  
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println("Response code: " + httpCode);

  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(9) + 4*JSON_OBJECT_SIZE(2) + 7*JSON_OBJECT_SIZE(6) + 3*JSON_OBJECT_SIZE(7) + JSON_OBJECT_SIZE(9) + 1280;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, payload);

  JsonObject features_0_attributes = doc["features"][0]["attributes"];
  String features_0_attributes_Country_Region = features_0_attributes["Country_Region"];   
  long features_0_attributes_Confirmed = features_0_attributes["Confirmed"]; // 45209
  int features_0_attributes_Deaths = features_0_attributes["Deaths"]; // 3032
  int features_0_attributes_Recovered = features_0_attributes["Recovered"]; // 7991


  Serial.println(payload);
  Serial.print("Country: ");
  Serial.println(features_0_attributes_Country_Region);
  Serial.print("Confirmed: ");
  Serial.println(features_0_attributes_Confirmed);
  Serial.print("Deaths: ");
  Serial.println(features_0_attributes_Deaths);
  Serial.print("Recovered: ");
  Serial.println(features_0_attributes_Recovered);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Country: " + features_0_attributes_Country_Region);
  display.drawString(0, 10, "Confirmed: " + String(features_0_attributes_Confirmed));
  display.drawString(0, 20, "Deaths: " + String(features_0_attributes_Deaths));
  display.drawString(0, 30, "Recovered: " + String(features_0_attributes_Recovered));
  display.display();
  delay(3600000); //Refresh every hour
}
