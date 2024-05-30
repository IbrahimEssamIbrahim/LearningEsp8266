
//-------Libraries----------------

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// Pins variables
int LED = 4; //~D2
int dataIn = A0;
int data;
int digital_width = 0; // this variable is used to store the bar width ==> HTML code

//---------WiFi credentials -------

const char ssid[] = "Home.Essam";
const char password[] = "159753123456MmEe##  @";

// get server static IP address you can use cmd => ipconfig 
IPAddress serverIP(192, 168, 1, 3); // Inter your IP address you want
IPAddress gateway(192, 168, 1, 1); // Inter your router gateway
IPAddress subnet(255, 255, 255, 0); 

// create webserver at port -- 
ESP8266WebServer server(80);

//---------webpage html ----------

char temp[1450]; //Note the size of the array should be bigger than the number of the chatecters in the html code so you can use online tool like=> https://wordcounter.net/character-count  to get the number of chars, 
//  ===>  note to add \ at the start of th ecode and onother \ at the end
// the variables that you want to be edited later should be replaced with %d ==>and if there already an % sign then you replace it to %d%%   ==>Ex:  70% ➡➡ %d%%  

char HtmlWebpage[1450] = "\
<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
<title>ESP8266</title>\
<meta charset=\"utf-8\" />\
<meta http-equiv=\"refresh\" content=\"5; url=http://192.168.1.3\" />\
<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/css/bootstrap.min.css\"/>\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\
<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css\"/>\
<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.6.1/jquery.min.js\"></script>\
<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js\"></script>\
</head>\
<body>\
<div class=\"container\">\
<div class=\"jumbotron\">\
<h1>http Website with Esp8266 that display some variables</h1>\
<p>Some buttons!</p>\
<a href=\"/?led=on\" class=\"btn btn-outline-primary\" role=\"button\">LED_ON</a>\
<a href=\"/?led=off\" class=\"btn btn-outline-primary\" role=\"button\">LED_OFF</a>\
</div>\
<h3>Temperature</h3>\
<div class=\"progress\">\
<div class=\"progress-bar progress-bar-striped bg-warning\" style=\"width: %d%%\">%d%%</div>\
</div>\
<br />\
<h3>Humidity</h3>\
<div class=\"progress\">\
<div class=\"progress-bar progress-bar-striped bg-info\" style=\"width: %d%%\">%d%%</div>\
</div>\
<br />\
<h3>Brightness</h3>\
<div class=\"progress\">\
<div class=\"progress-bar progress-bar-striped bg-dark\" style=\"width: %d%%\">%d%%</div>\
</div>\
</div>\
</body>\
</html>";

//--------------------------End of HTML-------------------------------------

void handleRoot() {

  //-------- handke button requests  ---------
  // if the url contain the herf= "led = on" or herf= "led = off"
  if (server.argName(0) == "led") {
    if (server.arg(0) == "on") digitalWrite(LED, HIGH); // if the argument of herf is on 
    if (server.arg(0) == "off") digitalWrite(LED, LOW); // if the argument of herf is off 
  }


  // get the sensor readings 
  data = analogRead(dataIn);
  data = map(data, 0, 1024, 0, 100);  // this because the bar max value is 100% another way is ⬇⬇⬇
//  digital_width = data * 100/255 // to covert it to persent 

  //----------------- change variable in html code == this snprintf searches foe %d%% and replace it with the value NOTE that the order of vas matter  
  snprintf(temp, 1450, HtmlWebpage, data, data, data*2/3, data*2/3, data*1/3, data*1/3); // I passed some fake values but you can change them latter  
  // ask the server to lanch page 
  server.send(200, "text/html", temp);
}
// if the page were not found 
void handleNotFound() {
  
  server.send(404, "text/html", "<h1>Page NOT Found!</h1>");
}



void setup(void) {
  // Note the next 2 lines always must be at the starting of void setup 
  // --------------connect to network-----------
  WiFi.config(serverIP, gateway, subnet);
  WiFi.begin(ssid, password);
  
  Serial.begin(115200);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected!, IP address: ");
  Serial.println(WiFi.localIP());
  MDNS.begin("esp8266"); // this line allow us to acces the site without typing the 192.168.1.. as it will be hard to remember insted we can type http://esp8266.local. Or what ever name we use in the MDNS function 
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // initialize pins

  pinMode(dataIn, INPUT);
  pinMode(LED, OUTPUT);
 
  // the next 2 lines handle the webpage (Post webpage, and get the readings if there any) or handle any error 404
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  // tells the server to start listening to any connections 
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
    MDNS.update();

}
