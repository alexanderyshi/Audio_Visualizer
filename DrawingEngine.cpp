#include <Arduino.h>

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
//TODO: make this into a static class
class DrawingEngine{

  public:
    DrawingEngine(){
      
    }
	private:
  unsigned char Display_Buffer[2];
    void Send( unsigned char dat)
    {
      unsigned char i;
      digitalWrite(LEDARRAY_CLK, LOW);
      delayMicroseconds(1);;  
      digitalWrite(LEDARRAY_LAT, LOW);
      delayMicroseconds(1);;
            
      for( i = 0 ; i < 8 ; i++ )
      {
      //mic input range: 0-1023
        if( dat&0x01)  //&true comparison targetting just first position of packed char?
        {
          digitalWrite(LEDARRAY_DI, LOW); 
        }
        else
        {
          digitalWrite(LEDARRAY_DI, HIGH);
        }
    
        delayMicroseconds(1);
        digitalWrite(LEDARRAY_CLK, HIGH);       //上升沿发送数据   //hardware signal timing - data transfer on rising edge 
          delayMicroseconds(1);
        digitalWrite(LEDARRAY_CLK, LOW);
          delayMicroseconds(1);   
        dat >>= 1;                                                      //bit shift w/ assignment
          
      }     
    }
    
    //sets pins for 1 column of eight at a time
    void Scan_Line( unsigned char m)
    {  
      switch(m)
      {
        case 0:     
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);          
          break;
        case 1:         
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
          break;
        case 2:         
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
          break;
        case 3:         
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
          break;
        case 4:
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
          break;
        case 5:
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
          break;
        case 6:
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
          break;
        case 7:
          digitalWrite(LEDARRAY_D, LOW);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
          break;
        case 8:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);     
          break;
        case 9:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);    
          break;  
        case 10:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);    
          break;
        case 11:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, LOW);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);     
          break;
        case 12:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, LOW);    
          break;
        case 13:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, LOW);digitalWrite(LEDARRAY_A, HIGH);     
          break;
        case 14:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, LOW);     
          break;
        case 15:
          digitalWrite(LEDARRAY_D, HIGH);digitalWrite(LEDARRAY_C, HIGH);digitalWrite(LEDARRAY_B, HIGH);digitalWrite(LEDARRAY_A, HIGH);    
          break;
        default : break;  
      }
    }

    void static clearScreen(){
      for( int i = 0 ; i < 8 ; i++ )
        {
          digitalWrite(LEDARRAY_DI, HIGH);
      
          delayMicroseconds(1);
          digitalWrite(LEDARRAY_CLK, HIGH);         //hardware signal timing - data transfer on rising edge 
            delayMicroseconds(1);
          digitalWrite(LEDARRAY_CLK, LOW);
            delayMicroseconds(1);         
        }   
    }
  public:
    void Display(unsigned char dat[][32])          
    {
      unsigned char i;
      for( i = 0 ; i < 16 ; i++ )
      {
        digitalWrite(LEDARRAY_G, HIGH);   
        
        Display_Buffer[0] = dat[0][i];    
        Display_Buffer[1] = dat[0][i+16];
          
        Send(Display_Buffer[1]);
        Send(Display_Buffer[0]);  
        
        digitalWrite(LEDARRAY_LAT, HIGH);      
        delayMicroseconds(1);
      
        digitalWrite(LEDARRAY_LAT, LOW);
        delayMicroseconds(1);
    
        Scan_Line(i);             
    
        digitalWrite(LEDARRAY_G, LOW);
        
        delayMicroseconds(100);        
      } 
    }
};
