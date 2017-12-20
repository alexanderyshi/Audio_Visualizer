// note to self: using triggers will not work for real sound waveforms, where the perioidicity of the trigger is irregular
// will need to send microphone input through serial to python
// then send back the respective note to the arduino for processing
// unless a light weight FFT algorithm can be used to help speed this up

//Alexander Shi
//last modified 19 July 2015
#include <Arduino.h>
//#include <Time.h>
#include "WordBuilder.cpp"
#include "DrawingEngine.cpp"

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
static int preValue = 0;

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
  
//  for (int i=1; i<=16;i++){
//  drawPixel(mutableWord, i,i);  
//  }
//  WordBuilder::drawCenterDiamond(mutableWord,5);  
//  WordBuilder::drawPixel(mutableWord,2,2);
//  WordBuilder::drawPixel(mutableWord,2,15);
//  WordBuilder::drawPixel(mutableWord,15,2);
//  WordBuilder::drawPixel(mutableWord,15,15);
    WordBuilder::drawLine(mutableWord, 9,9,13,16);
    WordBuilder::drawLine(mutableWord, 8,9,4,16);
    WordBuilder::drawLine(mutableWord, 8,8,4,1);
    WordBuilder::drawLine(mutableWord, 9,8,13,1);
    WordBuilder::drawLine(mutableWord, 9,9,16,13);
    WordBuilder::drawLine(mutableWord, 9,8,16,4);
    WordBuilder::drawLine(mutableWord, 8,8,1,4);
    WordBuilder::drawLine(mutableWord, 8,9,1,13);
  Serial.begin(9600);
  INTENSITY_THRESHOLD = GetSoundThreshold();
  Serial.println(INTENSITY_THRESHOLD);

  drawingEngine = DrawingEngine();
  //    WordBuilder::cleanWord(testWord);  
}

void loop()
{
//  preValue = curValue;
  curValue = analogRead(A0);
  delay(1); // max analog read speed is about 100 us
  char hit = curValue>INTENSITY_THRESHOLD;
  if (!hit){
    drawingEngine.Display(testWord); 
  }
  else{    
      drawingEngine.Display(mutableWord);
      Serial.println(curValue);
      delay(10);
  }
}


