/**
 * A Night Lamp library for my little nieces... basically takes four buttons, a couple of 
 * RGB LEDs and creates some magical patterns for them.
 *
 * This is the main script which reads buttons and controls the light show parameters
 *
 * Made with Uncley and Auntly love, Clare and Will.
 *
 * MIT Licensed, Will Hart 2014
 *
 */

#include <Bounce2.h>

// pin definitions
#define R1 3
#define G1 5
#define B1 6
#define R2 9
#define G2 10
#define B2 11
#define PIN_BRT 7
#define PIN_DIM 4
#define PIN_COL 8
#define PIN_RND 12

// colour definitions
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3

// control timings
#define LERP_PERIOD 1000.0
#define TIME_PERIOD 50

// Random data format Indexes
#define LOW_BRIGHT_STEPS 9
#define DURATION 0

// set up four button debouncers
Bounce brighter = Bounce();
Bounce dimmer = Bounce();
Bounce colour = Bounce();
Bounce randomise = Bounce();

// flag to make sure buttons are released before they are pressed again
bool bright_released = true;
bool dimmer_released = true;
bool colour_released = true;
bool randomise_released = true;

// store the brightness
int last_brightness = 150;
int brightness = 0;
int brightness_delta = 40;

// store the current colour
char display_colour = WHITE;

// set up flags and counters for controlling the normal show
bool random_mode = false;
bool brightness_changed = false;
bool lerping = false;
long lerp_start_time = 0;
long last_millis = 0;

// set up variables for controlling the random show
const int brightness_threshold = 150;
char random_index = -1;
bool low_bright = brightness <= brightness_threshold;
unsigned int last_r;
unsigned int last_g;
unsigned int last_b;
unsigned int prev_r; 
unsigned int prev_g;
unsigned int prev_b;
unsigned int target_r;
unsigned int target_g; 
unsigned int target_b; 
unsigned int target_duration;

void setup() {
    // set pin modes
    pinMode(R1, OUTPUT);
    pinMode(G1, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(B2, OUTPUT);
    pinMode(PIN_BRT, INPUT);
    pinMode(PIN_DIM, INPUT);
    pinMode(PIN_COL, INPUT);
    pinMode(PIN_RND, INPUT);
  
    // activate internal pull up resistors
    digitalWrite(PIN_BRT, HIGH);
    digitalWrite(PIN_DIM, HIGH);
    digitalWrite(PIN_COL, HIGH);
    digitalWrite(PIN_RND, HIGH);
  
    // set up the debouncers
    brighter.attach(PIN_BRT);
    brighter.interval(5);
    dimmer.attach(PIN_DIM);
    dimmer.interval(5);
    colour.attach(PIN_COL);
    colour.interval(5);
    randomise.attach(PIN_RND);
    randomise.interval(5);
    
    // initialise the random number generator
    randomSeed(analogRead(0));
    
    // wait a bit, then store the current time
    delay(100);
    last_millis = millis();
   
    Serial.begin(57600);
}

void loop() {
    // update the button debounce state
    brighter.update();
    dimmer.update();
    colour.update();
    randomise.update();
  
    // check our brightness
    if (brighter.read() == LOW) {
        //Serial.print("BR ");
        if (!lerping && bright_released) {
            brightness_changed = true;
            bright_released = false;
            last_brightness = brightness;
            brightness = constrain(brightness + brightness_delta, 0, 255);
        }
    } else {
        //Serial.print("   ");
        bright_released = true;
        
        if (dimmer.read() == LOW) {
            //Serial.print("DM ");
            if (!lerping && dimmer_released) {
                brightness_changed = true;
                dimmer_released = false;
                last_brightness = brightness;
                brightness = constrain(brightness - brightness_delta, 0, 255);
            }
        } else {
            //Serial.print("   ");
            dimmer_released = true;
        }
    }
    
    // see if we are changing the colour
    if (colour.read() == LOW) {
        //Serial.print("CL ");
        if (colour_released) {
            colour_released = false;
            display_colour++;
            if (display_colour > BLUE) {
                display_colour = WHITE;
            }
        }
    } else {
        //Serial.print("   ");
        colour_released = true;
    }
    
    // check if we need to toggle RANDO mode
    if (randomise.read() == LOW) {
        //Serial.print("RD ");
        if (randomise_released) {
            randomise_released = false;
            random_mode = !random_mode;
            
            if (random_mode) {
                low_bright = brightness <= brightness_threshold;   
            }
            lerp_start_time = 0;
        }
    } else {
        //Serial.print("   ");
        randomise_released = true;
    }
    
    // if we have passed our delay period, update the LEDs
    if (millis() - TIME_PERIOD > last_millis) {
        if (random_mode) {
            random_mode_step();
        } else {
            normal_mode_step(brightness, display_colour);
        }
        
        last_millis = millis();
    }
    
    /*Serial.print(brightness);
    Serial.print(" ");
    Serial.print(last_r);
    Serial.print(" ");
    Serial.print(last_g);
    Serial.print(" ");
    Serial.print(last_b);
    Serial.print(" ");
    Serial.print(random_index);
    Serial.println(" ");*/
}
