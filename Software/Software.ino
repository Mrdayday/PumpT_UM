//codeing by Martin Ortner and Jan Stotz
#include <ModbusRtu.h> //Modbus libary (Modbus Master-Slave library for Arduino by smarmengol(Github)
// data Array for Modbus excange
/*
 * pumpdata[0]=Time pump is on in seconds
 */
uint16_t pumpdata[1]={0};
// define Used Pins
// define pin to switch on the pump
#define sourcePin 3
// define pin that is connected to the button for priming the tubes
#define buttonPin 5
// assign the Arduino pin that must be connected to RE-DE RS485 transceiver
#define TXEN  4
// define Slave 2
Modbus slave(2,0,TXEN);
//initialize on_time as 0 so that the pump is not running at the start
unsigned long on_time = 0;

volatile bool switch_on = true;
int run_val;


void setup() {
  slave.begin(19200); // baud-rate at 19200
  // define pintype
  pinMode(sourcePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  }

void loop() {
  //update array info
  Start:slave.poll(pumpdata,1);
  
  if (digitalRead(buttonPin)==1) { //ask if the button is pressed
    digitalWrite(sourcePin, 1);
    goto Start; //go to beginning of the loop
  }
  
  if (pumpdata[0] > 0 && switch_on == true){
    on_time = millis();//get new time
    run_val = pumpdata[0];
    digitalWrite(sourcePin, HIGH);
    switch_on = false;
  }
  if (pumpdata[0] == 0 ){
    digitalWrite(sourcePin, LOW);
    switch_on = true;
    pumpdata[0] = 0;
  }
  //count the second downwards
  if (millis() >= on_time + 1000 && pumpdata[0] != 0){
    on_time = millis();//get new time
    -- pumpdata[0];//reduce the businformation by one
  }
}
