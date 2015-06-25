#include "segheader.h"


// soft serial pins 
const int dataPin = 11;
const int clockPin = 12;
const int latchPin = 8; 
const int outputenablePin = 5; 
const int resetButtonPin = 2; 
const int stopButtonPin = 3; 
const int startButtonPin = 4; 
boolean resetButtonState = true; 
boolean stopButtonState = true; 
boolean startButtonState = true; 
boolean stopped = false; 
volatile boolean time_updated = true;

// character definitions for 7segs 


// for sending big numbers
const int numberOfDigits = 6; 



int sendTime(segTime time) {
  
    digitalWrite(outputenablePin, HIGH); 
    if (time.leading_zeroes) { 
      
//          sendDigit(digits[time.sss%10]);
//              sendDigit(digits[time.sss/10]);
          sendDigit(blank);
              sendDigit(blank);
        sendDigit(digits[time.secs%10]);
         sendDigit(digits[time.secs/10]);
       sendDigit(digits[time.mins%10]);
      sendDigit(digits[time.mins/10]);
      sendDigit(digits[time.hours%10]); 
       sendDigit(digits[time.hours/10]);


  } else {
  //  sendDigit(digits[time.hours/10]);
  //  sendDigit(digits[time.hours%10]); 
    // handle leading zeroes. 
    
    
  } 
  digitalWrite(outputenablePin, HIGH); 

  
  
  
} 


byte i = 0; 
// numbers 

segTime the_time; 

void setup() {
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 
  pinMode(latchPin, OUTPUT); 
  pinMode(outputenablePin, OUTPUT); 
  pinMode(resetButtonPin, INPUT_PULLUP); 
  pinMode(stopButtonPin, INPUT_PULLUP);
  pinMode(startButtonPin, INPUT_PULLUP);
  
  the_time.hours = 00; 
  the_time.mins = 00; 
  the_time.secs = 00;
  the_time.sss = 0; 
  the_time.leading_zeroes = true; 
  
  
  // init timer 1 
  
  
//  // initialize timer1 
  noInterrupts();           // disable all interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    OCR1A = 15624;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
  interrupts();             // enable all interrupts
  
  Serial.begin(115200); 
}


 

int sendDigit(byte digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}




int advanceTime(segTime *time, byte mode) {
  
  switch (mode) {
  case 0: //hours
    time->hours++; 
    break; 
  case 1: //mins
    time->mins++;
    break;
  case 2: //secs
    time->secs++; 
    break;
  case 3: // sss
    time->sss++; 
    break; 
  }
    
    if (time->sss > 99) {
      time->sss = 0; 
      time->secs++; 
    }
    
    if (time->secs > 59) {
      time->secs = 0; 
      time->mins++; 
    }
    
    if (time->mins > 59) {
      time->mins = 0;
      time->hours++;
    }
  
  // no (real) hour overflow. 
    if (time->hours > 99) time->hours = 0;  
  
  }


ISR(TIMER1_COMPA_vect)        
{
             advanceTime(&the_time, 2);

      time_updated = true;

 

}

void sendNumber(int number) {
  
  for (int i = 0; i < numberOfDigits; i++) {
  sendDigit(digits[number%10]);
  number /= 10; 
  }
  
} 

void timerStop() {
  TCCR1B = 0; 
}

void timerContinue() {
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
    //advanceTime(&the_time, 3); 
//
    if (time_updated) {
      time_updated = false;
      sendTime(the_time); 
      
      
      Serial.println("Button statuses: (2 3 4) ");
      Serial.print(startButtonState); 
      Serial.print(stopButtonState); 
      Serial.println(resetButtonState); 
    }
    
    stopButtonState = digitalRead(stopButtonPin); 

    if (!stopButtonState) {
      stopped = true; 
       timerStop(); 
    }

    if (stopped) {
      resetButtonState = digitalRead(resetButtonPin); 
      if (!resetButtonState) {
      the_time.hours = 0; 
      the_time.mins = 0;
      the_time.secs = 0;
      the_time.sss = 0;
      TCNT1 = 0; 
      time_updated = true; 
      }
      
      startButtonState = digitalRead(startButtonPin);
      if (!startButtonState) {
        timerContinue();
        stopped = false; 
      }    
    }

    
    // poll button othrwise. 
    
    

//    sendTime(the_time);
   //sendNumber(analogRead(A0));  
   // delay(200); 
    


}
