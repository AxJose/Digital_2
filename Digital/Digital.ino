// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch

#define LEDB 19

uint8_t estadoLedAzul = 0;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;


// set up the feeda
AdafruitIO_Feed *feedLedAzul = io.feed("led");

AdafruitIO_Feed *feedColor = io.feed("color");

AdafruitIO_Feed *feedDetector = io.feed("detector");

AdafruitIO_Feed *feedServo = io.feed("servo");

AdafruitIO_Feed *feedMotor = io.feed("motor");

void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  feedLedAzul->onMessage(handleMessageLedAzul);
  feedServo->onMessage(handleMessageServo);
  feedMotor->onMessage(handleMessageMotor);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(2, HIGH);
  // we are connected

  Serial.println();
  Serial.println(io.statusText());
  feedLedAzul->get();
  feedServo->get();
  feedMotor->get();
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run(); 
  if  (estadoLedAzul == 1){
    digitalWrite(LEDB, HIGH);
  }else {
    digitalWrite(LEDB, LOW);
  }
  

  

  if (millis() - lastUpdate > IO_LOOP_DELAY) {

    
    // Verificar si hay datos reales
    if (Serial2.available()) {

        // Vaciar buffer antes de leer
        
        int inByte = Serial2.read();  // Leer un byte válido
        /*
        if ((inByte = 'B') | (inByte = 'R')){
          Serial.print("Dato recibido: ");
          Serial.println(inByte);
          feedColor->save(inByte); // Enviar a Adafruit
        }else if (inByte == 97 | inByte == 98){
          Serial.print("Dato recibido: ");
          Serial.println(inByte);
          feedDetector->save(inByte); // Enviar a Adafruit
        }
        */
        while (Serial2.available()) { Serial2.read(); }
        }

    lastUpdate = millis(); // Actualizar el tiempo
  }
}
// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessageLedAzul(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());

  if (atoi(data->value()) == 1){
    
    estadoLedAzul = 1;
    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(1);
  }else if (atoi(data->value()) == 0){

    estadoLedAzul = 0;
    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(0);
  }


}

void handleMessageServo(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());

  if (atoi(data->value()) == 4){

    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(4);
  }else if (atoi(data->value()) == 6){

    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(6);
  }else if (atoi(data->value()) == 8){

    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(8);
  }

}

void handleMessageMotor(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->value());

  if (atoi(data->value()) == 2){

    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(2);
  }else if (atoi(data->value()) == 3){

    // Vaciar buffer antes de escribir
    while (Serial2.available()) { Serial2.read(); }
    delay(10);
    Serial2.write(3);
  }
}