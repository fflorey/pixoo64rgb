#include <HTTPClient.h>
#include <divoom64com.h>
#include <rgbplot.h>

#define DIVOOMADDR "http://YOURLOCALIPHERE/post" // e.g.: "http://192.168.178.10/post"

Divoom64Communicator::Divoom64Communicator() {
    http.begin(DIVOOMADDR);
    RGBcounter = 0;
}

Divoom64Communicator::~Divoom64Communicator() { http.end(); }

int Divoom64Communicator::clearAllTexts() {
    httpRequestData = "{ \"Command\": \"Draw/ClearHttpText\" }";
    return sendHttpPostRequest();
}

int Divoom64Communicator::resetPicID() {
    httpRequestData = "{ \"Command\": \"Draw/ResetHttpGifId\" }";
    int ret = sendHttpPostRequest();
    if (ret >= 0) {
        RGBcounter = 1;
    }
    return ret;
}

int Divoom64Communicator::sendRGB(RGB *myscreen) {
    size_t len;
    String result;
    getBase64Encoded(myscreen, &len, result);

    httpRequestData =
        "{ \"Command\": \"Draw/SendHttpGif\", \"PicNum\": 1, \"PicWidth\": "
        "64,  \"PicOffset\": 0, \"PicID\": " +
        String(RGBcounter++) + " , \"PicSpeed\": 1000, \"PicData\": \"" +
        result + String("\"}");
    return sendHttpPostRequest();
}

int Divoom64Communicator::sendText(int x, int y, String text, int textID,
                                   String rgbColor, int font ) {
    if (x > SIZE_X || y > SIZE_Y || x < 0 || y < 0) {
        return -1;
    }
    httpRequestData =
        "{ \"Command\": \"Draw/SendHttpText\", \"TextId\":" + String(textID) +
        ", \"x\": " + String(x) + ", \"y\": " + String(y) +
        ", \"dir\": 0, \"font\": " + String(font) +
        ", \"TextWidth\": 64, \"speed\": 0, \"TextString\": \"" + text +
        "\", \"color\": \"" + rgbColor + "\", \"align\": 1 }";
    return sendHttpPostRequest();
}

int Divoom64Communicator::sendScrollText( int x, int y, String text, int textID, String rgbColor, int textWidth, int scrollSpeed, int font ) {
    httpRequestData =
        "{ \"Command\": \"Draw/SendHttpText\", \"TextId\":" + String(textID) +
        ", \"x\": " + String(x) + ", \"y\": " + String(y) +
        ", \"dir\": 0, \"font\": " + String(font) + ", \"speed\": " + (scrollSpeed > 100?String(100):String(scrollSpeed)) + 
        ", \"TextWidth\": "+String(textWidth)+", \"speed\": 0, \"TextString\": \"" + text +
        "\", \"color\": \"" + rgbColor + "\", \"align\": 1 }";
    return sendHttpPostRequest();
}


int Divoom64Communicator::displaySmallText ( int x, int y, String text, int textID, String rgbColor ) {
    int len = text.length(); 
    if ( len * 4 + x + 2 > 64 ) return -1;
    httpRequestData = "{ \"Command\": \"Draw/SendHttpItemList\", \"ItemList\": [" + Message(x,y,text,textID,rgbColor) + "]}";
    Serial.printf("%s\n", httpRequestData.c_str());
    return sendHttpPostRequest();
}

int Divoom64Communicator::displayTime ( int x, int y, int textID, String rgbColor, int withSeconds ) {
    if ( 30 + x > 64 ) return -1;
    httpRequestData = "{ \"Command\": \"Draw/SendHttpItemList\", \"ItemList\": [" + Time(x,y,textID,rgbColor,withSeconds) + "]}";
    Serial.printf("%s\n", httpRequestData.c_str());
    return sendHttpPostRequest();
}

int Divoom64Communicator::displayTemperatur ( int x, int y, String text, int textID, String rgbColor, int tempmode ) {
    if ( 22 + x > 64 ) return -1;
    httpRequestData = "{ \"Command\": \"Draw/SendHttpItemList\", \"ItemList\": [" + Temperatur(x,y,text,textID,rgbColor,tempmode) + "]}";
    Serial.printf("%s\n", httpRequestData.c_str());
    return sendHttpPostRequest();
}

String Divoom64Communicator::Time ( int x, int y, int textID, String rgbColor, int withSeconds ) {
    return "{ \"type\": "+String((withSeconds==1?6:5))+", \"TextId\":" + String(textID) +
        ", \"x\": " + String(x) + ", \"y\": " + String(y) +
        ", \"dir\": 0, \"font\": 18, \"TextWidth\": "+String(withSeconds==1?30:20)+",  \"Textheight\": 6, \"speed\": 0, \"color\": \"" + rgbColor + "\", \"align\": 1 }";
}

String Divoom64Communicator::Temperatur ( int  x, int  y, String text, int textID, String rgbColor, int tempmode ) {
    return "{ \"type\": "+String((tempmode+17))+", \"TextId\":" + String(textID) +
        ", \"x\": " + String(x) + ", \"y\": " + String(y) +
        ", \"dir\": 0, \"font\": 18, \"TextWidth\": 30,  \"Textheight\": 6, \"speed\": 0, \"color\": \"" + rgbColor + "\", \"align\": 1 }";
}
String Divoom64Communicator::Message ( int x, int y, String text, int textID, String rgbColor ) {
    int len = text.length()*4+2; 
    return "{ \"type\": 22, \"TextId\":" + String(textID) + ", \"x\": " + String(x) + ", \"y\": " + String(y) +
        ", \"dir\": 0, \"font\": 18, \"TextWidth\": "+String(len)+",  \"Textheight\": 6, \"speed\": 0, \"TextString\": \"" + text +
        "\", \"color\": \"" + rgbColor + "\", \"align\": 1 }";
}

int Divoom64Communicator::displayList ( String itemList ) {
    httpRequestData = "{ \"Command\": \"Draw/SendHttpItemList\", \"ItemList\": [ " + itemList + "]}";    
    Serial.printf("%s\n", httpRequestData.c_str());
    return sendHttpPostRequest();
}


int Divoom64Communicator::sendHttpPostRequest() {
    http.addHeader("Content-Type", "application/json");
    sprintf(lenstr, "%d", httpRequestData.length());
    http.addHeader("Content-Length", lenstr);
    int httpResponseCode = http.POST(httpRequestData);
    Serial.printf("Response: %d\n", httpResponseCode);
    return handleHttpReturnCode(httpResponseCode);
}

int Divoom64Communicator::handleHttpReturnCode(int httpResponseCode) {
    if (httpResponseCode < 0) {
        Serial.printf("[HTTP] POST... failed, error: %s\n",
                      http.errorToString(httpResponseCode).c_str());
    }
    return httpResponseCode;
}
