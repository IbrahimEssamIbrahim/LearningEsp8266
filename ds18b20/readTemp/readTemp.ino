// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  
  sensors.begin(); // Start up the library
}


void loop(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
 
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature C is: ");
    Serial.print(tempC);
    Serial.print(" Temperature F is: ");
    Serial.println();
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
}
