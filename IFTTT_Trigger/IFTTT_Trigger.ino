// stepps to create the applet in --> https://scribehow.com/shared/Ifttt_Workflow__H5nkwZENTqK9VmjVsiHYaQ

#include <ESP8266WiFi.h>

//-------------------------Network Credentials---------------
char mySSID[] = "B10-F3-03";
char password[] = "00004815";

//---------------IFTTT requirments---------------------------
const char host[] = "maker.ifttt.com";
const char privatKey[] = "-----";
const String event = "SOS";

//--------------Variables -----------------------------------
int dataIn = A0;
int delayTime = 500;
float data; // to stoere the analog readings 

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);

  // ------------connect to the neywork---------------------
  WiFi.begin(mySSID, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Conected!");
  Serial.print("IP: "); Serial.print(WiFi.localIP());

}

void loop() {
  
  //------------Read sensor Data ----------
  data = analogRead(dataIn);
  data = map(data, 0, 1023, 0, 255);

  Serial.println(data);

  if (data >= 127) {
    // fire the triger for this event 
    callTrigger(host, privatKey, event, data, 0, 0); // note if you don't have any values to pass then set them to 0 or none
    
    Serial.println("Trigger has been fired");
    
  }

  delay(5*delayTime);
}



void callTrigger(String host, String privatKey, String event, int value1, int value2, int value3){
  //Connect to host seriever 
    //create a client
    Serial.println("Connecting to host....");

    WiFiClient client;

    if(!client.connect(host, 80)) {
      Serial.print("conection Failed!");
      delay(2*1000);
      return;
    }

    // creat the Url that 'll trigger the applet 

    String url = "/trigger/";
           url += event + "/with/key/" + privatKey + "?value1=" + String((int)value1) + "&value2=" + String((int)value2) + "&value3=" + String((int)value3);

    //Make the http request using the GET method 

    String http = "GET ";
           http += url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";

    // send the http requiest (fire the trigger)
    client.print(http);

    // Disconect from the servier 
    client.stop();

}
