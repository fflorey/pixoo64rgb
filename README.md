# Use your pixoo64 as a remote display for your esp32

This library gives a more easy access to the REST API of the Divoom Pixoo64 from your ESP32 or similiar devices (has only been tested with some esp32 dev boards!)
The REST API can be found at
https://www.divoom.com/de-de/apps/help-center#!hc-pixoo64-developeropen-sourcesdkapiopen-source
(as of Feb 2022)

## Thanks
A big "thank you" for the idea to create the image in memory upfront with some simple algorithms goes to Ron Talman and his work: 
https://github.com/SomethingWithComputers/pixoo

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

# Documentation for Divoom64Communicator - class

###    int clearAllTexts();
Clear all texts on display which are drawn with `sendText`, `sendScrollText`.
###    int resetPicID();
Sets the internal used PicID on the divoom device back to 0: You should allways use this before sending your first rgb to device
### int sendRGB(RGB *screen);
Send (prepared) RGB * to device
### int sendText(int x, int y, String text, int textID, String rgbColor, int font = 3);
Note: Divoom provides 7 different fonts (parameter font), the textID identifies the string on the display. This lets you modify a specific string on the display if needed
### int sendScrollText ( int x, int y, String text, int textID, String rgbColor, int textWidth, int scrollSpeed, int font = 3);
Same as sendText, but Text is scrolling with scrollSpeed scrollSpeed
### int displayList ( String list);  
Display Messages, Time or Temperatur strings with a single statement. Note: You can only use one displayList statement on a screen. Multiple calls deletes other Texts. 
Note: fonts are different to the ones available for sendText. Use this method especially, when you need small fonts
###  int displaySmallText ( int x, int y, String text, int textID, String rgbColor );
###  int displayTime ( int  x, int  y, int textID, String rgbColor, int withSeconds=0 );
###  int displayTemperatur ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );
All methods are needed for displayList


    String Time(int  x, int  y, int textID, String rgbColor, int withSeconds=0 );
    String Temperatur ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );
    String Message ( int x, int y, String text, int textID, String rgbColor );


(Directory structure follows PlatformIO instructions.)
