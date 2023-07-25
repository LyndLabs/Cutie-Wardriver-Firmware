#pragma once

class Filesys {
    public:
        Filesys();

        typedef void (*ScreenUpdateCallback)(char* message);
        static void init(char * filename, ScreenUpdateCallback callback);
        static void write(char * data);

        static void open();
        static void close();
};
