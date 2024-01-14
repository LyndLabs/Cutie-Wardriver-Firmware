#pragma once

class Screen {
    public:
        Screen();
        static void init();
        static void drawSplash(uint8_t sec);
        static void drawMockup(char* gpscoords, char* time, uint8_t icon1, uint16_t icon2, uint8_t icon3, uint8_t icon4, uint8_t icon5, uint8_t icon6, char* message);
        
        // icon bitmap, values, number of icons
        static void setIcons(uint8_t *icons, char **iconVals, uint8_t iconNum);
        static void setHeader(char *textHeader, char *textSubHeader);
        static void setFooter(char *textFooter);

        // wrappers for now
        static void update(); 
        static void clear();
        
        // static void setIcons(uint8_t *icons, uint8_t iconVals[], uint8_t iconNum);
};
