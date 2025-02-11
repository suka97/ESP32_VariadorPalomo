#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

esp_err_t http_callMeBot_send(String number, String apikey, String message, bool retry=true);
esp_err_t http_telegram_send(String chat_id, String token, String message, bool retry=true);

#endif