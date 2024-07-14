#ifndef __DIVOOM_64_HPP__
#define __DIVOOM_64_HPP__

#include <rgbplot.h>
#include <HTTPClient.h> 

#define DIVOOMADDR "http://192.168.178.115/post"

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
    int displayTemperature ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );
    int playBuzzer ( int ActiveTimeInCycle, int OffTimeInCycle, int PlayTotalTime );

    String findDevice();  // see: https://app.divoom-gz.com/Device/ReturnSameLANDevice

    String Time(int  x, int  y, int textID, String rgbColor, int withSeconds=0 );
    String Temperature ( int  x, int  y, String text, int textID, String rgbColor, int tempmode=0 );
    String Message ( int x, int y, String text, int textID, String rgbColor );



   private:
    int sendHttpPostRequest( const char *httpURL = DIVOOMADDR);
    int handleHttpReturnCode(int httpResponseCode);

    HTTPClient http;
    String response;
    String httpRequestData;
    int RGBcounter;
    int textID;
};

#endif