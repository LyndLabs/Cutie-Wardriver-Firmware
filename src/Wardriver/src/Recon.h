#pragma once

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include "WiFi.h"
#endif

char* getAuthType(uint8_t wifiAuth) {
    static char authType[15];
    switch (wifiAuth) {

        #if defined(ESP8266)
            case ENC_TYPE_TKIP:
                return "[WPA-PSK-CCMP+TKIP][ESS]";
            case ENC_TYPE_CCMP:
                return "[WPA2-PSK-CCMP+TKIP][ESS]";
            case ENC_TYPE_WEP:
                return "[WEP][ESS]";
            case ENC_TYPE_NONE:
                return "[ESS]";
            case ENC_TYPE_AUTO:
                return "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][ESS]";

        #elif defined(ESP32)
            case WIFI_AUTH_OPEN:
                return "[]";
            case WIFI_AUTH_WEP:
                return "[WEP]";
            case WIFI_AUTH_WPA_PSK:
                return "[WPA]";
            case WIFI_AUTH_WPA2_PSK:
                return "[WPA2]";
            case WIFI_AUTH_WPA_WPA2_PSK:
                // return "WPA+WPA2";
                return "[WPA2]";
            case WIFI_AUTH_WPA2_ENTERPRISE:
                // return "WPA2-EAP";
                return "[WPA2]";
            case WIFI_AUTH_WPA3_PSK:
                return "[WPA3]";
            case WIFI_AUTH_WPA2_WPA3_PSK:
                // return "WPA2+WPA3";
                return "[WPA3]";
            case WIFI_AUTH_WAPI_PSK:
                return "[WPAI]";
        #endif

        default:
            return "[UNKNOWN]";
    }
    return authType;
}