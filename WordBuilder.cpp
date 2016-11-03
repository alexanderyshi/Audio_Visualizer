#include <Arduino.h>
class WordBuilder{
	public:

  //TODO: FIX THIS
    void static cleanWord(unsigned char dat[][32]) {
      for (int i = 0 ; i < 32 ; i++) {
        dat[0][i] = 0x00;
      }
    }
    
		//indexed 1-16, take a vert (j) and hor (i) cartesian coordinate to draw
    //logic here is weird because the led matrix is "upside-down"
		void static drawPixel(unsigned char dat[][32], int i, int j){
		  if (j<1 || i<1 || j>16 || i>16){
			  return;
		  }
		  ///bottom half
		  else if (j<=8){
        if (dat[0][i-1] & (1 << (8-j))) {
          return;
        }
        dat[0][i-1] += 1 << (8-j);  
		  }
		  //top half
		  else {
        if (dat[0][i+15] & (1 << (16-j))) {
          return;
        }
			  dat[0][i+15] += 1 << (16-j);  
		  }
		}

   //draw a line given start and end
   //TODO: clean up these for loops
   void static drawLine(unsigned char dat[][32], int x1, int y1, int x2, int y2){
    if (x1 < 1 || x2 < 1 || y1 < 1 || y2 < 1 || x1 > 16 || x2 > 16 || y1 > 16 || y2 > 16) {
      return;
    }
    //same start and end
    else if (x1 == x2 && y1 == y2) {
      drawPixel(dat,x1,y1);
    }
    //horizontal
     if (y1 == y2) {
      for (int i = 0; i <= abs(x2 - x1);  i++) {
        drawPixel(dat, min(x1, x2) + i, y1);
      }
    }
    //vertical
    else if (x1 == x2) {
      for (int j = 0; j <= abs(y2 - y1); j++) {
        drawPixel(dat, x1, min(y1,y2) + j);
      }
    }
    else {
      if (x1 > x2){
          int tmpx = x1;
          x1 = x2;
          x2 = tmpx;
          int tmpy = y1;
          y1 = y2;
          y2 = tmpy;
        }
  
      //45 deg diagonal
      if (abs(abs(x1-x2) - abs(y1-y2))< 1E-10){        
        int inc = y2 > y1 ? 1 : -1;
        int y = y1;
        for (int i = x1; i <= x2; i++) {
          drawPixel(dat, i, y);
          y += inc;
        }
      }
      //abs slope less than half "hor"
      else if (abs(x2 - x1) > abs(y2 - y1)) {
        double m = (y2-y1)/(double)(x2-x1);
        double y = y1;
        for (int i = x1; i<= x2;i++){
          drawPixel(dat, i, (int)y);
          y += m;
        }
      }
      //abs slope greater than half "vert"
      else {
        int inc = y2 > y1 ? 1 : -1;
        double m = abs((x2-x1)/(double)(y2-y1));
        double x = x1;
        for (int j = y1; y2 > y1 ? j<= y2: j >=y2; j+=inc){
          drawPixel(dat, (int)x, j);
          x += m;
        }
      }
    }
   }
   
   //draws a circle from the centre of the board of radius r
    void static drawCenterCircle(unsigned char dat[][32], int r){
      if (r >= 15 || r<0)
        return;
      for (int i = 0; i< r; i++){
        //top left quadrant  
        drawPixel(dat,8-i, 8+r-i);  
        
        //top right quadrant
        drawPixel(dat,9+i, 8+r-i);  
        
        //bottom left quadrant  
        drawPixel(dat,8-i, 9-r+i);  
        
        //bottom right quadrant
        drawPixel(dat,9+i, 9-r+i); 
      }
    }

   //draws a circle  from a given centre of the board of radius r

		//draws a diamond from the centre of the board with side length r
   // TODO: refactor to make use of drawLine
		void static drawCenterDiamond(unsigned char dat[][32], int r){
		  if (r >= 15 || r<0)
			  return;
		  for (int i = 0; i< r; i++){
  			//top left quadrant  
  			drawPixel(dat,8-i, 8+r-i);  
        
  			//top right quadrant
  			drawPixel(dat,9+i, 8+r-i);  
  			
  			//bottom left quadrant  
  			drawPixel(dat,8-i, 9-r+i);  
  			
  			//bottom right quadrant
  			drawPixel(dat,9+i, 9-r+i); 
		  }
		}

   //draws a diamond from a given centre of the board of side length 

   //draw a rectangle given top left corner, bottom right corner using line method

   
};
