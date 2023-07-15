#pragma once

class Wardriver{
    public:
        Wardriver();
        static void init();
        static void scan();
        

    private:
        void getBattery();
        // void initSD();
};