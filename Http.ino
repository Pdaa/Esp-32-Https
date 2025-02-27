#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "";   //Insert SSID here 
const char* password = "";   //Insert PASSWORD here 

String serverName = "https://api.energidataservice.dk/dataset/ElectricityProdex5MinRealtime?limit=1";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {

  if ((millis() - lastTime) > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverPath = serverName;

      
      http.begin(serverPath.c_str());


      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        
         String payload = http.getString();
         Serial.println(payload); //Print the string recived

        //Break the sting apart with , and : seperators
        String part11 = getValue(payload, ',', 9);  //Count , into the string
        // Serial.println(part11);
        String  part111 = getValue(part11, ':', 1); //from there : into the string
        Serial.print("OnshoreWindPower ");
        Serial.println(part111.toFloat());
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}