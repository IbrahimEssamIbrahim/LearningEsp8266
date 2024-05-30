// stepps to create the applet in --> https://scribehow.com/shared/Ifttt_Workflow__H5nkwZENTqK9VmjVsiHYaQ

#include <ESP8266WiFi.h>
//-------------------------Network Credentials---------------
char mySSID[] = "B10-F3-03";
char password[] = "00004815";

//---------------IFTTT requirments---------------------------
const char host[] = "api.thingspeak.com";
const char privatKey[] = "---------------------";

//--------------Variables -----------------------------------
String data; // to stoere the analog readings

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

  //------------Read Data ----------

  data = callTrigger(host, privatKey); // note if you don't have any values to pass then set them to 0 or none
  // format the data to only get the time 
  data = FormatData(data); 
  Serial.println(data);
  delay(1000);
}



String callTrigger(String host, String privatKey) {
  //Connect to host seriever
  //create a client
  Serial.println("Connecting to host....");

  WiFiClient client;

  if (!client.connect(host, 80)) {
    Serial.print("conection Failed!");
    delay(2 * 1000);
    return "";
  }

  //Url formate
  String http = "GET ";
  http += privatKey+"&headers=false" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";

  // send the http requiest (fire the trigger)
  client.print(http);
  //you need to wait at least 1.5 sec 
  delay(1500); 
  
  String Data = "";
  
  if(client.available()){
    Data = client.readString();
//    Serial.println(Data);
  }
  // Disconect from the servier
  client.stop();
  return Data;
}

String FormatData(String dataIn) {
  //note this will differ depinding on your applecation or the site you are using 
  //check pm or am then find the index of it 
  int index;
  if (dataIn.indexOf("am") != -1) index = dataIn.indexOf("am");
  else if (dataIn.indexOf("pm") != -1)  index = dataIn.indexOf("pm");
  else  return "Not valied Data!";

  // get the part the part of string that contain the time 
  dataIn = dataIn.substring(index-8, index+2);
  return dataIn;

}
