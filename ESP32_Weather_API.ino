#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

//LCD Object (0x3F or 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Put your WiFi Credentials here
const char* ssid = "DML8825252";
const char* password = "Sony1234";

//URL Endpoint for the API
String URL = "https://api.open-meteo.com/v1/forecast?latitude=30.202163&longitude=-97.880721&current=temperature_2m,relative_humidity_2m,precipitation&temperature_unit=fahrenheit&timezone=America%2FChicago";

void setup() {
  Serial.begin(115200);

  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    //Set HTTP Request Final URL with Location and API key information
    http.begin(URL);

    // start connection and send HTTP Request
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {

      //Read Data as a JSON string
      String JSON_Data = http.getString();
      Serial.println(JSON_Data);

      //Retrieve some information about the weather from the JSON format
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, JSON_Data);
      JsonObject obj = doc.as<JsonObject>();

      //Display the Current Weather Info
      double temperature = doc["current"]["temperature_2m"].as<double>();
      double humidity = doc["current"]["relative_humidity_2m"].as<double>();
      String time = doc["current"]["time"].as<String>();

      lcd.clear();
      lcd.print(time);
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      lcd.print(" F | ");
      lcd.print(humidity);
      lcd.print(" %");

      http.end();
    }

  }
  
  //Wait for 30 seconds
  delay(60000);
}
