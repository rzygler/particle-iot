// fish tank temperature using a dallas compatible waterproof temperature sensor
// send the data to ubidots service

#include "Ubidots/Ubidots.h"
#include "OneWire/OneWire.h"
#include "spark-dallas-temperature/spark-dallas-temperature.h"

// update for your token
#define UBIDOTS_TOKEN "B..."

Ubidots ubidots(UBIDOTS_TOKEN);

// Data wire is plugged into port 0 on the Arduino
// Setup a oneWire instance to communicate with it
OneWire oneWire(D0 );

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature dallas(&oneWire);

// Create a variable that will store the temperature value
double tempC = 0.0;
double tempF = 0.0;
String ubidotsName = "fish01-temp";

void setup()
{
    // Register a Particle variable here
    Particle.variable("tempC", &tempC, DOUBLE);
    Particle.variable("tempF", &tempF, DOUBLE);

    // setup the library
    dallas.begin();
}

void debug(String str)
{
    Particle.publish("debug", str);
}

void loop()
{
    // Request temperature conversion from proble
    dallas.requestTemperatures();

    // get the temperature in Celcius
    float loopTempC = dallas.getTempCByIndex(0);

    // convert to Fahrenheit
    float loopTempF = DallasTemperature::toFahrenheit( loopTempC );

	// device freaks out on power outage and shows negative temps for a minute or so
    if (round(tempF) > 0)
    {
        // set public variable
        tempC = (double)loopTempC;

        // set public variable
        tempF = (double)loopTempF;

        // send temp data to ubidots
        ubidots.add(ubidotsName, tempF);  // Change for your variable name
        ubidots.sendAll();
    }
    else
    {
        // Particle.publish("temp out of loop");
    }

    // come back 5 mins later (1000 ms = 1 s)
    delay(300000);
}
