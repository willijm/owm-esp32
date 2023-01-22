#include <Arduino.h>
// load libraries
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>



#define docSize 24576
#define displayFreeHeap false
String owm_api_key = "";
String city = "échallat";
String country = "FR";

int heap_at_startup = 0;

// Replace with your network credentials
const char* SSID = "maison";
const char* PASSWORD = "";

String jsonBuffer;
String owm_icon;

void setup() {

  Serial.begin(115200);
  Serial.println("\n");

  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected at IP address:");
  Serial.println(WiFi.localIP());



  // Start Web Server
  if (WiFi.status() == WL_CONNECTED) {
    String serverPath = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country + "&units=metric" + "&lang=fr" + "&appid=" + owm_api_key;




    HTTPClient http;
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    // pour debug
    // Serial.println(httpResponseCode);

    if (httpResponseCode == 200) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String input = http.getString();
      Serial.println();
      // Serial.println("debug");
      Serial.println(input);

      // String input;

      DynamicJsonDocument doc(24576);

      DeserializationError error = deserializeJson(doc, input);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      } else {
        Serial.println("deserializeJson() no error.");

        //--- Copy from ArduinoJson Assistant
        float coord_lon = doc["coord"]["lon"];
        float coord_lat = doc["coord"]["lat"];

        JsonObject weather_0 = doc["weather"][0];
        int weather_0_id = weather_0["id"];
        const char* weather_0_main = weather_0["main"];
        const char* weather_0_description = weather_0["description"];
        const char* weather_0_icon = weather_0["icon"];



        const char* base = doc["base"];

        JsonObject main = doc["main"];
        float main_temp = main["temp"];
        float main_feels_like = main["feels_like"];
        float main_temp_min = main["temp_min"];
        float main_temp_max = main["temp_max"];
        int main_pressure = main["pressure"];
        int main_humidity = main["humidity"];

        int visibility = doc["visibility"];

        float wind_speed = doc["wind"]["speed"];
        int wind_deg = doc["wind"]["deg"];

        int clouds_all = doc["clouds"]["all"];

        long dt = doc["dt"];

        JsonObject sys = doc["sys"];
        int sys_type = sys["type"];
        int sys_id = sys["id"];
        const char* sys_country = sys["country"];
        long sys_sunrise = sys["sunrise"];
        long sys_sunset = sys["sunset"];

        int timezone = doc["timezone"];
        long id = doc["id"];
        const char* name = doc["name"];
        int cod = doc["cod"];

        //--- End of ArduinoJson Assistant ---

        // Print values.
        Serial.println("temp_min: " + String(main_temp_min));
        Serial.println("temp_max: " + String(main_temp_max));
        Serial.println("icon: " + String(weather_0_icon));
      }
    }
  }
}

void loop() {
}
