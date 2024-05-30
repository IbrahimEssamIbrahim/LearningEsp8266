#include <ESP8266WiFi.h>

const char mySSID[] = "AS";
const char Password[] = "m8647741@+" ;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(mySSID, Password);

  Serial.println("Connecting");
  /*
    // way number 1
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
  */

  // way 2
  int counter = 20;

  while (  counter >= 0 && WiFi.status() != 3 ) {
    counter--;
    delay(500);
    Serial.print(".");

  }
  
  switch (WiFi.status()) {

    case 3: // or use WL_CONNECTED they have the same value 
      Serial.print("Connected, IP address: ");
      Serial.println(WiFi.localIP());
      break;
    case 1:
      Serial.print("SSID cannot be reached");
      break;

    case 7:
      Serial.print("WRONG_PASSWORD");
      break;

    default:
      Serial.println("Unknown Error");
      delay(5000);
      ESP.reset();

  }

}

void loop() {



}
