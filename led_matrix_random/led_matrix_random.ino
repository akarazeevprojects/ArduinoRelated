#define RUN_DEMO  0

#include <MD_MAX72xx.h>
#include <SPI.h>

#if RUN_DEMO
#define DEMO_DELAY  15  // time to show each demo element in seconds
#else
#include <MD_UISwitch.h>
#endif

#define DEBUG 0   // Enable or disable (default) debugging output

#if DEBUG
#define PRINT(s, v)   { Serial.print(F(s)); Serial.print(v); }        // Print a string followed by a value (decimal)
#define PRINTX(s, v)  { Serial.print(F(s)); Serial.print(v, HEX); }   // Print a string followed by a value (hex)
#define PRINTB(s, v)  { Serial.print(F(s)); Serial.print(v, BIN); }   // Print a string followed by a value (binary)
#define PRINTC(s, v)  { Serial.print(F(s)); Serial.print((char)v); }  // Print a string followed by a value (char)
#define PRINTS(s)     { Serial.print(F(s)); }                         // Print a string
#else
#define PRINT(s, v)   // Print a string followed by a value (decimal)
#define PRINTX(s, v)  // Print a string followed by a value (hex)
#define PRINTB(s, v)  // Print a string followed by a value (binary)
#define PRINTC(s, v)  // Print a string followed by a value (char)
#define PRINTS(s)     // Print a string
#endif

// --------------------
// MD_MAX72xx hardware definitions and object
// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
//
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 5
#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);                      // SPI hardware interface
//MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); // Arbitrary pins

#if !RUN_DEMO
// --------------------
// Mode keyswitch parameters and object
//
#define MODE_SWITCH 9 // Digital Pin

MD_UISwitch_Digital  ks = MD_UISwitch_Digital(MODE_SWITCH, LOW);
#endif

int COUNTER = 0;
bool COUNTING_DOWN = false;

// --------------------
// Constant parameters
//
// Various delays in milliseconds
#define UNIT_DELAY      25
#define RANDOM_DELAY    (6 * UNIT_DELAY)

#define CHAR_SPACING     1  // pixels between characters
#define BUF_SIZE        75  // character buffer size

// ========== General Variables ===========
//
uint32_t prevTimeAnim = 0;    // Used for remembering the millis() value in animations
#if RUN_DEMO
uint32_t prevTimeDemo = 0;      //  Used for remembering the millis() time in demo loop
uint8_t  timeDemo = DEMO_DELAY; // number of seconds left in this demo loop
#endif

uint8_t column_values[32];

bool graphicRandom(bool bInit) {
  if (bInit) {
    PRINTS("\n--- Random init");
    resetMatrix();
    bInit = false;
  }
  if (millis()-prevTimeAnim < RANDOM_DELAY) {
    return(bInit);
  }
  prevTimeAnim = millis();
  int intensity = 1;
  mx.control(MD_MAX72XX::INTENSITY, intensity);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  
//  for (uint8_t i = 0; i < mx.getColumnCount(); i++) {
////    mx.setColumn(i, (uint8_t) random(255));
//    for (int j = 0; j < 8; j++) {
//      int rand_value = random(5);
//      if (rand_value == 0) {
//        mx.setPoint(j, i, true);
//      } else {
//        mx.setPoint(j, i, false);
//      }
//    }
//  }

  if (COUNTING_DOWN == false) {

    for (uint8_t i = 0; i < mx.getColumnCount(); i++) {
      for (int j = 0; j < 8; j++) {
        
        if (mx.getPoint(j, i) == false) {
          int rand_value = random(10);
          if (rand_value == 0) {
            mx.setPoint(j, i, true);
          }
        }
        
      }
    }
  } else if (COUNTING_DOWN) {
    for (uint8_t i = 0; i < mx.getColumnCount(); i++) {
      for (int j = 0; j < 8; j++) {
        
        if (mx.getPoint(j, i) == true) {
          int rand_value = random(8);
          if (rand_value == 0) {
            mx.setPoint(j, i, false);
          }
        }
        
      }
    }
  }

  if (COUNTER == 15) {
    COUNTING_DOWN = true;
  } else if (COUNTER == 0) {
    COUNTING_DOWN = false;
  }

  if (COUNTING_DOWN) {
    COUNTER--;
  } else {
    COUNTER++;
  }

  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

  return(bInit);
}


// ========== Control routines ===========
//
void resetMatrix(void)
{
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY/2);
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  mx.clear();
  prevTimeAnim = 0;
}

void runMatrixAnimation(void)
// Schedule the animations, switching to the next one when the
// the mode switch is pressed.
{
  static  uint8_t state = 0;
  static  uint8_t mesg = 0;
  static  boolean bRestart = true;
	static boolean	bInMessages = false;
  boolean changeState = false;

#if RUN_DEMO
  // check if one second has passed and then count down the demo timer. Once this
  // gets to zero, change the state.
  if (millis()-prevTimeDemo >= 1000)
  {
    prevTimeDemo = millis();
    if (--timeDemo == 0)
    {
      timeDemo = DEMO_DELAY;
      changeState = true;
    }
  }
#else
  // check if the switch is pressed and handle that first
  changeState = (ks.read() == MD_UISwitch::KEY_PRESS);
#endif
  if (changeState)
  {
    bRestart = true;
  };

  bRestart = graphicRandom(bRestart);
}

void setup() {
  mx.begin();
  prevTimeAnim = millis();
#if RUN_DEMO
  prevTimeDemo = millis();
#else
  ks.begin();
#endif
#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("\n[MD_MAX72XX DaftPunk]");
}

void loop() {
  runMatrixAnimation();
}

