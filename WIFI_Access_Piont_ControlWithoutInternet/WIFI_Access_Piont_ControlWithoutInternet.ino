// Load Wi-Fi library
#include <ESP8266WiFi.h>

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";
 
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String output13State = "off";

// Assign output variables to GPIO pins
const int output13 = 13;
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output13, OUTPUT);
    
  // Set outputs to high because we are using active low type relay module
  digitalWrite(output13, HIGH);
  
  // Connect to Wi-Fi network with SSID and password
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off

            //for GPIO13
            if (header.indexOf("GET /13/on") >= 0)
            {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, LOW);
            } 
            else if (header.indexOf("GET /13/off") >= 0)
            {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, HIGH);
            }
                   
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>TVC Model Rocket</h1>");
        
    // Display current state, and ON/OFF buttons for GPIO 13 
            client.println("<p>GPIO 13 - State " + output13State + "</p>");
            // If the output13State is off, it displays the ON button       
            if (output13State=="off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

          client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
