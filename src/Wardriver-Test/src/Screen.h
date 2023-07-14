#pragma once

class Screen {
    public:
        Screen();
        static void init();
        static void drawSplash(uint8_t sec);
        static void drawMockup(char* gpscoords, char* time, uint8_t icon1, uint8_t icon2, uint8_t icon3, uint8_t icon4, uint8_t icon5, uint8_t icon6, char* message);
};
