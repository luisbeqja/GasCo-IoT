/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled 2"
  https://create.arduino.cc/cloud/things/507a225b-0709-4317-997b-ba8ad0afe320

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float sensore1;
  float temperatura;
  float sensore3;
  bool inizioLavoro;
  float sensore2;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

float sensore;
String lavoratore;

//Controllo sensore
float PrecX, PrecY, PrecZ = 0; //dichiaro la misurazione precedente dell giroscopi
float x, y, z;                 //dichiaro la misurazione letta dell giroscopi
int i = 0;                     //serve per il numero di misurazioni
float accelerometro;

const int Led_sensore2 = 7; //LED COLLEGATO SUL D9 E SUL GND
const int Led_sensore1 = 8; //LED COLLEGATO SUL D9 E SUL GND
const int Led = 9;          //LED COLLEGATO SUL D9 E SUL GND

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(9600);

    // Defined in thingProperties.h
    initProperties();

    // Connect to Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    //Get Cloud Info/errors , 0 (only errors) up to 4
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();

    //Wait to get cloud connection to init the carrier
    while (ArduinoCloud.connected() != 1)
    {
        ArduinoCloud.update();
        delay(500);
    }
    delay(500);
    CARRIER_CASE = false;
    carrier.begin();
    carrier.display.setRotation(0);
    delay(1500);

    pinMode(Led_sensore1, OUTPUT);
    pinMode(Led_sensore2, OUTPUT);
    pinMode(Led, OUTPUT);

    digitalWrite(Led, LOW);
    digitalWrite(Led_sensore1, LOW);
    digitalWrite(Led_sensore2, LOW);
}

void loop()
{
    // Your code here
    ArduinoCloud.update();
    carrier.Buttons.update();

    sensore1 = analogRead(1);
    //Serial.println(sensore1);

    sensore2 = analogRead(2);
    //Serial.println(sensore2);

    sensore3 = analogRead(3);
    //Serial.println(sensore3);

    //
    //
    //
    //
    accelerometro = carrier.IMUmodule.readAcceleration(x, y, z);
    //Serial.println(x);
    //Serial.println(y);
    //Serial.println(z);

    if (abs(x - PrecX) < 0.3 && abs(y - PrecY) < 0.3 && abs(z - PrecZ) < 0.3)
    {
        i++;
        if (i >= 50 && inizioLavoro == 1)
        {
            carrier.Buzzer.sound(440);
            Serial.println(i);
        }
    }
    else
    {
        i = 0;
        carrier.Buzzer.noSound();
        Serial.println(inizioLavoro);
        Serial.println(i);
    }
    PrecX = x;
    PrecY = y;
    PrecZ = z;

    //led 1
    if (sensore1 > 450)
    {
        digitalWrite(Led, HIGH);
    }
    else
    {
        digitalWrite(Led, LOW);
    }

    //led 2
    if (sensore2 > 450)
    {
        digitalWrite(Led_sensore2, HIGH);
    }
    else
    {
        digitalWrite(Led_sensore2, LOW);
    }

    //led 2
    if (sensore3 > 450)
    {
        digitalWrite(Led_sensore1, HIGH);
    }
    else
    {
        digitalWrite(Led_sensore1, LOW);
    }

    temperatura = carrier.Env.readTemperature();

    delay(100);
};

void onInizioLavoroChange()
{
    if (inizioLavoro == 1)
    {
        lavoratore = "Luis";
    }
    else
    {
        lavoratore = " ";
        carrier.Buzzer.noSound();
    }
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setTextColor(ST77XX_WHITE); //white text
    carrier.display.setTextSize(6);             //medium sized text
    carrier.display.setCursor(40, 80);
    carrier.display.print(lavoratore);
    Serial.println(inizioLavoro);
    // Do something
}