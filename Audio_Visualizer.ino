// note to self: using triggers will not work for real sound waveforms, where the perioidicity of the trigger is irregular
// will need to send microphone input through serial to python
// then send back the respective note to the arduino for processing
// unless a light weight FFT algorithm can be used to help speed this up

//Alexander Shi
//last modified 21 December 2017
#include <Arduino.h>
#include "WordBuilder.cpp"
#include "DrawingEngine.cpp"

#define LOG_OUT 1
#define FHT_N 256

// external includes
#include <FHT.h>

//IO    
#define LEDARRAY_D    11
#define LEDARRAY_C    10
#define LEDARRAY_B    9
#define LEDARRAY_A    8
#define LEDARRAY_G    7
#define LEDARRAY_DI   6
#define LEDARRAY_CLK  5
#define LEDARRAY_LAT  4
#define BUTTON        13
#define MICROPHONE    A0


static int INTENSITY_THRESHOLD = 350;

static int curValue = 0;
//static int preValue = 0;

DrawingEngine drawingEngine;
unsigned char mutableWord[1][32];
//each index position represents one of 32 columns in a 16 wide, 2 tall array (cables being on the "bottom" side), 
  //values from 0->255 loaded for each column (MSD on bottom, LSD on top)
unsigned char testWord[1][32]= 
{
  0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x09, 0x09, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x90, 0x90, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00
};

unsigned GetSoundThreshold() { 
    const unsigned TEST_POINT_COUNT = 100;
    unsigned long long testAggr = 0;
    unsigned maxVal = 0;
    unsigned A0_Val = 0;
  
    for (int i = 0; i < TEST_POINT_COUNT; ++i) {
      delay(1); // max analog read speed is about 100 us
      A0_Val = analogRead(A0);  
//    testAggr += A0_Val;
      maxVal = A0_Val > maxVal ? A0_Val : maxVal;
//    Serial.println(A0_Val);
    }
//  INTENSITY_THRESHOLD = testAggr / TEST_POINT_COUNT;
//  Serial.println(INTENSITY_THRESHOLD);
    maxVal = maxVal < 200 ? 200 : maxVal;
    return 2*maxVal > 1023 ? 800 : 2*maxVal;
}

void setup()
{
  pinMode(LEDARRAY_D, OUTPUT); 
  pinMode(LEDARRAY_C, OUTPUT);
  pinMode(LEDARRAY_B, OUTPUT);
  pinMode(LEDARRAY_A, OUTPUT);
  pinMode(LEDARRAY_G, OUTPUT);
  pinMode(LEDARRAY_DI, OUTPUT);
  pinMode(LEDARRAY_CLK, OUTPUT);
  pinMode(LEDARRAY_LAT, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(MICROPHONE, INPUT);
  
//  WordBuilder::drawCenterDiamond(mutableWord,5);  
//  WordBuilder::drawPixel(mutableWord,15,15);
    WordBuilder::drawLine(mutableWord, 9,9,13,16);
    WordBuilder::drawLine(mutableWord, 8,9,4,16);
    WordBuilder::drawLine(mutableWord, 8,8,4,1);
    WordBuilder::drawLine(mutableWord, 9,8,13,1);
    WordBuilder::drawLine(mutableWord, 9,9,16,13);
    WordBuilder::drawLine(mutableWord, 9,8,16,4);
    WordBuilder::drawLine(mutableWord, 8,8,1,4);
    WordBuilder::drawLine(mutableWord, 8,9,1,13);
//  Serial.begin(9600);
//  INTENSITY_THRESHOLD = GetSoundThreshold();
//  Serial.println(INTENSITY_THRESHOLD);

//  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

  drawingEngine = DrawingEngine();
}

void loop()
{
  while(1) {
    cli();  
    // RUNNING THE FHT
    for (uint16_t i = 0; i < FHT_N; ++i) {
      // fht_input[i*2] = (uint16_t) analogRead(A0);
      // fht_input[i*2+1] = 0;
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    // Serial.println("finish buffer fill");
    fht_window();
    fht_reorder();
    fht_run();
    fht_mag_log();
    sei();

    WordBuilder::cleanWord(mutableWord);  
    for (uint8_t i = 0; i < FHT_N/2; ++i) {
      if (fht_log_out[i] & 0xc0) {
        WordBuilder::drawPixel(mutableWord, i/16, i%16);      
      }
    }
    drawingEngine.Display(mutableWord);
    delay(5);
    drawingEngine.Display(testWord);
    
    // BEAT DETECTION
  //  curValue = analogRead(A0);
  //  delay(1); // max analog read speed is about 100 us
  //  char hit = curValue>INTENSITY_THRESHOLD;
  //  if (!hit){
  //    drawingEngine.Display(testWord); 
  //  }
  //  else{    
  //      drawingEngine.Display(mutableWord);
  //      Serial.println(curValue);
  //      delay(10);
  //  }
  }
}
