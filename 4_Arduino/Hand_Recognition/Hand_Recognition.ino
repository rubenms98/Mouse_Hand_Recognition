#include "Arduino.h"
#include <ArduinoBLE.h>

//***********************************TENSORFLOW*******************************************************************************
// Import TensorFlow stuff
#include "TensorFlowLite.h" 
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Our model
#include "Hand_Recognition_NN.h"

#define DEBUG 0
constexpr int led_pin = 2;

// TFLite globals, used for compatibility with Arduino-style sketches
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* model_input = nullptr;
  TfLiteTensor* model_output = nullptr;

  // Create an area of memory to use for input, output, and other TensorFlow
  // arrays. You'll need to adjust this by combiling, running, and looking
  // for errors.
  constexpr int kTensorArenaSize = 10 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
} // namespace
//***********************************TENSORFLOW****************************************************************************************

//***********************************************BLUETOOTH****************************************************************************
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

BLEService testService( BLE_UUID_TEST_SERVICE );
BLEFloatCharacteristic Value_1_Characteristic( BLE_UUID_FLOAT_1, BLERead | BLENotify );
BLEFloatCharacteristic Value_2_Characteristic( BLE_UUID_FLOAT_2, BLERead | BLENotify );
BLEFloatCharacteristic Value_3_Characteristic( BLE_UUID_FLOAT_3, BLERead | BLENotify );
BLEFloatCharacteristic Value_4_Characteristic( BLE_UUID_FLOAT_4, BLERead | BLENotify );
BLEFloatCharacteristic Value_5_Characteristic( BLE_UUID_FLOAT_5, BLERead | BLENotify );
BLEFloatCharacteristic Value_6_Characteristic( BLE_UUID_FLOAT_6, BLERead | BLENotify );
BLEFloatCharacteristic Value_7_Characteristic( BLE_UUID_FLOAT_7, BLERead | BLENotify );
BLEFloatCharacteristic Value_8_Characteristic( BLE_UUID_FLOAT_8, BLERead | BLENotify );
BLEFloatCharacteristic Value_9_Characteristic( BLE_UUID_FLOAT_9, BLERead | BLENotify );

float floatValue_1;
float floatValue_2;
float floatValue_3;
float floatValue_4;
float floatValue_5;
float floatValue_6;
float floatValue_7;
float floatValue_8;
float floatValue_9;

//***********************************************BLUETOOTH****************************************************************************

//***********************************ACELEROMETRO*************************************************************************
#include "Nano33BLEAccelerometer.h"

#define BLE_BUFFER_SIZES             20
#define BLE_DEVICE_NAME                "Arduino Nano 33 BLE Sense"
#define BLE_LOCAL_NAME                "Accelerometer BLE"

Nano33BLEAccelerometerData accelerometerData;

BLEService BLEAccelerometer("590d65c7-3a0a-4023-a05a-6aaf2f22441c");
BLECharacteristic accelerometerXBLE("0004", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerYBLE("0005", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic accelerometerZBLE("0006", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

char bleBuffer[BLE_BUFFER_SIZES];
 
char mssg; //variable para guardar el mensaje

#define POSICIONES 1
//***********************************ACELEROMETRO*****************************************************************************************


//////////////////////////////////////GUANTE///////////////////////////////////////////////////////////////

#define LEDPIN 22
#define DELAY 500

int retardo = DELAY;
int promedio = 150;
int promedioAverage = 2000;

float maximo[4] = {0}, minimo[4] = {0};

float a_150[150] = {0};
float b_150[150] = {0};
float c_150[150] = {0};
float d_150[150] = {0};

float a_ave = 0,b_ave = 0,c_ave = 0,d_ave = 0;
///////////////////////////////////GUANTE///////////////////////////////////////////////////////////////////////////////////

void setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(LEDPIN, OUTPUT);

//////////////  INPUTS SCALING - Start ////////////////////////////////////////////////////////////////////////////

/// WARNS THE MAXMIN VALUES ACQUISITION
while(retardo>=20)
  {
  digitalWrite(LEDPIN, LOW);   // turn on the RGB_RED_LED (LOW is the voltage level)
  delay(retardo);                       // wait for a second
  digitalWrite(LEDPIN, HIGH);    // turn off the RGB_RED_LED by making the voltage HIGH
  delay(retardo);                       // BLINKIING TIME
  retardo-=20;
  }

digitalWrite(LEDPIN, LOW);    // 
digitalWrite(LEDPIN+1, LOW);   // YELLOW LIGHT
delay(4*DELAY); 
digitalWrite(LEDPIN, HIGH);    // AND GREEN. MAX VALUE ACQUISITION STARTS


for (int i=0;i<promedioAverage;i++){
  maximo[0] += analogRead(7); maximo[1] += analogRead(6); maximo[2] += analogRead(5); maximo[3] += analogRead(4);
}
maximo[0] /= promedioAverage; maximo[1] /= promedioAverage; maximo[2] /= promedioAverage; maximo[3] /= promedioAverage;



//while(not(Serial.available())){digitalWrite(LED_BUILTIN, HIGH);delay(500);digitalWrite(LED_BUILTIN, LOW);delay(500);}
//Serial.read();
//
//digitalWrite(LED_BUILTIN, LOW);


digitalWrite(LEDPIN, HIGH);    // 
digitalWrite(LEDPIN+1, HIGH);   // 
digitalWrite(LEDPIN+2, LOW);   // BLUE, MIN VALUE ACQUISITION WARNING 
delay(4000); 
digitalWrite(LEDPIN+2, HIGH);    // 
digitalWrite(LEDPIN+1, LOW);    // AND GREEN. ACQUISITION STARTS

for (int i=0;i<promedioAverage;i++){
  minimo[0] += analogRead(7); minimo[1] += analogRead(6); minimo[2] += analogRead(5); minimo[3] += analogRead(4);
}
minimo[0] /= promedioAverage; minimo[1] /= promedioAverage; minimo[2] /= promedioAverage; minimo[3] /= promedioAverage;

digitalWrite(LEDPIN+1, HIGH);

//while(not(Serial.available()));
//Serial.read();

digitalWrite(LED_BUILTIN, HIGH);

//////////////  INPUTS SCALING - End  /////////////////////////////////////////////////////////////////////////////////////////////////

//***********************************TENSORFLOW*******************************************
  // Wait for Serial to connect
  #if DEBUG
    while(!Serial);
  #endif

  Serial.begin(115600); //inicializamos Serial

  // Let's make an LED vary in brightness
  pinMode(led_pin, OUTPUT);

  // Set up logging (will report to Serial, even within TFLite functions)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure
  model = tflite::GetModel(Hand_Recognition_NN_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report("Model version does not match Schema");
    while(1);
  }

  tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize,
    error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    while(1);
  }

  // Assign model input and output buffers (tensors) to pointers
  model_input = interpreter->input(0);
  model_output = interpreter->output(0);

  // Get information about the memory area to use for the model's input
  // Supported data types:
  // https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/c/common.h#L226
  
  #if DEBUG
    Serial.print("CARACERISTICAS DEL TENSOR  DE INPUT \n");
    Serial.print("Number of dimensions: ");
    Serial.println(model_input->dims->size);
    Serial.print("Dim 1 size: ");
    Serial.println(model_input->dims->data[0]);
    Serial.print("Dim 2 size: ");
    Serial.println(model_input->dims->data[1]);
    Serial.print("Input type: ");
    Serial.println(model_input->type);
  #endif
//***********************************TENSORFLOW*********************************************

//***********************************ACELEROMETRO*******************************************
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
        #if DEBUG
          Serial.println("X, Y, Z");
        #endif
    }
//***********************************ACELEROMETRO*******************************************

//***********************************BLUETOOTH*********************************************
  BLE.begin();

  // set advertised local name and service
  BLE.setDeviceName( "Arduino Nano 33 BLE" );
  BLE.setLocalName( "Arduino Nano 33 BLE" );
  BLE.setAdvertisedService( testService );

  // BLE add characteristics
  testService.addCharacteristic( Value_1_Characteristic );
  testService.addCharacteristic( Value_2_Characteristic );
  testService.addCharacteristic( Value_3_Characteristic );
  testService.addCharacteristic( Value_4_Characteristic );
  testService.addCharacteristic( Value_5_Characteristic );
  testService.addCharacteristic( Value_6_Characteristic );
  testService.addCharacteristic( Value_7_Characteristic );
  testService.addCharacteristic( Value_8_Characteristic );
  testService.addCharacteristic( Value_9_Characteristic );
  

  // add service
  BLE.addService( testService );

  // set the initial value for characeristics
  Value_1_Characteristic.writeValue( floatValue_1 );
  Value_2_Characteristic.writeValue( floatValue_2 );
  Value_3_Characteristic.writeValue( floatValue_3 );
  Value_4_Characteristic.writeValue( floatValue_4 );
  Value_5_Characteristic.writeValue( floatValue_5 );
  Value_6_Characteristic.writeValue( floatValue_6 );
  Value_7_Characteristic.writeValue( floatValue_7 );
  Value_8_Characteristic.writeValue( floatValue_8 );
  Value_9_Characteristic.writeValue( floatValue_9 );
  

  BLE.advertise();
//*************************************BLUETOOTH*********************************************************
}

int control = 0;
float output[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void loop()
{ 

  static long previousMillis = 0;

  BLEDevice central = BLE.central();
           
  float a,b,c,d;
  int writeLength;
  
   //if (Serial.available() > 0)
   //{

    if(central){
      while(central.connected()){
        if (control == 0) Serial.print("Empezamos\n"); //leemos el serial solo el primer paso
          control++;
          Serial.print("flag1\n");
          if(Accelerometer.pop(accelerometerData))
           {
               /* 
                * sprintf is used to convert the read float value to a string 
                * which is stored in bleBuffer. This string is then written to 
                * the BLE characteristic. 
                */
                   
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.x);
               accelerometerXBLE.writeValue(bleBuffer, &writeLength); 
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.y);
               accelerometerYBLE.writeValue(bleBuffer, &writeLength);      
               writeLength = sprintf(bleBuffer, "%f", accelerometerData.z);
               accelerometerZBLE.writeValue(bleBuffer, &writeLength);   

               #if POSICIONES
                // Serial.printf("%f,%f,%f\n", accelerometerData.x,accelerometerData.y,accelerometerData.z);
               #endif

               a = (analogRead(7)-minimo[0])/(maximo[0]-minimo[0]);
               b = (analogRead(6)-minimo[1])/(maximo[1]-minimo[1]);
               c = (analogRead(5)-minimo[2])/(maximo[2]-minimo[2]);
               d = (analogRead(4)-minimo[3])/(maximo[3]-minimo[3]);
              
               a_ave = a_ave - a_150[0] + a;
               b_ave = b_ave - b_150[0] + b;
               c_ave = c_ave - c_150[0] + c;
               d_ave = d_ave - d_150[0] + d;
            
               for (int i=0;i<promedio-1;i++){
                 a_150[i] = a_150[i+1];
                 b_150[i] = b_150[i+1];
                 c_150[i] = c_150[i+1];
                 d_150[i] = d_150[i+1];
               }
            
               a_150[promedio-1] = a; b_150[promedio-1] = b; c_150[promedio-1] = c; d_150[promedio-1] = d;

               model_input->data.f[0] =  accelerometerData.x;
               model_input->data.f[1] =  accelerometerData.y;
               model_input->data.f[2] =  accelerometerData.z;
               model_input->data.f[3] =  a_ave/promedio;
               model_input->data.f[4] =  b_ave/promedio;
               model_input->data.f[5] =  c_ave/promedio;
               model_input->data.f[6] =  d_ave/promedio;
   

               // Run inference
               TfLiteStatus invoke_status = interpreter->Invoke();
               if (invoke_status != kTfLiteOk) {
                 error_reporter->Report("Invoke failed on input: %f\n", accelerometerData.x);
                 error_reporter->Report("Invoke failed on input: %f\n", accelerometerData.y);
                 error_reporter->Report("Invoke failed on input: %f\n", accelerometerData.z);
                 error_reporter->Report("Invoke failed on input: %f\n", a_150);
                 error_reporter->Report("Invoke failed on input: %f\n", b_150);
                 error_reporter->Report("Invoke failed on input: %f\n", c_150);
                 error_reporter->Report("Invoke failed on input: %f\n", d_150);
               }

               // Read predicted y value from output buffer (tensor)
               output[0] = model_output->data.f[0];
               output[1] = model_output->data.f[1];
               output[2] = model_output->data.f[2];
               output[3] = model_output->data.f[3];
               output[4] = model_output->data.f[4];
               output[5] = model_output->data.f[5];
               output[6] = model_output->data.f[6];
               output[7] = model_output->data.f[7];
               output[8] = model_output->data.f[8];

               floatValue_1 = output[0];
               floatValue_2 = output[1];
               floatValue_3 = output[2];
               floatValue_4 = output[3];
               floatValue_5 = output[4];
               floatValue_6 = output[5];
               floatValue_7 = output[6];
               floatValue_8 = output[7];
               floatValue_9 = output[8];

               long interval = 80;
               unsigned long currentMillis = millis();
               if ( currentMillis - previousMillis > interval )
               {
                   previousMillis = currentMillis;
                   if ( central.rssi() != 0 )
                    {
                        Value_1_Characteristic.writeValue( floatValue_1 );
                        Value_2_Characteristic.writeValue( floatValue_2 );
                        Value_3_Characteristic.writeValue( floatValue_3 );
                        Value_4_Characteristic.writeValue( floatValue_4 );
                        Value_5_Characteristic.writeValue( floatValue_5 );
                        Value_6_Characteristic.writeValue( floatValue_6 );
                        Value_7_Characteristic.writeValue( floatValue_7 );
                        Value_8_Characteristic.writeValue( floatValue_8 );
                        Value_9_Characteristic.writeValue( floatValue_9 );
                    }
              }

               //Serial.printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\n",output[0],output[1],output[2], output[3],output[4],output[5], output[6],output[7],output[8]);
               //Serial.printf("%f,%f,%f,%f\n",output[5], output[6],output[7],output[8]);
               //Serial.printf("\n");
               

               #if DEBUG
                 Serial.print("Time for inference (us): ");
                // Serial.println(micros() - start_timestamp);
               #endif      
           }
        }
      //}
   }
} 
