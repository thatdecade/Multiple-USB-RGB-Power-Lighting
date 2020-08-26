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
#include <EEPROM.h>

#define ENABLE_COLOR_KNOB   1        //Set to 0 if you do not plan to use a potentiometer for color selection

#define NUMBER_OF_TOGGLES_TO_ENTER_SETUP 7
#define SETUP_ENTRY_TIMEOUT 5000

#define SPIN_SPEED 60
#define SPIN_DURATION 80

#define SOLID_COLOR_FADE_SPEED 10
#define BLANKING_OFFSET 4

#define RED_PIN   0
#define BLUE_PIN  1
#define GREEN_PIN 2
#define NEO_PIN   3
#define POT_PIN   A2

#define NUM_LEDS 32
#define NUM_LEDS_ONE_WHEEL 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, NEO_PIN);

uint32_t colorwheel[] =
{
  0xff0000, //0 red
  0x800000, //1 maroon
  0xff4000, //2 orange
  0xffff00, //3 yellow
  0x808000, //4 olive
  0x800080, //5 purple
  0xff00ff, //6 fuschia
  0xffffff, //7 white
  0x000080, //8 navy
  0x0000ff, //9 blue
  0x00ffff, //10 aqua
  0x008080, //11 teal
  0x00ff00, //12 lime
  0x008000, //13 green
};

#define WHEEL_RED_INDEX    0
#define WHEEL_BLUE_INDEX   9
#define WHEEL_GREEN_INDEX  13

uint32_t color1, color2;

uint32_t prevTime;

#define RED_EEPROM_ADDRESS    0
#define GREEN_EEPROM_ADDRESS  4
#define BLUE_EEPROM_ADDRESS   8

uint32_t user_color[3];

#define USER_RED_INDEX    0
#define USER_GREEN_INDEX  1
#define USER_BLUE_INDEX   2

typedef enum
{
  START = 0,
  SPINNING,
  FADING,
  IDLE_MODE,
  SETUP_MODE_RED,
  SETUP_MODE_BLUE,
  SETUP_MODE_GREEN,
} mode_e;

mode_e system_mode = START;

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
  pinMode(POT_PIN,   INPUT);

  //set starup colors
  user_color[USER_RED_INDEX]   = get_color_from_index(WHEEL_RED_INDEX);
  user_color[USER_GREEN_INDEX] = get_color_from_index(WHEEL_GREEN_INDEX);
  user_color[USER_BLUE_INDEX]  = get_color_from_index(WHEEL_BLUE_INDEX);
  color1  = user_color[USER_BLUE_INDEX];
  color2  = user_color[USER_BLUE_INDEX];

  read_colors_from_eeprom();
  
  prevTime = millis();

  system_mode = START;
}

void loop() {

  check_for_setup_entry();

  check_for_input_changes();

  update_state_machine();

  draw_animation();
}

void check_for_setup_entry()
{
  static uint32_t previous_user_interaction = 0;
  static uint8_t user_toggle_counter = 0;
  static uint8_t previous_user_value = 0;
  static uint8_t current_user_value = 0;

  current_user_value = (digitalRead(GREEN_PIN) << 1) + (digitalRead(RED_PIN) << 0) + (digitalRead(BLUE_PIN) << 2);

  if(current_user_value != previous_user_value)
  {
    previous_user_value = current_user_value;
    user_toggle_counter++;
    previous_user_interaction = millis();
  }

  if(user_toggle_counter > NUMBER_OF_TOGGLES_TO_ENTER_SETUP)
  {
    user_toggle_counter = 0;

    if(current_user_value == 1)
    {
      setup_mode(RED_PIN, USER_RED_INDEX, RED_EEPROM_ADDRESS);
    }
    
    if(current_user_value == 2)
    {
      setup_mode(GREEN_PIN, USER_GREEN_INDEX, GREEN_EEPROM_ADDRESS);
    }
    
    if(current_user_value == 4)
    {
      setup_mode(BLUE_PIN, USER_BLUE_INDEX, BLUE_EEPROM_ADDRESS);
    }
  }

  if((previous_user_interaction + SETUP_ENTRY_TIMEOUT) < millis())
  {
    user_toggle_counter = 0;
    previous_user_interaction = millis();
  }
  
}

void setup_mode(int pin, uint8_t index, uint8_t eeprom_address)
{
  int analog = 0;
  uint8_t i;
 
  //continue to read pot until USB is pulled
  while(digitalRead(pin) == HIGH)
  {
    //read POT set colors
    analog = analogRead(POT_PIN);

    pixels.setBrightness(45);
    
    for(i=0; i<NUM_LEDS; i++) 
    {
      pixels.setPixelColor(i, lookup_color_from_analog(analog));
    }
    pixels.show();
    delay(500);
  }

  //write to eeprom
  user_color[index] = lookup_color_from_analog(analog);
  EEPROM.write(eeprom_address, map(analog, 0, 1024, 0, ( sizeof( colorwheel ) / sizeof( uint32_t ) ) ));
}

void read_colors_from_eeprom()
{
  if(ENABLE_COLOR_KNOB)
  {
    user_color[USER_RED_INDEX]   = EEPROM.read(RED_EEPROM_ADDRESS);
    user_color[USER_GREEN_INDEX] = EEPROM.read(GREEN_EEPROM_ADDRESS);
    user_color[USER_BLUE_INDEX]  = EEPROM.read(BLUE_EEPROM_ADDRESS);
  
    if(user_color[USER_RED_INDEX] > ( sizeof( colorwheel ) / sizeof( uint32_t ) ))
    {
      user_color[USER_RED_INDEX] = WHEEL_RED_INDEX;
      
      EEPROM.write(RED_EEPROM_ADDRESS, WHEEL_RED_INDEX);
    }
    if(user_color[USER_GREEN_INDEX] > ( sizeof( colorwheel ) / sizeof( uint32_t ) ))
    {
      user_color[USER_GREEN_INDEX] = WHEEL_GREEN_INDEX;
      
      EEPROM.write(GREEN_EEPROM_ADDRESS, WHEEL_GREEN_INDEX);
    }
    if(user_color[USER_BLUE_INDEX] > ( sizeof( colorwheel ) / sizeof( uint32_t ) ))
    {
      user_color[USER_BLUE_INDEX] = WHEEL_BLUE_INDEX;
      
      EEPROM.write(BLUE_EEPROM_ADDRESS, WHEEL_BLUE_INDEX);
    }

    user_color[USER_RED_INDEX]   = get_color_from_index(user_color[USER_RED_INDEX]);
    user_color[USER_GREEN_INDEX] = get_color_from_index(user_color[USER_GREEN_INDEX]);
    user_color[USER_BLUE_INDEX]  = get_color_from_index(user_color[USER_BLUE_INDEX]);
  }
}

void write_colors_from_eeprom(uint8_t red_idx, uint8_t green_idx, uint8_t blue_idx)
{
    EEPROM.write(RED_EEPROM_ADDRESS,   red_idx);
    EEPROM.write(GREEN_EEPROM_ADDRESS, green_idx);
    EEPROM.write(BLUE_EEPROM_ADDRESS,  blue_idx);
}

uint32_t get_color_from_index(uint8_t index)
{
  //perform lookup in a function to reduce program size
  return colorwheel[index];
}

uint32_t lookup_color_from_analog(int analog)
{
  return get_color_from_index(map(analog, 0, 1024, 0, ( sizeof( colorwheel ) / sizeof( uint32_t ) ) ));
}

void update_state_machine()
{
  static uint32_t previous_color1 = 0;
  static uint32_t previous_color2 = 0;
  
  //reset animations
  if( (color1 != previous_color1) ||
      (color2 != previous_color2))
  {
    previous_color1 = color1;
    previous_color2 = color2;

    system_mode = SPINNING;
  }
}

void draw_animation()
{
  if(system_mode == SPINNING)
  {
    animate_spin();
  }
  else if (system_mode == FADING)
  {
    solid_color_fade_down();
  }
}

void check_for_input_changes()
{
  //read inputs and set color

  //NONE
  
  if((digitalRead(GREEN_PIN) == LOW) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = 0x0;
    color2 = 0x0;
  }
  
  //SINGLES
  
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = user_color[USER_GREEN_INDEX];
    color2 = user_color[USER_GREEN_INDEX];
  }
  if((digitalRead(GREEN_PIN) == LOW) && 
     (digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = user_color[USER_RED_INDEX];
    color2 = user_color[USER_RED_INDEX];
  }
  if((digitalRead(GREEN_PIN) == LOW) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    color1 = user_color[USER_BLUE_INDEX];
    color2 = user_color[USER_BLUE_INDEX];
  }

  //DOUBLES
  
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == LOW))
  {
    color1 = user_color[USER_GREEN_INDEX];
    color2 = user_color[USER_RED_INDEX];
  }
  if((digitalRead(GREEN_PIN) == HIGH) && 
     (digitalRead(RED_PIN) == LOW) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    color1 = user_color[USER_BLUE_INDEX];
    color2 = user_color[USER_GREEN_INDEX];
  }
  if((digitalRead(RED_PIN) == HIGH) && 
     (digitalRead(BLUE_PIN) == HIGH))
  {
    //DOUBLES and TRIPLES, Favor blue RED, even if green is also ON
    color1 = user_color[USER_BLUE_INDEX];
    color2 = user_color[USER_RED_INDEX];
  }
}

void animate_spin()
{
  static uint8_t i = 0;
  static uint8_t j = 0;
  static int offset = 0;
  static mode_e previous_system_mode = START;

  if(previous_system_mode != system_mode)
  {
    i = 0;
    j = 0;
    previous_system_mode = system_mode;
  }

  if((system_mode == SPINNING) && ((prevTime + SPIN_SPEED) < millis()))
  {
    prevTime = millis();
    
    // Spinny wheels (4 LEDs on at a time)
  
    pixels.setBrightness(254);
    
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

    j++;
    if(j >= SPIN_DURATION)
    {
      system_mode = FADING;
      previous_system_mode = system_mode;
    }
  }
}

void solid_color_fade_down()
{
  static uint8_t i = 0;
  static uint8_t j = 0;
  static mode_e previous_system_mode = START;
  static uint32_t previous_primary_color = 0xff0000;
  
  if(previous_system_mode != system_mode)
  {
    i = 0;
    j = 0;
    previous_system_mode = system_mode;
  }

  if((system_mode == FADING) && ((prevTime + SOLID_COLOR_FADE_SPEED) < millis()))
  {
    prevTime = millis();
    
    //set to solid color, while also dimming gradually.

    pixels.setBrightness(map(i*15+j,0,254,254,45));

    j++;
    if(j >= 16)
    {
      j = 0;
      pixels.setPixelColor(           i, color1);
      pixels.setPixelColor(NUM_LEDS-1-i, color2);
  
      if((i+BLANKING_OFFSET)<16)
      {
        pixels.setPixelColor(           (i+BLANKING_OFFSET), 0);
        pixels.setPixelColor(NUM_LEDS-1-(i+BLANKING_OFFSET), 0);
      }
  
      i++;
      if(i >= 16)
      {
        system_mode = IDLE_MODE;
        previous_system_mode = system_mode;
      }
    }
    
    pixels.show();
  }
}

