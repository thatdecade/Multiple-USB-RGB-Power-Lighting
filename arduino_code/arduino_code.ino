/* 
 * Trinket Powered Neopixels with Color Selection
 *  
 *  Dustin Westaby
 *  August 2020
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define SPIN_SPEED 60
#define SPIN_DURATION 10*8

#define SOLID_COLOR_FADE_SPEED 10
#define BLANKING_OFFSET 4

#define NEO_PIN 3

#define RED_PIN   0
#define BLUE_PIN  1
#define GREEN_PIN 2

#define NUM_LEDS 32
#define NUM_LEDS_ONE_WHEEL 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, NEO_PIN);

#define RED    0xFF0000
#define BLUE   0x0000FF
#define GREEN  0x00FF00
#define TEAL   0x00FFFF
#define YELLOW 0xFF00FF

//set starup colors
uint32_t color1  = BLUE;
uint32_t color2  = BLUE;

uint32_t prevTime;

void setup() 
{
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(255);

  pinMode(RED_PIN,   INPUT);
  pinMode(BLUE_PIN,  INPUT);
  pinMode(GREEN_PIN, INPUT);
  
  prevTime = millis();
}

void loop() {
  static uint32_t previous_color1 = 0;
  static uint32_t previous_color2 = 0;

  check_for_input_changes();

  if( (color1 != previous_color1) ||
      (color2 != previous_color2))
  {
    previous_color1 = color1;
    previous_color2 = color2;
    
    animate_spin();
    solid_color_fade_down();
  }
}

void check_for_input_changes()
{
  //read inputs and set color

  //SINGLES
  
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = GREEN;
    color2 = GREEN;
  }
  if((digitalRead(GREEN_PIN) == LOW) && 
     (digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = RED;
    color2 = RED;
  }
  if((digitalRead(GREEN_PIN) == LOW) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    color1 = BLUE;
    color2 = BLUE;
  }

  //DOUBLES
  
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = GREEN;
    color2 = RED;
  }
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    color1 = BLUE;
    color2 = GREEN;
  }
  if((digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    //DOUBLES and TRIPLES, Favor blue RED, even if green is also ON
    color1 = BLUE;
    color2 = RED;
  }

  
}

void solid_color_fade_down()
{
  uint8_t  i,j;
  
  //set to solid color, while also dimming gradually.
  for(i=0; i<16; i++) 
  {
    for(j=1;j<16;j++)
    {
      pixels.setBrightness(map(i*15+j,0,254,254,45));
      pixels.show();
      delay(SOLID_COLOR_FADE_SPEED);
    }
    pixels.setPixelColor(           i, color1);
    pixels.setPixelColor(NUM_LEDS-1-i, color2);

    if((i+BLANKING_OFFSET)<16)
    {
      pixels.setPixelColor(           (i+BLANKING_OFFSET), 0);
      pixels.setPixelColor(NUM_LEDS-1-(i+BLANKING_OFFSET), 0);
    }

    
    pixels.show();
    delay(SOLID_COLOR_FADE_SPEED);
  }
}
void animate_spin()
{
  uint8_t  i,j;
  int offset = 0;
  
  // Spinny wheels (4 LEDs on at a time)
  for(j=0;j<SPIN_DURATION;j++)
  {
    for(i=0; i<NUM_LEDS_ONE_WHEEL; i++) 
    {
      uint32_t c = 0;
      uint32_t d = 0;
      if(((offset + i) & 7) < 2)
      {
        c = color1; // 4 pixels on...
        d = color2; // 4 pixels on...
      }
      pixels.setPixelColor(   i, c);
      pixels.setPixelColor(NUM_LEDS-1-i, d);
    }
    offset++;
    pixels.show();
    delay(SPIN_SPEED);
  }

  prevTime = millis();
}
