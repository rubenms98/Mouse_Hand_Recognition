#include "Arduino.h"
/* For the bluetooth funcionality */
#include <ArduinoBLE.h>
/* For the use of the IMU sensor */
#include "Nano33BLEAccelerometer.h"

#define LED 13

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * We use strings to transmit the data via BLE, and this defines the buffer
 * size used to transmit these strings. Only 20 bytes of data can be 
 * transmitted in one packet with BLE, so a size of 20 is chosen the the data 
 * can be displayed nicely in whatever application we are using to monitor the
 * data.
 */
#define BLE_BUFFER_SIZES             20
/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME                "Arduino Nano 33 BLE Sense"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "Accelerometer BLE"

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* 
 * Nano33BLEAccelerometerData object which we will store data in each time we read
 * the accelerometer data. 
 */ 
Nano33BLEAccelerometerData accelerometerData;

/* 
 * Declares the BLEService and characteristics we will need for the BLE 
 * transfer. The UUID was randomly generated using one of the many online 
 * tools that exist. It was chosen to use BLECharacteristic instead of 
 * BLEFloatCharacteristic was it is hard to view float data in most BLE 
 * scanning software. Strings can be viewed easiler enough. In an actual
 * application you might want to transfer floats directly.
 */
BLEService BLEAccelerometer("590d65c7-3a0a-4023-a05a-6aaf2f22441c");
BLECharacteristic accelerometerXBLE("0004", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerYBLE("0005", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerZBLE("0006", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

/* Common global buffer will be used to write to the BLE characteristics. */
char bleBuffer[BLE_BUFFER_SIZES];
 
char mssg; //variable para guardar el mensaje
  
void setup()
{
   pinMode(LED, OUTPUT); //establecemos el pin D13 como salida
   Serial.begin(115200); //inicializamos Serial

     while(!Serial);


    /* BLE Setup. For information, search for the many ArduinoBLE examples.*/
    if (!BLE.begin()) 
    {
        while (1);    
    }
    else
    {
        BLE.setDeviceName(BLE_DEVICE_NAME);
        BLE.setLocalName(BLE_LOCAL_NAME);
        BLE.setAdvertisedService(BLEAccelerometer);
        /* A seperate characteristic is used for each X, Y, and Z axis. */
        BLEAccelerometer.addCharacteristic(accelerometerXBLE);
        BLEAccelerometer.addCharacteristic(accelerometerYBLE);
        BLEAccelerometer.addCharacteristic(accelerometerZBLE);

        BLE.addService(BLEAccelerometer);
        BLE.advertise();
        /* 
         * Initialises the IMU sensor, and starts the periodic reading of the 
         * sensor using a Mbed OS thread. The data is placed in a circular 
         * buffer and can be read whenever.
         */
        Accelerometer.begin();
        /* Plots the legend on Serial Plotter */
        //Serial.println("X, Y, Z");
    }
    
}

int control = 0;


void loop()
{ 
           
   int writeLength;
   
   if (Serial.available() > 0)
   {
        if (control == 0) mssg = Serial.read(); //leemos el serial solo el primer paso
        control++;
    
        if(mssg == 'c')
        {
  
           if(Accelerometer.pop(accelerometerData))
           {
               /* 
                * sprintf is used to convert the read float value to a string 
                * which is stored in bleBuffer. This string is then written to 
                * the BLE characteristic. 
                */
                   
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.x);
               accelerometerXBLE.writeValue(bleBuffer, writeLength); 
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.y);
               accelerometerYBLE.writeValue(bleBuffer, writeLength);      
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.z);
               accelerometerZBLE.writeValue(bleBuffer, writeLength);      
  
               Serial.printf("%f,%f,%f\n", accelerometerData.x,accelerometerData.y,accelerometerData.z);
               //delay(300);
           }
           
        }
        else
        {
           digitalWrite(13, LOW); //si entra una 'a' apagamos el LED
           delay(200);
        }
   }
}
