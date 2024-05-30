#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>


const char *ssid     = "AS";
const char *password = "m8647741@+";

WiFiUDP ntpUDP;


NTPClient timeClient(ntpUDP, "http://time.nist.gov/", 7200); // this line chose the siervier from which we get the time (grinsh servier in this case) then we correct the time differernce in cairo we have 2 houre delay == 7200 seconds

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.print("Conected, IP:"); Serial.println(WiFi.localIP());

  timeClient.begin();
}

void loop() {

  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
  // for more details see his video  https://www.youtube.com/watch?v=_wwMx2AwA9Q&list=PLfgCIULRQavyhr5eZssPgJa36n3A6b7MM&index=14

}
