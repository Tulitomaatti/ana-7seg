
#include <Arduino.h>

typedef struct {
  byte hours;
  byte mins; 
  byte secs;
  byte sss; 
  boolean leading_zeroes; 
} segTime; 


const byte digits[10] = {B11000000, B11111001, B10100100, B10110000, B10011001, B10010010, B10000010, B11111000, B10000000, B10010000};
const byte blank = B11111111; 
