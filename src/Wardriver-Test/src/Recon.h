// #include <ESP8266WiFi.h>
// #include <SD.h>
// #include <SoftwareSerial.h>
// #include <TinyGPS++.h>

// #include "Screen.h"
// #include "Recon.h"

// // #define UTC_offset -7  // PDT
// #define SD_CS      D8

// String logFileName = "";
// int networks = 0;

// #define LOG_RATE 500
// char currentTime[5];
        
// SoftwareSerial ss(D4, D3); // RX, TX
// TinyGPSPlus tinyGPS;
// extern SH1106Wire display;


//   char currentGPS[17];
//   char currTime[6];

// /** Screen Variables **/
// uint32_t totalNets = 0;
// uint8_t  clients = 0;
// uint8_t  openNets = 0;
// uint8_t  sats = 0;
// uint8_t  bat = 0;
// uint8_t  speed = 0;

// float lat = 0; float lng = 0;
// uint8_t hr; uint8_t mn;

// void setup() {
//   Serial.begin(115200);
//   ss.begin(9600);


//   WiFi.mode(WIFI_STA); WiFi.disconnect();
//   delay(500);

//   if (ss.available() > 0) {
//     drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Waiting for fix...");
//   }
//   else {
//     drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: NOT FOUND");
//   }

//   while (!tinyGPS.location.isValid()) {
//     Serial.println(tinyGPS.location.isValid());
//     delay(0);
//     smartDelay(500);
//   }

//   sats = tinyGPS.satellites.value();
//   hr = tinyGPS.time.hour();
//   mn = tinyGPS.time.minute();
//   lat = tinyGPS.location.lat();
//   lng = tinyGPS.location.lng();

//   sprintf(currentGPS,"%1.3f,%1.3f",lat,lng);
        

//   sprintf(currTime,"%02d:%02d",hr,mn);

//   drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: LOCATION FOUND");
//   delay(500);

//   if (!SD.begin(SD_CS)) {
//     drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"SD Card: NOT FOUND");
//     while (!SD.begin(SD_CS)) {delay(0);}
//   }

//   drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"SD Card: FOUND");
//   initializeSD();

// }

// void lookForNetworks() {
//   sprintf_P(currentTime, PSTR("%02d:%02d"),tinyGPS.time.hour(),tinyGPS.time.minute());

//   int n = WiFi.scanNetworks();

//   totalNets+= n;
//   openNets = 0;
//   lat = tinyGPS.location.lat();
//   lng = tinyGPS.location.lng();
//   hr = tinyGPS.time.hour();
//   mn = tinyGPS.time.minute();
//   sats = tinyGPS.satellites.value();
//   speed = int(tinyGPS.speed.mph());

//   if (n == 0) {
//     Serial.println("no networks found");
//   }
//   else {
//     totalNets+= n;
//     for (int i = 0; i < n; ++i) {
//       if ((WiFi.channel(i) > 0) && (WiFi.channel(i) < 15)) {
//         sprintf_P(currentTime, PSTR("%02d:%02d"),tinyGPS.time.hour(),tinyGPS.time.minute());

//         networks++;
//         File logFile = SD.open(logFileName, FILE_WRITE);
//         logFile.print(WiFi.BSSIDstr(i));  logFile.print(',');
//         logFile.print(WiFi.SSID(i)); logFile.print(',');

//         if (WiFi.SSID(i).length() > 20) { }// display.println(WiFi.SSID(i).substring(0, 18 ) + "..."); }
//             else { // display.println(WiFi.SSID(i)); }
//             String bssid = WiFi.BSSIDstr(i);
//             bssid.replace(":", "");
//             // display.println(bssid + "    (" + WiFi.RSSI(i) + ")");
//             logFile.print(getEncryption(i,"")); logFile.print(',');
//             // display.print("Enc: "+getEncryption(i,"screen"));
//             // display.println("   Ch: "+ String(WiFi.channel(i)));    
//             // display.println();    
//             // display.setCursor(0,40);

//             if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
//               openNets++;
//             }

//             // replace this shit with print formatting
//             logFile.print(tinyGPS.date.year());   logFile.print('-');
//             logFile.print(tinyGPS.date.month());  logFile.print('-');
//             logFile.print(tinyGPS.date.day());    logFile.print(' ');
//             logFile.print(tinyGPS.time.hour());   logFile.print(':');
//             logFile.print(tinyGPS.time.minute()); logFile.print(':');
//             logFile.print(tinyGPS.time.second()); logFile.print(',');
//             logFile.print(WiFi.channel(i)); logFile.print(',');
//             logFile.print(WiFi.RSSI(i)); logFile.print(',');
//             logFile.print(tinyGPS.location.lat(), 6); logFile.print(',');
//             logFile.print(tinyGPS.location.lng(), 6); logFile.print(',');
//             // display.println("Networks: " + String(networks));
//             // display.print(String(int(tinyGPS.speed.mph())) + " MPH");
//             // display.println(" Sats: " + String(tinyGPS.satellites.value()));
//             // display.println("(" + String(tinyGPS.location.lat(), 5) + "," + String(tinyGPS.location.lng(), 5) + ")");
//             logFile.print(tinyGPS.altitude.meters(), 1); logFile.print(',');
//             logFile.print(tinyGPS.hdop.value(), 1); logFile.print(',');
//             logFile.println("WIFI");
//             logFile.close();
            

//             }

//     }
//     }
//   }
// }
//     //     // display.display();
//     //     if (getEncryption(i,"")=="[WEP][ESS]"){  // flash if WEP detected
//     //       // display.invertDisplay(true);  delay(200);
//     //       // display.invertDisplay(false); delay(200);
//     //       // display.invertDisplay(true);  delay(200);
//     //       // display.invertDisplay(false); delay(200);
//         // }
// void loop() {

//  delay(1000);
//   if (tinyGPS.location.isValid()) {
// //     setTime(tinyGPS.time.hour(), tinyGPS.time.minute(), tinyGPS.time.second(), tinyGPS.date.day(), tinyGPS.date.month(), tinyGPS.date.year());
// //     adjustTime(UTC_offset * SECS_PER_HOUR);  
//     drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"WiFi: Scanning...");
//     lookForNetworks();

//     char message[21];
//     sprintf(message,"Logged %d networks.",networks);
//     drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,message);
//   }
//   smartDelay(LOG_RATE);
//   if (millis() > 5000 && tinyGPS.charsProcessed() < 10) {
//     drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: NO DATA");
//     Serial.println("No GPS data received: check wiring");
//   }

// }

// static void smartDelay(unsigned long ms) {
//   unsigned long start = millis();
//   do {
//     while (ss.available())
//       tinyGPS.encode(ss.read());
//   } while (millis() - start < ms);
// }

// // int isOnFile(String mac) {
// //   File netFile = SD.open(logFileName);
// //   String currentNetwork;
// //   if (netFile) {
// //     while (netFile.available()) {
// //       currentNetwork = netFile.readStringUntil('\n');
// //       if (currentNetwork.indexOf(mac) != -1) {
// //         netFile.close();
// //         return currentNetwork.indexOf(mac);
// //       }
// //     }
// //     netFile.close();
// //     return currentNetwork.indexOf(mac);
// //   }
// // }

// void initializeSD() { // create new CSV file and add WiGLE headers
//   int i = 0; logFileName = "log0.csv";
//   while (SD.exists(logFileName)) {
//     i++; logFileName = "log" + String(i) + ".csv";
//   }
//   File logFile = SD.open(logFileName, FILE_WRITE);

//   char tmpMessage[20];
//   sprintf(tmpMessage,"Created: %s",logFileName);
//   drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,tmpMessage);
  
//   if (logFile) {
//     logFile.println("WigleWifi-1.4,appRelease=2.53,model=D1-Mini-Pro,release=0.0.0,device=NetDash,display=SSD1306,board=ESP8266,brand=Wemos");
//     logFile.println("MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type");
//   }
//   logFile.close();
// }

// String getEncryption(uint8_t network, String src) { // return encryption for WiGLE or print
//   byte encryption = WiFi.encryptionType(network);
//   switch (encryption) {
//     case 2:
//       if (src=="screen") { return "WPA"; }
//       return "[WPA-PSK-CCMP+TKIP][ESS]";
//     case 5:
//       if (src=="screen") { return "WEP"; }
//       return "[WEP][ESS]";
//     case 4:
//       if (src=="screen") { return "WPA2"; }
//       return "[WPA2-PSK-CCMP+TKIP][ESS]";
//     case 7:
//       if (src=="screen") { return "NONE" ; }
//       return "[ESS]";
//     case 8:
//       if (src=="screen") { return "AUTO"; }
//       return "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]";
//   }
//   return "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]";
// }
