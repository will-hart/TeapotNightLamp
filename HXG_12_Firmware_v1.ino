/**
 * A Night Lamp library for my little nieces... basically takes four buttons, a couple of 
 * RGB LEDs and creates some magical patterns for them.
 *
 * Made with uncley and aunty love, Clare and Will
 */

#include <Bounce2.h>

// set up four button debouncers
Bounce brighter = Bounce();
Bounce dimmer = Bounce();
Bounce colour = Bounce();
Bounce randomise = Bounce();

// store the brightness
int brightness = 150;
int brightness_delta = 40;

// colour definitions
#define WHITE 0
#define RED 1
#define GREEN 2
#define BLUE 3

// store the current colour
char display_colour = WHITE;

// store a flag for random mode
bool random_mode = false;

// pin definitions
#define R1 1
#define G1 2
#define B1 3
#define R2 4
#define G2 5
#define B2 6
#define PIN_BRT 7
#define PIN_DIM 8
#define PIN_COL 9
#define PIN_RND 10

// update freuqency
int time_period = 100; // update once this number of millis has elapsed
int last_millis = 0;

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
    
    // wait a bit, then store the current time
    delay(100);
    last_millis = millis();
}

void loop() {
    // read the buttons
    brighter.update();
    dimmer.update();
    colour.update();
    randomise.update();
  
    // check our brightness
    if (brighter.read() == LOW) {
        brightness = constrain(brightness + brightness_delta, 0, 255);
    } else if (dimmer.read() == LOW) {
        brightness = constrain(brightness - brightness_delta, 0, 255);
    }
    
    // check our colour
    if (colour.read() == LOW) {
        display_colour++;
        if (display_colour > BLUE) {
            display_colour = WHITE;
        }
    }
    
    // check if we need to toggle RANDO mode
    if (randomise.read() == LOW) {
        random_mode = !random_mode;
    }
    
    // if we have passed our delay period, update the LEDs
    if (millis() - time_period > last_millis) {
        if (random_mode) {
            random_mode_step();
        } else {
            render_leds(brightness, display_colour);
        }
        
        last_millis = millis();
    }
}
