#include "Arduino.h"


////////////////////////////////

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
//int count=0;

void setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
pinMode(LEDPIN, OUTPUT);

//////////////  INPUTS SCALING - Start
 
//while(not(Serial.available())){digitalWrite(LED_BUILTIN, HIGH);delay(100);digitalWrite(LED_BUILTIN, LOW);delay(100);}
//Serial.read();
//
//digitalWrite(LED_BUILTIN, LOW);

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

//////////////  INPUTS SCALING - End
}

int control = 0;
char mssg;

void loop()
{ 
           
  float a,b,c,d;
   
   if (Serial.available() > 0)
   {
        if (control == 0) mssg = Serial.read(); //leemos el serial solo el primer paso
        control++;
    
        if(mssg == 'c')
        {
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
            
            /////// SEND DATA
            
            Serial.print(String(constrain(a_ave/promedio, 0, 1))+','+String(constrain(b_ave/promedio, 0, 1))+','+String(constrain(c_ave/promedio, 0, 1))+','+String(constrain(d_ave/promedio, 0, 1))+'\n');

        }
        else
        {
           digitalWrite(13, LOW); //si entra una 'a' apagamos el LED
           delay(200);
        }
   }
}
