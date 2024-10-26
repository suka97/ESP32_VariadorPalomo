#include "global.h"


esp_err_t http_callMeBot_send(String number, String apikey, String message, bool retry) {
    Serial.println("[CallMeBot]: Sending \""+message+"\" to "+number);
    message.replace(" ", "+");

    for (int tries=3 ; tries>0 ; tries--) {
        HTTPClient http;
        http.begin("http://api.callmebot.com/whatsapp.php?text=" + message
            + "&phone=" + number
            + "&apikey=" + apikey
        );
        int httpCode = http.GET();
        if (httpCode > 0) {
            Serial.printf("[CallMeBot] HTTP GET... code: %d\n", httpCode);
            if (httpCode == 200 || httpCode == 210) {       // 210: Too many messages sent, but ok
                String payload = http.getString();
                // Serial.println(payload);
                
                Serial.println("[CallMeBot]: Message sent!");
                http.end();
                return ESP_OK;
            }
        } else {
            Serial.printf("[CallMeBot] HTTP GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();

        if (!retry) return ESP_FAIL;
        Serial.println("[CallMeBot]: Retrying in 5 seconds...");
        delay(5000);
    }

    return ESP_OK;
}