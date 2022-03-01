# Use your pixoo64 as a remote display for your esp32

This library gives a more easy access to the REST API of the Divoom Pixoo64 from your ESP32 or similiar devices (has only been tested with some esp32 dev boards!)
The REST API can be found at
https://www.divoom.com/de-de/apps/help-center#!hc-pixoo64-developeropen-sourcesdkapiopen-source
(as of Feb 2022)

## Contribution
A big "thank you" for the idea to create the image in memory upfront with some simple algorithms goes to Ron Talman and his work: 
GitHub - SomethingWithComputers/pixoo: A library to help you make the most out of your Pixoo 64 (and hopefully soon other Wi-Fi enabled Pixoos)

With some simple lines of code and this library, you can use the Pixoo64 as a wireless display for your ESP32.

Example:
```
#include <Arduino.h>
#include <HTTPClient.h>
#include <Wifi.h>
#include <divoom64com.h>
#include <rgbplot.h>

const char* ssid = "YOUR WLAN SSID GOES HERE";
const char* password = "AND YOUR PASSSWORD...";

RGB* pixoo64Display = NULL;

void setup() {
    Serial.begin(115200);  // Any baud rate should work
    Serial.println("Hello Arduino\n");
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println(" Connected!");
    // initialize display
    pixoo64Display = newRGB();

}

void loop() {
    Serial.println("go");
    if (WiFi.status() == WL_CONNECTED) {
        Divoom64Communicator divoomClient; // setup communication line to device
        fillRGB(pixoo64Display, 0x0, 0x0, 0x0); // fill internal image black
        plotRect(pixoo64Display, 1,10,10,10,0xfc, 0xf4, 0x03 ); // plot a rect at x:1, y:10 width:10, height: 10
        divoomClient.resetPicID();
        divoomClient.sendRGB(pixoo64Display);  // finally, send to pixoo device
    }
    delay(25000);
}

```

(Directory structure follows PlatformIO instructions.)
