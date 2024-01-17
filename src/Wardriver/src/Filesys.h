#include <sys/stat.h>
#pragma once

class Filesys {
public:
  Filesys();

  typedef void (*ScreenUpdateCallback)(char* message);
  static void init(ScreenUpdateCallback callback);
  static void createLog(char* filename, ScreenUpdateCallback callback);
  static void write(char* data);

  static void open();
  static void close();
  static bool dedupe;
  static bool showHidden;
  static bool dynamicScan;
  static int timePerChan;

private:
  static void configure();
};
