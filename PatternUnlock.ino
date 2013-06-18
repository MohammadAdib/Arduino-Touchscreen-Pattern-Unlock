#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>
 
#ifdef SEEEDUINO
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM A0   // can be a digital pin, this is A0
#define XP A3   // can be a digital pin, this is A3
#endif
 
#ifdef MEGA
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3
#endif
 
#define TS_MINX 140
#define TS_MAXX 900
 
#define TS_MINY 120
#define TS_MAXY 940
 
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// The 2.8" TFT Touch shield has 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 500);
 
//colors
int color_background = BLACK, color_circle = WHITE, color_line = WHITE, color_text = WHITE, color_unlocked = GREEN;
 
/**
 * (0) (1) (2)
 * (3) (4) (5)
 * (6) (7) (8)
 */
 
//variables
String saved_combination = "0124678";
String combination = "";
int radius = 5;
int lastActivated = -1;
boolean activations[9];
int lastTouch = -1;
int coords[9][2];
 
//touch variables
boolean down = false;
 
void setup()
{
  Tft.init();  //init TFT library
  Tft.setDisplayDirect(UP2DOWN);
  //circle coordinates [index][0 for x, 1 for y]
  coords[0][0] = 80;
  coords[0][1] = 40;
  coords[1][0] = 160;
  coords[1][1] = 40;
  coords[2][0] = 240;
  coords[2][1] = 40;
  coords[3][0] = 80;
  coords[3][1] = 120;
  coords[4][0] = 160;
  coords[4][1] = 120;
  coords[5][0] = 240;
  coords[5][1] = 120;
  coords[6][0] = 80;
  coords[6][1] = 200;
  coords[7][0] = 160;
  coords[7][1] = 200;
  coords[8][0] = 240;
  coords[8][1] = 200;
  clear();
  drawGrid();
  Serial.begin(9600);
}
 
void drawGrid() {
  //0
  drawCircle(coords[0][0],coords[0][1],radius,color_circle);
  //1
  drawCircle(coords[1][0],coords[1][1],radius,color_circle);
  //2
  drawCircle(coords[2][0],coords[2][1],radius,color_circle);
  //3
  drawCircle(coords[3][0],coords[3][1],radius,color_circle);
  //4
  drawCircle(coords[4][0],coords[4][1],radius,color_circle);
  //5
  drawCircle(coords[5][0],coords[5][1],radius,color_circle);
  //6
  drawCircle(coords[6][0],coords[6][1],radius,color_circle);
  //7
  drawCircle(coords[7][0],coords[7][1],radius,color_circle);
  //8
  drawCircle(coords[8][0],coords[8][1],radius,color_circle);
}
 
void drawCircle(int x, int y, int radius, int color) {
  Tft.drawCircle(240-y,x,radius,color);
}
 
void fillCircle(int x, int y, int radius, int color) {
  Tft.fillCircle(240-y,x,radius,color);
}
 
//fills in a specific circle on the grid
void activate(int n) {
  //record this data
  if(!activations[n]) {
    combination += n;
    Serial.println(combination);
    activations[n] = true;
    if(lastActivated != -1) {
      connectLine(lastActivated, n, color_line);
    }
    lastActivated = n;
    if(n == 0)
      fillCircle(80,40,radius,color_circle);  
    if(n == 1)
      fillCircle(160,40,radius,color_circle);
    if(n == 2)
      fillCircle(240,40,radius,color_circle);
    if(n == 3)
      fillCircle(80,120,radius,color_circle);
    if(n == 4)
      fillCircle(160,120,radius,color_circle);
    if(n == 5)
      fillCircle(240,120,radius,color_circle);
    if(n == 6)
      fillCircle(80,200,radius,color_circle);
    if(n == 7)
      fillCircle(160,200,radius,color_circle);
    if(n == 8)
      fillCircle(240,200,radius,color_circle);      
    //check combination    
    if(combination.toInt() == saved_combination.toInt()) {
      unlock();
      return;
    }
  }
}
 
//connects the line between two of the circles
void connectLine(int n0, int n1, int color) {
  int x0 = coords[n0][0];
  int x1 = coords[n1][0];
  int y0 = coords[n0][1];
  int y1 = coords[n1][1];
  Tft.drawLine(240 - y0, x0, 240 - y1, x1, color);
}
 
void loop()
{
  // a point object holds x y and z coordinates
  Point p = ts.getPoint();
  p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  if (p.z > ts.pressureThreshhold) {
    //Process touch
    int x = p.y;
    int y = 240 - p.x;
    processTouch(x,y);
  }
  if(combination.length() == 9) {
    reset();
  }
  delay(1);
}
 
void processTouch(int x, int y) {
  if(x > 50 && x < 100) {
    if(y > 10 && y < 70) {
      activate(0);
    }
    else if(y > 90 && y < 150) {
      activate(3);
    }
    else if(y > 170 && y < 230) {
      activate(6);
    }
  }
  //second column
  if(x > 130 && x < 190) {
    if(y > 10 && y < 70) {
      activate(1);
    }
    else if(y > 90 && y < 150) {
      activate(4);
    }
    else if(y > 170 && y < 230) {
      activate(7);
    }
  }
  //third column
  if(x > 210 && x < 270) {
    if(y > 10 && y < 70) {
      activate(2);
    }
    else if(y > 90 && y < 150) {
      activate(5);
    }
    else if(y > 170 && y < 230) {
      activate(8);
    }
  }
}
 
// clear the screen with the background color
void clear() {
  Tft.fillRectangle(0,0,239,319,color_background);
}
 
// unlocks the lock
void unlock() {
  //make all lines green
  int n0 = combination.substring(0,1).toInt();
  for(int i = 1; i < combination.length()+1; i++) {
    int n1 = combination.substring(i-1,i).toInt();
    connectLine(n0, n1, color_unlocked);
    n0 = n1;
  }
  delay(250);
  clear();
  Tft.drawString("Welcome!", 130, 40, 4, color_text);
  delay(1000);
  clear();
  delay(5000);
  reset();
}
 
void reset() {
  clear();
  activations[0] = false;
  activations[1] = false;
  activations[2] = false;
  activations[3] = false;
  activations[4] = false;
  activations[5] = false;
  activations[6] = false;
  activations[7] = false;
  activations[8] = false;
  lastActivated = -1;
  combination = "";
  drawGrid();
}
 
uint16_t color(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}
