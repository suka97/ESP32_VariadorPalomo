#include "global.h"

bool isTimeBetween(struct tm now, struct tm start, struct tm end) {
    if ( now.tm_hour < start.tm_hour ) return false;
    if ( now.tm_hour > end.tm_hour ) return false;
    if ( now.tm_hour == start.tm_hour && now.tm_min < start.tm_min ) return false;
    if ( now.tm_hour == end.tm_hour && now.tm_min > end.tm_min ) return false;
    return true;
}


int getProfileForTime(VF_Profile profiles[], struct tm now) {
    for ( int i=0 ; i<VF_PROFILES_MAX ; i++ ) {
        if ( !profiles[i].enabled ) continue;
        if ( isTimeBetween(now, profiles[i].time_start, profiles[i].time_end) ) {
            return i;
        }
    }
    return -1;
}


String getVfHtml(const String& var) {
    int profile = var.substring(2, 3).toInt();
    if ( profile < 0 || profile >= VF_PROFILES_MAX ) return String();

    String param = var.substring(4); // skip "vfX-"
    if ( param == "enabled" ) return settings.vf_profiles[profile].enabled ? "checked" : "";
    if ( param == "rel_temp2vel" ) return String(settings.vf_profiles[profile].rel_temp2vel);
    if ( param == "vel0" ) return String(settings.vf_profiles[profile].vel0);
    if ( param == "time_start" ) return time2string(settings.vf_profiles[profile].time_start);
    if ( param == "time_end" ) return time2string(settings.vf_profiles[profile].time_end);
    return String();
}


void vf_getSettings(EEPROM_Settings& sett, AsyncWebServerRequest *request) {
    for ( uint8_t i=0 ; i<VF_PROFILES_MAX ; i++ ) {
        sett.vf_profiles[i].enabled = request->hasParam("vf" + String(i) + "-enabled");
        sett.vf_profiles[i].rel_temp2vel = request->getParam("vf" + String(i) + "-rel_temp2vel")->value().toInt();
        sett.vf_profiles[i].vel0 = request->getParam("vf" + String(i) + "-vel0")->value().toInt();
        sett.vf_profiles[i].time_start = string2time(request->getParam("vf" + String(i) + "-time_start")->value());
        sett.vf_profiles[i].time_end = string2time(request->getParam("vf" + String(i) + "-time_end")->value());

        Serial.println("vf" + String(i) + ":");
        Serial.println("  enabled: " + String(sett.vf_profiles[i].enabled));
        Serial.println("  rel_temp2vel: " + String(sett.vf_profiles[i].rel_temp2vel));
        Serial.println("  vel0: " + String(sett.vf_profiles[i].vel0));
        Serial.println("  time_start: " + time2string(sett.vf_profiles[i].time_start));
        Serial.println("  time_end: " + time2string(sett.vf_profiles[i].time_end));
    }
}


void handleVfProfile() {
    if ( vf_profile == -1 ) return;
    float vel = settings.vf_profiles[vf_profile].vel0 + ds2820_temp * settings.vf_profiles[vf_profile].rel_temp2vel;
    vf_pwm = vel / settings.vf_vel_max * PWM_MAX_DUTY;
    ledcWrite(PWM_CHANNEL, vf_pwm); 
}