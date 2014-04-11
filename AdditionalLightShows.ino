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
//  [ R, G, B, Lerp Duration (ms) ]

/* 
 * A pulsing, low brightness display for encouraging sleep
 */

 
unsigned char low_brightness[LOW_BRIGHT_STEPS][4] = {
    {  0,   0,   0, 1000},
    {100,   0,   0, 3000},
    {  0,   0,   0, 3000},
    {  0, 100,   0, 3000},
    {  0,   0,   0, 3000},
    {  0,   0, 100, 3000},
    {  0,   0,   0, 3000},
    {100, 100, 100, 3000},
    {  0,   0,   0, 3000},
};

