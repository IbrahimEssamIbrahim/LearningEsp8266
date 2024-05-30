#include <ESP8266WiFi.h>

//-------------------------Network Credentials---------------
char mySSID[] = "AS";
char password[] = "m8647741@+";

//--------------Variables -----------------------------------
int dataIn = A0;
int delayTime = 500;
float data;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println()
  
  // ------------connect to the neywork---------------------
  WiFi.begin(mySSID, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conected!");
  Serial.print("IP: "); Serial.print(WiFi.localIP())
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
