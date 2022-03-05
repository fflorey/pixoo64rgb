#include <WString.h>
#include <base64.h>
#include <stdio.h>
#include <stdlib.h>

#include "./rgbplot.h"



RGB *newRGB() {
    RGB *tmp = (RGB *)malloc(sizeof(RGB));
    tmp->r = (unsigned char *)malloc(sizeof(unsigned char) * SIZE);
    tmp->g = (unsigned char *)malloc(sizeof(unsigned char) * SIZE);
    tmp->b = (unsigned char *)malloc(sizeof(unsigned char) * SIZE);
    if (tmp == NULL || tmp->r == NULL || tmp->g == NULL || tmp->b == NULL) {
        exit(0);
    }
    return tmp;
}

void fillRGB(RGB *pic, unsigned char r, unsigned char g, unsigned char b) {
    for (int i = 0; i < SIZE; i++) {
        pic->r[i] = r;
        pic->g[i] = g;
        pic->b[i] = b;
    }
}

void flushRGB(RGB *pic) {
    char result[3 * SIZE];

    for (int i = 0; i < SIZE; i++) {
        result[i * 3] = pic->r[i];
        result[i * 3 + 1] = pic->g[i];
        result[i * 3 + 2] = pic->b[i];
    }
    for (int i = 0; i < SIZE * 3; i++) {
        fprintf(stdout, "%c", result[i]);
    }
}

void plotPoint(RGB *pic, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y) {
        int pos = y * SIZE_X + x;
        pic->r[pos] = r;
        pic->g[pos] = g;
        pic->b[pos] = b;
    }
}

void plotFilledRect(RGB *pic, int x, int width, int y, int height, unsigned char r,
                    unsigned char g, unsigned char b) {
    if (width > 0 && height > 0) {
        if (x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y &&
            x + width < SIZE_X && y + height < SIZE_Y) {
            for (int i = x; i < x + width; i++) {
                for (int j = y; j < y + height; j++) {
                    plotPoint(pic, i, j, r, g, b);
                }
            }
        }
    }
}

void plotRect(RGB *pic, int x, int width, int y, int height, unsigned char r, unsigned char g,
              unsigned char b) {
    if (width > 0 && height > 0) {
        if (x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y &&
            x + width <= SIZE_X && y + height <= SIZE_Y) {
            for (int i = x; i < x + width; i++) {
                for (int j = y; j < y + height; j++) {
                    if (i == x || j == y || i == x + width - 1 ||
                        j == y + height - 1) {
                        plotPoint(pic, i, j, r, g, b);
                    }
                }
            }
        }
    }
}

// RGB: #010203 means: transparent
void plotSprite(RGB *pic, int xpos, int ypos, Sprite *sprite ) {
    for (int y = 0; y < sprite->getHeight(); y++) {
        for (int x = 0; x < sprite->getWidth(); x++) {
            if ( sprite->getR(x,y) == 0x01 && sprite->getG(x,y) == 0x02 && sprite->getB(x,y) == 0x03 ) 
                continue;
            plotPoint(pic, xpos+x, ypos+y, sprite->getR(x,y),  sprite->getG(x,y), sprite->getB(x,y) );            
        }    
    }
}

void plotLine(RGB *pic, int x0, int y0, int x1, int y1, unsigned char r, unsigned char g,
              unsigned char b) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;) { /* loop */
        plotPoint(pic, x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}

void drawCircle(RGB *pic, int xc, int yc, int x, int y, unsigned char r, unsigned char g,
                unsigned char b) {
    plotPoint(pic, xc + x, yc + y, r, g, b);
    plotPoint(pic, xc - x, yc + y, r, g, b);
    plotPoint(pic, xc + x, yc - y, r, g, b);
    plotPoint(pic, xc - x, yc - y, r, g, b);
    plotPoint(pic, xc + y, yc + x, r, g, b);
    plotPoint(pic, xc - y, yc + x, r, g, b);
    plotPoint(pic, xc + y, yc - x, r, g, b);
    plotPoint(pic, xc - y, yc - x, r, g, b);
}

void plotCircle(RGB *pic, int xc, int yc, int r, unsigned char rcolor, unsigned char g, unsigned char b) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(pic, xc, yc, x, y, rcolor, g, b);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        drawCircle(pic, xc, yc, x, y, rcolor, g, b);
    }
}

void plotFilledCircle(RGB *pic, int x0, int y0, int radius, unsigned char rcolor, unsigned char g,
                      unsigned char b) {
    plotCircle(pic, x0, y0, radius, rcolor, g, b);
    int sr = 0;
    while (sr < radius) {
        plotCircle(pic, x0, y0, sr, rcolor, g, b);
        sr++;
    }
}

void getBase64Encoded(RGB *pic, size_t *size, String &res) {
    unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char) * 3 * SIZE);
    for (int i = 0; i < SIZE; i++) {
        result[i * 3] = pic->r[i];
        result[i * 3 + 1] = pic->g[i];
        result[i * 3 + 2] = pic->b[i];
    }
    // char *encoded = base64_encode((const unsigned char *)result, 3 * SIZE,
    // size, encodestr);
    base64 b;
    res = b.encode((uint8_t *)result, (size_t)3 * SIZE);
    *size = res.length();
    free(result);
}
