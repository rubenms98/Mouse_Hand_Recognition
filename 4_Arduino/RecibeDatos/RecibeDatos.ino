/*
  This example creates a BLE central that scans for a peripheral with a Test Service
  If that contains floatValue characteristics the value can be seen in the Serial Monitor or Plotter.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 IoT board.

  This example code is in the public domain.
*/
#include "Arduino.h"
#include <ArduinoBLE.h> 


//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FLOAT_1                  "C8F88594-2217-0CA6-8F06-A4270B675D69"
#define BLE_UUID_FLOAT_2                  "E3ADBF53-950E-DC1D-9B44-076BE52760D6"
#define BLE_UUID_FLOAT_3                  "0a29843e-acf2-11eb-8529-0242ac130003"
#define BLE_UUID_FLOAT_4                  "0a298808-acf2-11eb-8529-0242ac130003"
#define BLE_UUID_FLOAT_5                  "0a298970-acf2-11eb-8529-0242ac130003"
#define BLE_UUID_FLOAT_6                  "8b7db4a4-cc34-11eb-b8bc-0242ac130003"
#define BLE_UUID_FLOAT_7                  "8b7dba94-cc34-11eb-b8bc-0242ac130003"
#define BLE_UUID_FLOAT_8                  "8b7dbc1a-cc34-11eb-b8bc-0242ac130003"
#define BLE_UUID_FLOAT_9                  "8b7dbcec-cc34-11eb-b8bc-0242ac130003"

void setup()
{
  Serial.begin( 115600 );
  while ( !Serial );

  BLE.begin();

  BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
}


void loop()
{
  BLEDevice peripheral = BLE.available();

  if ( peripheral )
  {
    if ( peripheral.localName() != "Arduino Nano 33 BLE" )
    {
      return;
    }

    BLE.stopScan();

    connectPeripheral( peripheral );

    BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
  }
}


bool connectPeripheral( BLEDevice peripheral )
{
  int buttonState = 'c';
  
  if ( !peripheral.connect() )
  {
    return false;
  }

  if ( !peripheral.discoverAttributes() )
  {
    peripheral.disconnect();
    return false;
  }

  BLECharacteristic Value_1_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_1 );
  BLECharacteristic Value_2_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_2 );
  BLECharacteristic Value_3_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_3 );
  BLECharacteristic Value_4_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_4 );
  BLECharacteristic Value_5_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_5 );
  BLECharacteristic Value_6_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_6 );
  BLECharacteristic Value_7_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_7 );
  BLECharacteristic Value_8_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_8 );
  BLECharacteristic Value_9_Characteristic = peripheral.characteristic( BLE_UUID_FLOAT_9 );


if ( !Value_1_Characteristic || !Value_2_Characteristic || !Value_3_Characteristic || !Value_4_Characteristic || !Value_3_Characteristic )
  {
    peripheral.disconnect();
    return false;
  }

  if ( !Value_1_Characteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  if ( !Value_1_Characteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  while ( peripheral.connected() )
  {
    if ( Value_1_Characteristic.valueUpdated() )
    {
      float floatValue_1; float floatValue_2; float floatValue_3; float floatValue_4; float floatValue_5;
      float floatValue_6; float floatValue_7; float floatValue_8; float floatValue_9;
      Value_1_Characteristic.readValue( &floatValue_1, 4 );
      Value_2_Characteristic.readValue( &floatValue_2, 4 );
      Value_3_Characteristic.readValue( &floatValue_3, 4 );
      Value_4_Characteristic.readValue( &floatValue_4, 4 );
      Value_5_Characteristic.readValue( &floatValue_5, 4 );
      Value_6_Characteristic.readValue( &floatValue_6, 4 );
      Value_7_Characteristic.readValue( &floatValue_7, 4 );
      Value_8_Characteristic.readValue( &floatValue_8, 4 );
      Value_9_Characteristic.readValue( &floatValue_9, 4 );

      //Serial.println( floatValue_1);
      //Serial.println( floatValue_2 );
      //Serial.println( floatValue_3);
      //Serial.println( floatValue_4);
      //Serial.println( floatValue_5);
      Serial.print(floatValue_1);Serial.print(',');Serial.print(floatValue_2);Serial.print(',');
      Serial.print(floatValue_3);Serial.print(',');Serial.print(floatValue_4);Serial.print(',');
      Serial.print(floatValue_5);Serial.print(',');Serial.print(floatValue_6);Serial.print(',');
      Serial.print(floatValue_7);Serial.print(',');Serial.print(floatValue_8);Serial.print(',');
      Serial.print(floatValue_9);Serial.print(',');Serial.print('\n');
    }


  }

  peripheral.disconnect();
  return true;
}
