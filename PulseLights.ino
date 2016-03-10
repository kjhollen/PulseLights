
/*  Pulse Sensor Amped 1.4    by Joel Murphy and Yury Gitman   http://www.pulsesensor.com

  ----------------------  Notes ----------------------  ----------------------
  This code:
  1) Blinks an LED to User's Live Heartbeat   PIN 13
  2) Fades an LED to User's Live HeartBeat
  3) Determines BPM
  4) Prints All of the Above to Serial

  Read Me:
  https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
  ----------------------       ----------------------  ----------------------
*/

#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

//  Variables
#define PULSE_PIN 0                 // Pulse Sensor purple wire connected to analog pin 0
#define BLINK_PIN 13                // pin to blink led at each beat
int fadeRate = 0;                   // used to fade LED brightness after beat
long waitPeriod = 0;
long lastMove = 0;
long timeSinceMove = 0;


// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

#define NUMPIXELS 60 // Number of LEDs in strip
#define DATA_PIN   4
#define CLOCK_PIN  5

uint32_t colors[NUMPIXELS];

Adafruit_DotStar strip = Adafruit_DotStar(
                           NUMPIXELS, DATA_PIN, CLOCK_PIN, DOTSTAR_GBR);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);


void setup() {
  pinMode(BLINK_PIN, OUTPUT);        // pin that will blink to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
  // IF YOU ARE POWERING The Pulse Sensor AT VOLTAGE LESS THAN THE BOARD VOLTAGE,
  // UN-COMMENT THE NEXT LINE AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //   analogReference(EXTERNAL);

  for (int i = 0  ;  i < NUMPIXELS  ;  i++)
    colors[i] = 0x000000;

  strip.begin();
  strip.show();
}


// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

//  Where the Magic Happens
void loop() {

  serialOutput() ;

  if (QS == true) {    // A Heartbeat Was Found
    // BPM and IBI have been Determined
    // Quantified Self "QS" true when arduino finds a heartbeat
    digitalWrite(BLINK_PIN, HIGH);    // Blink LED, we got a beat.
    fadeRate = 255;         // Makes the LED Fade Effect Happen
    waitPeriod = 0;
    timeSinceMove = 0;
    lastMove = millis();
    // Set 'fadeRate' Variable to 255 to fade LED with pulse
    serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
    QS = false;                      // reset the Quantified Self flag for next time
  }



  // turn on lights for pulses

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
}




void ledFadeToBeat() {
  timeSinceMove = millis() - lastMove;
  if (timeSinceMove >= waitPeriod) {
    Serial.println("WAIT " + waitPeriod);
    waitPeriod = constrain (waitPeriod + 5, 0, 100);
    lastMove = millis ();
    timeSinceMove = 0;
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate, 0, 255); //  keep LED fade value from going into negative numbers!
    
    for (int i = NUMPIXELS - 1  ;  i >= 1  ;  i--) {
      colors[i] = colors[i-1];
    }
    colors[0] = (unsigned long) fadeRate << 16 | (unsigned long) (255 - fadeRate) << 8;
    
    // light em up
    for (int i = 0 ;  i < NUMPIXELS  ;  i++)
      strip.setPixelColor (head + i, colors[i]);
    
    strip.show();
  }
}




