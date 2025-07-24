#include "Watchy_Nachi.h"

#define DARKMODE false  // Light background

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void WatchyNachi::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    
    drawTime();
    drawDate();
    drawSteps();
    drawDualTemperature();  // New dual temperature display
    drawBattery();
    
    // WiFi indicator
    display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if(BLE_CONFIGURED){
        display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyNachi::drawTime(){
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setCursor(5, 53+5);
    uint8_t displayHour;
    if(HOUR_12_24==12){
      displayHour = ((currentTime.Hour+11)%12)+1;
    } else {
      displayHour = currentTime.Hour;
    }
    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.println(currentTime.Minute);
}

void WatchyNachi::drawDate(){
    display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if(currentTime.Wday == 4){
        w = w - 5;
    }
    display.setCursor(85 - w, 85);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(85 - w, 110);
    display.println(month);

    display.setFont(&DSEG7_Classic_Bold_25);
    display.setCursor(5, 120);
    if(currentTime.Day < 10){
    display.print("0");
    }
    display.println(currentTime.Day);
    display.setCursor(5, 150);
    display.println(tmYearToCalendar(currentTime.Year));
}

void WatchyNachi::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(35, 190);
    display.println(stepCount);
}

void WatchyNachi::drawBattery(){
    display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyNachi::drawDualTemperature(){
    weatherData currentWeather = getWeatherData();
    
    // Draw weather temperature  
    int8_t weatherTemp = currentWeather.temperature;
    display.setFont(&DSEG7_Classic_Regular_15);  // Use smaller font
    display.setCursor(120, 125);
    char tempStr[10];
    snprintf(tempStr, sizeof(tempStr), "%d", weatherTemp);
    display.print(tempStr);
    display.drawBitmap(160, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    
    // Draw sensor temperature (internal temperature)
    // Using a fixed internal temperature for now (could be from an internal sensor)
    int8_t sensorTemp = 22; // Default room temperature
    display.setCursor(120, 150);
    snprintf(tempStr, sizeof(tempStr), "%d", sensorTemp);
    display.print(tempStr);
    display.drawBitmap(160, 135, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    
    // Draw small indicators for weather/sensor using regular font
    display.setFont(&Seven_Segment10pt7b);
    display.setCursor(105, 125);
    display.print("W");  // W for Weather
    display.setCursor(105, 150);
    display.print("S");  // S for Sensor
    
    // Draw weather icon
    const unsigned char* weatherIcon;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;
    
    if(weatherConditionCode > 801){
        weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){
        weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){
        weatherIcon = sunny;
    }else if(weatherConditionCode >=700){
        weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){
        weatherIcon = snow;
    }else if(weatherConditionCode >=500){
        weatherIcon = rain;
    }else if(weatherConditionCode >=300){
        weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){
        weatherIcon = thunderstorm;
    }else
        return;
    
    display.drawBitmap(145, 160, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void WatchyNachi::drawWeather(){
    // This method is not used in Nachi, replaced by drawDualTemperature
}