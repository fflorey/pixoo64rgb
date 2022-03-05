#ifndef MY__RGB__H__
#define MY__RGB__H__

#define SIZE_X 64
#define SIZE_Y 64
#define SIZE SIZE_X *SIZE_Y

#include <stdlib.h>
#include <Arduino.h>


typedef struct __RGB {
    unsigned char *r;
    unsigned char *g;
    unsigned char *b;
} RGB;

class Sprite {
    // nice hack: r = 0x01, g = 0x02, b = 0x03 means: transparent pixel
    public:
        Sprite(int _width, int _height, const uint8_t *init) : width(_width), height(_height) {
            int counter = 0;
            r = (uint8_t *)malloc(sizeof(uint8_t)*width*height);
            g = (uint8_t *)malloc(sizeof(uint8_t)*width*height);
            b = (uint8_t *)malloc(sizeof(uint8_t)*width*height);
            for ( int y = 0; y < height; y++ ) {
                for ( int x = 0; x < width; x++ ) {                    
                    r[x+y*width] = init[counter*3];
                    g[x+y*width] = init[counter*3+1];
                    b[x+y*width] = init[counter*3+2];
                    counter++;
                }
            }
        }
        ~Sprite() {
            if ( r ) free(r);
            if ( g ) free(g);
            if ( b ) free(b);
        }
        char getR ( int x, int y ) {
            return r[y*width+x];
        }
        char getG ( int x, int y ) {
            return g[y*width+x];
        };
        char getB ( int x, int y ){
            return b[y*width+x];
        };        
        int getWidth() { 
            return width;
        }
        int getHeight() { 
            return height;
        }

    private:
        int width;
        int height;
        uint8_t *r;
        uint8_t *g;
        uint8_t *b;
};

RGB *newRGB();
void fillRGB(RGB *pic, unsigned char r, unsigned char g, unsigned char b);
void flushRGB(RGB *pic);
void plotPoint(RGB *pic, int x, int y, unsigned char r, unsigned char g, unsigned char b);
void plotFilledRect(RGB *pic, int x, int width, int y, int height, unsigned char r, unsigned char g, unsigned char b);
void plotRect(RGB *pic, int x, int width, int y, int height, unsigned char r, unsigned char g, unsigned char b);
void plotLine(RGB *pic, int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b);
void plotCircle(RGB *pic, int xc, int yc, int r, unsigned char rcolor, unsigned char g, unsigned char b);
void plotFilledCircle(RGB *pic, int x0, int y0, int radius, unsigned char rcolor, unsigned char g, unsigned char b);
void getBase64Encoded(RGB *pic, size_t *size, String &result_str );

void plotSprite(RGB *pic, int xpos, int ypos, Sprite *sprite );

#endif