/*TRIAC control with potentiometer; author: ELECTRONOOBS 
 * Subscribe: http://www.youtube.com/c/ELECTRONOOBS
 * Tutorial: http://www.ELECTRONOOBS.com/eng_circuitos_tut20.php
 * Thank you
 * @edit: this is a modified version to be compatible with esp32
*/

//edit to suit your board
#define OPTO_PIN 19
#define DIAC_PIN 2
#define POT_PIN 34
#define ADC_MAX 4096

int detectado = 0;
int valor=0;
int last_CH1_state = 0; 

void setup() {
  /*
   * Port registers allow for lower-level and faster manipulation of the i/o pins of the microcontroller on an Arduino board. 
   * The chips used on the Arduino board (the ATmega8 and ATmega168) have three ports:
     -B (digital pin 8 to 13)
     -C (analog input pins)
     -D (digital pins 0 to 7)   
  //All Arduino (Atmega) digital pins are inputs when you begin...
  */  
  pinMode(DIAC_PIN,OUTPUT);        //Define output for the DIAC pulse

  pinMode(OPTO_PIN, INPUT_PULLUP);
  attachInterrupt(OPTO_PIN, ISR, CHANGE);
  
  //Serial.begin(9600);
}

void loop() {
   //Read the value of the pot and map it from 10 to 10.000 us. AC frequency is 50Hz, so period is 20ms. We want to control the power
   //of each half period, so the maximum is 10ms or 10.000us. In my case I've maped it up to 7.200us since 10.000 was too much
   //float tmp = 1<<10;
   valor = map(analogRead(POT_PIN),0,ADC_MAX,10000,10);
//   Serial.println(valor);
   //In my case I've used valor = map(analogRead(A0),0,1024,7200,10); for better results
    if (detectado)
    {
      delayMicroseconds(valor); //This delay controls the power
      digitalWrite(DIAC_PIN,HIGH);
      delayMicroseconds(100);
      digitalWrite(DIAC_PIN,LOW);
      
      detectado=0;
    } 
}




//This is the interruption routine
//----------------------------------------------

void ISR(){
  /////////////////////////////////////               //Input from optocoupler
  if(digitalRead(OPTO_PIN)){                           //We make an AND with the pin state register, We verify if pin 8 is HIGH???
    if(last_CH1_state == 0){                          //If the last state was 0, then we have a state change...
      detectado=1;                                    //We haev detected a state change!
    }
  }
  
  else if(last_CH1_state == 1){                       //If pin 8 is LOW and the last state was HIGH then we have a state change      
    detectado=1;                                      //We haev detected a state change!
    last_CH1_state = 0;                               //Store the current state into the last state for the next loop
    }
}
