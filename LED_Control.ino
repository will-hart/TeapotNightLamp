/**
 * A Night Lamp library for my little nieces... basically takes four buttons, a couple of 
 * RGB LEDs and creates some magical patterns for them.
 *
 * This is the code for controlling the LEDs to the given program
 *
 * Made with Uncley and Auntly love, Clare and Will.
 *
 * MIT Licensed, Will Hart 2014
 *
 */

/* 
 * LERP interpolates lineraly between two values in time. For instance it can 
 * increase brightness from 0 to 255 over 1 second.
 * 
 * ARGS: 
 *  - progress:  a float 0.0 to 1.0 indicating how far through the LERP we are
 *  - initial: the initial LERP value
 *  - final: the final LERP value
 */
int lerp(float progress, int initial, int final) {
    if (progress >= 1.0) {
        return final;
    }
    
    return initial + (int)(progress * (final - initial));
}

/*
 * Updates the current LED state based on the given brightness and colour.
 * If the brightness has been changed it lerps to a new brightness over 1 second
 */
void normal_mode_step(char brightness, char display_colour) {
    
    int new_brightness = brightness;
    
    // handle the brightness being changed
    if (brightness_changed) {
        brightness_changed = false;
        lerping = true;
        lerp_start_time = millis();
    }
    
    // lerp between the brightness levels if required
    if (lerping) {
        long elapsed = millis() - lerp_start_time;
        float progress = elapsed / LERP_PERIOD;
        new_brightness = lerp(progress, last_brightness, brightness);
        
        if (progress > 1.0) {
            lerping = false;
            lerp_start_time = 0;
        }
    }
    
    // calculate the individual coloured LED states
    last_r = last_g = last_b = 0;
    if (display_colour == WHITE || display_colour == RED) {
        last_r = new_brightness;
    }
    if (display_colour == WHITE || display_colour == GREEN) {
        last_g = new_brightness;
    }
    if (display_colour == WHITE || display_colour == BLUE) {
        last_b = new_brightness;
    }
    
    set_leds(last_r, last_g, last_b);
}

/* 
 * Sets the lamp in random mode. If the brightness is under 150 then it is in "low intensity", 
 * or "sleepy time" mode. In this case the lights will simply pulse colours slowly. If brightness
 * is over 150 then the patterns will be random colours for random time periods.
 *
 * The supplied boolean flag is set to true where the brightness has been changed
 */
void random_mode_step() {
    if (brightness_changed) {
        bool new_low_bright = brightness <= brightness_threshold;
        if (new_low_bright != low_bright) {
            random_index = -1;
            lerp_start_time = 0;
        }
        
        low_bright = new_low_bright;
        brightness_changed = false;
    }
    
    // handle reaching/starting a new step
    if (lerp_start_time == 0) {
        
        // get our last target values
        prev_r = last_r;
        prev_g = last_g;
        prev_b = last_b;
        
        if (low_bright) {
            //Serial.print("LOW: ");
            if (random_index >= LOW_BRIGHT_STEPS) {
                random_index = 0;   
            } else {
                random_index++;
            }
            
            prev_r = target_r;
            prev_g = target_g;
            prev_b = target_b;
            
            target_duration = low_brightness[random_index][0];
            target_r = low_brightness[random_index][RED] * brightness;
            target_g = low_brightness[random_index][GREEN] * brightness;
            target_b = low_brightness[random_index][BLUE] * brightness;
        } else {
            //Serial.print("HIGH: ");
            target_r = random(0, 256);
            target_g = random(0, 256);
            target_b = random(0, 256);
            target_duration = random(1, 300) * 10;
        }
        
        /*
        Serial.print(">>> ");
        Serial.print(target_r);
        Serial.print(" ");
        Serial.print(target_g);
        Serial.print(" ");
        Serial.print(target_b);
        Serial.print(" ");
        Serial.println(target_duration);
        */
        
        lerp_start_time = millis();
    }
    
    // determine our new rgb values
    long elapsed = millis() - lerp_start_time;
    float progress = elapsed / target_duration;
    last_r = lerp(progress, prev_r, target_r);
    last_g = lerp(progress, prev_g, target_g);
    last_b = lerp(progress, prev_b, target_b);
    
    // work out if we are ready for the next step
    if (progress >= 1.0) {
        lerp_start_time = 0;
    }

    // update the LED state
    set_leds(last_r, last_g, last_b);
}

/* 
 * Sets the LED brightness based on the passed RGB values
 */
void set_leds(int r, int g, int b) {
    // constrain the LED values just in case
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);
    
    // write the LED states
    analogWrite(R1, r);
    analogWrite(R2, r);
    analogWrite(G1, g);
    analogWrite(G2, g);
    analogWrite(B1, b);
    analogWrite(B2, b);   
}
