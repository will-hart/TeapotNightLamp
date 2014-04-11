/**
 * A Night Lamp library for my little nieces... basically takes four buttons, a couple of 
 * RGB LEDs and creates some magical patterns for them.
 *
 * This file contains some predefined light shows for the "random" program
 *
 * Made with Uncley and Auntly love, Clare and Will.
 *
 * MIT Licensed, Will Hart 2014
 *
 */
 
// LIGHT SHOWS ARE STORED IN THE FOLLOWING 2D ARRAY FORMAT:
//  [ Lerp Duration (ms), R, G, B ]

/* 
 * A pulsing, low brightness display for encouraging sleep
 */

 
unsigned char low_brightness[LOW_BRIGHT_STEPS][4] = {
    { 1000,   0,   0,   0},
    { 3000,   1,   0,   0},
    { 3000,   0,   0,   0},
    { 3000,   0,   1,   0},
    { 3000,   0,   0,   0},
    { 3000,   0,   0,   1},
    { 3000,   0,   0,   0},
    { 3000,   1,   1,   1},
    { 3000,   0,   0,   0},
};

