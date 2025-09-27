#ifdef SENSORS
#include "SCD4x.h"
#include "globals.h"
#include "mqtt.h"
#include "defaults.h"
#include <HeadlessWiFiSettings.h>
#include "string_utils.h"

#include <M5UnitSCD4x.h>
//#include <M5UnitENV.h>
//#include <M5UnitUnifiedENV.h>

namespace SCD4x
{
    SCD4x scd4x;
    long SCD4x_status;
    String SCD4x_I2c;
    int SCD4x_I2c_Bus;

    int sensorInterval = 5000;  // SCD40/41 are designed to operate at 0.2Hz: so pull every five seconds
    int reportInterval = 60000; // Report every minute to MQTT (to avoid flooding)
    bool initialized = false;

    unsigned long SCD4xPreviousSensorMillis = 0;
    unsigned long SCD4xPreviousReportMillis = 0;

    void Setup() {
        Serial.println("starting scd41 setup");
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
        Serial.println("bus check complete");

//        scd4x = new I2cScd4x();
        scd4x = new M5UnitSCD4x();
        if (SCD4x_I2c == "0x62") {
            scd4x->begin(SCD4x_I2c_Bus == 1 ? Wire : Wire1, SCD41_I2C_ADDR_62);
        } else {
            return;
        }

        if (!scd4x.begin(&Wire, SCD4x_I2C_ADDR, 2, 1, 400000U)) {
            Serial.println("Couldn't find SCD4x");
            while (1) delay(1);
        }

        uint16_t error;
        // stop potentially previously started measurement
        error = scd4x.stopPeriodicMeasurement();
        if (error) {
            Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        }

        // Start Measurement
        error = scd4x.startPeriodicMeasurement();
        if (error) {
            Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        }

        Serial.println("Waiting for first measurement... (5 sec)");
    }







        
//        if (!bmp.begin(&Wire, BMP280m5_I2C_ADDR, 2, 1, 400000U)) {
//            Serial.println("[SCD41m5 SCD41] Couldn't find a sensor, check your wiring and I2C address!");
//            initialized = false;
//        } else {
//            initialized = true;
//        }

        /* Default settings from datasheet. */
//        bmp.setSampling(BMP280m5::MODE_NORMAL,     /* Operating Mode. */
//                        BMP280m5::SAMPLING_X2,     /* Temp. oversampling */
//                        BMP280m5::SAMPLING_X16,    /* Pressure oversampling */
//                       BMP280m5::FILTER_X16,      /* Filtering. */
//                        BMP280m5::STANDBY_MS_500); /* Standby time. */

//        if (!sht.begin(&Wire, SHT40_I2C_ADDR_44, 2, 1, 400000U)) {
//           Serial.println("[ENVIV SHT40]  Couldn't find SHT40, check your wiring and I2C address!");
//            initializedsht = false;
//        } else {
//            initializedsht = true;
//        }

//        sht.setPrecision(SHT4X_HIGH_PRECISION);
//        sht.setHeater(SHT4X_NO_HEATER);
//    }

    void ConnectToWifi()
    {
        Serial.println("scd4x connect to wifi");
        SCD4x_I2c_Bus = HeadlessWiFiSettings.integer("SCD4x_I2c_Bus", 1, 2, DEFAULT_I2C_BUS, "I2C Bus");
        SCD4x_I2c = HeadlessWiFiSettings.string("SCD4x_I2c", "", "I2C address (0x62)");
        
        //Serial.println("env IV connect to wifi");
//        ENVIVBMP280_I2c_Bus = HeadlessWiFiSettings.integer("ENVIVBMP280_I2c_Bus", 1, 2, DEFAULT_I2C_BUS, "I2C Bus");
//        ENVIVBMP280_I2c = HeadlessWiFiSettings.string("ENVIVBMP280_I2c", "", "I2C address (0x76 or 0x77)");
    }

    void SerialReport()
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
        if (SCD4x_I2c.isEmpty()) return;
        Serial.print("SCD4x:       ");
        Serial.println(SCD4x_I2c + " on bus " + SCD4x_I2c_Bus);

        
        //if (BMP280m5_I2c.isEmpty()) return;
//        Serial.print("ENVIV BMP280:       ");
//        Serial.println(ENVIVBMP280_I2c + " on bus " + ENVIVBMP280_I2c_Bus);
    }

    void Loop()
    {
        if (!I2C_Bus_1_Started && !I2C_Bus_2_Started) return;
//        if (!initialized) return;
        
        // Changing to only return if both sensors aren't working.  If one of the sensors is working, it will publish to that topic and ignore the nonworking sensor.
        //if (!initialized) return;
        //if (!initializedsht) return;
//        if (!initialized && !initializedsht) return;

        if (SCD4xPreviousSensorMillis == 0 || millis() - SCD4xPreviousSensorMillis >= sensorInterval) {
            SCD4xPreviousSensorMillis = millis();

            bool dataReady = false;
            uint16_t co2Concentration = 0;
            float humidity;
            float temperature;
            SCD4x_status = scd4x->getDataReadyStatus(dataReady);
            if (SCD4x_status != NO_ERROR) {
                 return;
            }
    
            if (!dataReady) {
                return; // Data not ready yet
            }
    
            SCD4x_status = scd4x->readMeasurement(co2Concentration, temperature, humidity);
            if (SCD4x_status != NO_ERROR) {
                return;
            }
    
            if (SCD4xPreviousSensorMillis > 30000) {  // First 30 seconds after boot, don't report
                if (SCD4xPreviousReportMillis == 0 || millis() - SCD4xPreviousReportMillis >= reportInterval) {
                    SCD4xPreviousReportMillis = millis();
    
                    pub((roomsTopic + "/co2").c_str(), 0, 1, String(co2Concentration).c_str());
                    pub((roomsTopic + "/humidity").c_str(), 0, 1, String(humidity).c_str());
                    pub((roomsTopic + "/temperature").c_str(), 0, 1, String(temperature).c_str());
                }
            }
        }


        
        if (scd4x.update())  // readMeasurement will return true when
                             // fresh data is available
        {
            Serial.println();
    
            Serial.print(F("CO2(ppm):"));
            Serial.print(scd4x.getCO2());
    
            Serial.print(F("\tTemperature(C):"));
            Serial.print(scd4x.getTemperature(), 1);
    
            Serial.print(F("\tHumidity(%RH):"));
            Serial.print(scd4x.getHumidity(), 1);
    
            Serial.println();
        } else {
            Serial.print(F("."));
        }
    
        delay(1000);
    }

    bool SendDiscovery()
    {
        Serial.println("scd4x send discovery");
        if (SCD4x_I2c.isEmpty()) return true;
        return sendSensorDiscovery("Co2", EC_NONE, "carbon_dioxide", "ppm") && sendSensorDiscovery("Temperature", EC_NONE, "temperature", "°C") && sendSensorDiscovery("Humidity", EC_NONE, "humidity", "%");
    }
        
}

#endif
