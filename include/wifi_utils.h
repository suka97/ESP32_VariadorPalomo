#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

struct tm string2time(String time);
String time2string(struct tm timeinfo);
void syncLocalTime();
void fillIP(uint8_t *buffer, const String &ip);
void fillIP(uint8_t *buffer, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4);
bool connectToWifi(const char* ssid, const char* password);
String htmlProcessor(const String& var);

#endif