#include "Arduino.h"
#include "Screen.h"
#include "SH1106.h"
#include "graphics.h"
#include "../Vars.h"

SH1106Wire display(0x3C, SDA, SCL);

Screen::Screen() {

}

void Screen::init() {
    display.init();
    display.flipScreenVertically();
    display.setFont(DejaVu_Sans_Mono_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.clear();
}

uint8_t *iconsD;
char **iconValsD;
uint8_t iconNumD;


void Screen::setIcons(uint8_t *icons, char **iconVals, uint8_t iconNum) {
    iconsD = icons;
    iconValsD = iconVals;
    iconNumD = iconNum;
}

void Screen::drawMockup(char* gpscoords, char* time, uint8_t icon1, uint16_t icon2, uint8_t icon3, uint8_t icon4, uint8_t icon5, uint8_t icon6, char* message) {
    display.clear();
    display.drawLine(0,12,127,12);
    display.drawLine(0,49,127,49);
    display.drawLine(94,0,94,12);
    display.drawLine(95,0,95,12);

    char subbuff[16];
    sprintf(subbuff,"%.*s", 15, gpscoords);

    char messagesubstr[21];
    sprintf(messagesubstr,"%.*s", 20, message);

    

    display.drawString(98,0,time);
    display.drawString(0,0,subbuff);
    display.drawString(0,50,messagesubstr);

    display.drawString(20,17,String(icon1));
    display.drawString(20,34,(icon2 > 999)? (String((int) icon2/1000) + "K"):String(icon2));

    display.drawString(60,17,String(icon3));
    display.drawString(60,34,"30");

    display.drawString(100,17,"%"); // bat
    display.drawString(100,34,String(icon4)); //

    // display Nugget instead
    #if defined(ESP8266)
        display.drawString(60,17,String(icon3));
        display.drawString(60,34,String(icon4));
        display.drawString(102,17,String(icon5));
        display.drawString(102,34,String(icon6));
         display.drawXbm(0,0,128,64,icons_bits);
    #elif defined(ESP32)
        display.drawString(60,17,String(icon3));
        display.drawString(60,34,String(icon6));
        display.drawXbm(0,0,128,64,icons_bits);
    #endif

   
    display.display();

}

void Screen::drawSplash(uint8_t sec) {
    display.clear();
    display.drawXbm(0,0,128,64,splash_bits);
    display.drawString(52,40,"@AlexLynd");
    display.drawString(20,40,VERSION_STR);
    display.display();
    delay(sec*1000);
}

char *header;
char *subHeader;
char *footer;

// setters for 
void Screen::setHeader(char *textHeader, char *textSubHeader) {
    header = textHeader;
    subHeader = textSubHeader;
}

void Screen::setFooter(char *textFooter) {
    footer = textFooter;
}

/* Updates the screen when called*/
void Screen::update() {
    display.clear();

    // draw Header
    display.drawLine(0,12,127,12);
    display.drawLine(94,0,94,12);
    display.drawLine(95,0,95,12);

    display.drawString(0,0,header);
    display.drawString(98,0,subHeader);

    // draw Footer
    display.drawLine(0,49,127,49);
    display.drawString(0,49,footer);

    // draw Icons
    display.drawXbm(0,0,128,64,iconsD);

    display.drawString(20,17,iconValsD[0]);
    display.drawString(20,34,iconValsD[1]);

    display.drawString(60,17,iconValsD[2]);
    display.drawString(60,34,iconValsD[3]);

    display.drawString(105,17,iconValsD[4]);
    display.drawString(105,34,iconValsD[5]);

    display.display();
}

void Screen::clear() {
    display.clear();
}

