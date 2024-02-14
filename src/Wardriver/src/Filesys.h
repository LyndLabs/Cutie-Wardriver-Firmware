#pragma once

class Filesys {
    public:
        Filesys();

        typedef void (*ScreenUpdateCallback)(char* message);
        static void init(ScreenUpdateCallback callback);
        static void createLog(char * filename, ScreenUpdateCallback callback);
        static void write(char * data);
        static bool dedupe;
        static bool showHidden;
        static bool dynamicScan;
        static int timePerChan;

        static void open();
        static void close();

    private:    
        static void configure();
};
