// ----------------------------------------------------- DHT11 ----------------------
#include "PietteTech_DHT.h"

#define DHTTYPE  DHT11       // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   5           // Digital pin for communications

//declaration
void dht_wrapper(); // must be declared before the lib initialization

// Lib instantiate
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);
int n;      // counter

float humidity;
float temperature;
char payload[1024];

TCPClient tcpClient;
#define ADAFRUIT_API_KEY "e51f171fae7f455cbaa57e35b6835d73"
#define ADAFRUIT_FEED_TEMPERATURE "temp"
#define ADAFRUIT_FEED_HUMIDITY "humidity"
#include "Adafruit_IO_Client.h"
#include "Adafruit_IO_Arduino.h"
Adafruit_IO_Client aio = Adafruit_IO_Client(tcpClient, ADAFRUIT_API_KEY);
Adafruit_IO_Feed aioFeedTemperature = aio.getFeed(ADAFRUIT_FEED_TEMPERATURE);
Adafruit_IO_Feed aioFeedHumidity = aio.getFeed(ADAFRUIT_FEED_HUMIDITY);
// ----------------------------------------------------- DHT11 ----------------------


void setup() {
    aio.begin();
}

// This wrapper is in charge of calling
// must be defined like this for the lib work
void dht_wrapper() { DHT.isrCallback(); }

void loop(){

    // ----------------------------------------------------- DHT11 ----------------------
    humidity = DHT.getHumidity();
    temperature = DHT.getCelsius();

    int result = DHT.acquireAndWait();
    sprintf(payload,
            "{\"temperature\":%.2f,\"humidity\":%.2f}",
            temperature,
            humidity);
    Particle.publish("temperature", payload, 604800, PRIVATE);

    aioFeedTemperature.send(temperature);
    aioFeedHumidity.send(humidity);

    switch (result) {
        case DHTLIB_OK:
            Particle.publish("temperature", "OK", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Particle.publish("temperature", "Error\n\r\tChecksum error", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_ISR_TIMEOUT:
            Particle.publish("temperature", "Error\n\r\tISR time out error", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_RESPONSE_TIMEOUT:
            Particle.publish("temperature", "Error\n\r\tResponse time out error", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_DATA_TIMEOUT:
            Particle.publish("temperature", "Error\n\r\tData time out error", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_ACQUIRING:
            Particle.publish("temperature", "Error\n\r\tAcquiring", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_DELTA:
            Particle.publish("temperature", "Error\n\r\tDelta time to small", 604800, PRIVATE);
            break;
        case DHTLIB_ERROR_NOTSTARTED:
            Particle.publish("temperature", "Error\n\r\tNot started", 604800, PRIVATE);
            break;
        default:
            Particle.publish("temperature", "Unknown error", 604800, PRIVATE);
            break;
    }
    // ----------------------------------------------------- DHT11 ----------------------
    delay(2500);

}
