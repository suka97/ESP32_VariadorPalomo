#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

esp_err_t http_callMeBot_send(String number, String apikey, String message, bool retry=true);

#endif