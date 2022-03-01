#ifndef __DIVOOM_64_HPP__
#define __DIVOOM_64_HPP__

#include <rgbplot.h>

class Divoom64Communicator {
   public:
    Divoom64Communicator();
    ~Divoom64Communicator();
    int clearAllTexts();
    int resetPicID();
    int sendRGB(RGB *screen);
    int sendText(int x, int y, String text, int textID, String rgbColor, int font = 3);
    int sendScrollText ( int x, int y, String text, int textID, String rgbColor, int textWidth, int scrollSpeed, int font = 3);

    int displayList ( String list);
    int displaySmallText ( int x, int y, String text, int textID, String rgbColor );
    int displayTime ( int  x, int  y, int textID, String rgbColor, int withSeconds=0 );
    int displayTemperatur ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );

    String Time(int  x, int  y, int textID, String rgbColor, int withSeconds=0 );
    String Temperatur ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );
    String Message ( int x, int y, String text, int textID, String rgbColor );

   private:
    int sendHttpPostRequest();
    int handleHttpReturnCode(int httpResponseCode);

    HTTPClient http;
    char lenstr[10];
    String httpRequestData;
    int RGBcounter;
    int textID;
};

#endif