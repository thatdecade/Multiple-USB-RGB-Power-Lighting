# Multiple USB RGB Power Lighting
The goal of this project is to set the color of two neopixel rings based on which USB cable is powered.  I plugged the USB cables into my game consoles, so when a console is powered, the shelf llght will have the corresponding color.

![Neopixel Ring Puck Lights](https://github.com/thatdecade/Three_USB_Color_Light/raw/master/photos/shelf_light_installed.png)

**How it Works**

Plug each USB cable into your separate devices.  The isolation diodes allow the trinket to be powered by any or all of the USB cables.  Periodically, the trinket will read the input pins to determine which USB cable is powered and will play the programmed animation and colors or combination of colors.

The trinket needs pull down resistors to set the idle state of an input pin when a USB cable is not powered or left unattached.

----

**Part List**

* Trinket 5V - <https://amzn.to/3gpQX6z>
* NeoPixel Ring - 16 RGB - <https://amzn.to/34wuUcn>
* Wired Under Cabinet Puck Lights - <https://amzn.to/2YssTdl>
* Backpack PCB - <https://oshpark.com/shared_projects/CATpwF4r>
* Three diodes - <https://octopart.com/search?q=CMF04%28TE12L%2CQ%2CM%29>
* Three 10k resistors - <https://octopart.com/search?q=RC0603FR-0710KL>
* Three USB Cables (dollar store)

The backpack pcb is optional.  You can just as easily wire up the diodes and resistors on a protoboard.  

**Pin Assignments**

Not all pins on the Trinket are equal.  Be sure to read Adafruit's help pages <https://learn.adafruit.com/introducing-trinket/pinouts>  Pins 3 and 4 should not be used as inputs and will conflict with talking to a PC for programming.

* GPIO #0 - Red Input, Attach to +5V wire from the first USB cable
* GPIO #1 - Blue Input, Attach to +5V wire from the second USB cable
* GPIO #2 - Green Input, Attach to +5V wire from the third USB cable
* GPIO #3 - Neopixel Data
* GPIO #4 - Potentiometer (optional)

**Assembly**

If you have access to a 3d printer, an enclosure stl is provided.  Thread the cables through the enclosure sides before soldering.

Solder the resistors and diodes to the backpack, then wire into the trinket.  Strip the USB cables and identify the 5V and Ground wires.  Use a multimeter to be sure, but the 5V and Ground wires can also be identified by thickness.  The power wires should be thicker than the data wires.

*There is a RGB LED and three 220 ohm resistors on the backback pcb for debugging, leave them unpopulated.*

Take apart the puck light and remove the PCB.  Recycling the wiring that came with the puck light to wire into your neopixel rings.  Use hot glue or epoxy to add the neopixel ring behind the puck light's diffuser.

**Programming**

Use the arduino ide and adafruit neopixel library and adafruit trinket board package.  Be sure to read Adafruit's help pages for each module.

* <https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide>
* <https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide>
* <https://github.com/adafruit/Adafruit_NeoPixel>

If you followed my pin assignments, you can use the code without modification.  After you get the neopixels to light up and changing based on which USB cable is powered.   You can explore modifying the colors and animations.

**Usage, Testing, and Setting Colors**

After initial programming, when you plug in each of the three USB cables you should see the ring light up red or blue or green.  Combinations are allowed.

If you installed the optional potentiometer, you can change the color selected by a USB cable without reprogramming using a PC.  You will need two USB power ports to plug into.  
1. Connect the Micro USB on the trinket so power remains on while the other USBs are unplugged.  Leave this plugged in throughout these steps.
2. Take the USB cable you want to configure and plug it into your power block.  
3. Wait 5 seconds (or just wait for the animation to complete)
4. Rapidly unplug and plug the cable in four times, ending with the USB plugged in.
    1. You must complete this four times, before the spinning animation ends.
    2. If you do it right, the animations will stop and you will see a steady light.  
    3. If you mess up, wait 5 seconds before trying again.
5. Slowly rotate the potentiometer to select the color.
6. Unplug the USB cable to save the color selection.

----

**Additional Notes**

I used the NeoPixel ring and Trinket I listed above because that is what I had extra.  If I were to make one from scratch, I would use the [RGBW Neopixel Rings](https://octopart.com/search?q=adafruit+2854) and [Arduino NANO 33 IoT](https://amzn.to/3lg6Tfj) instead.  So I could use the Puck Light for normal lighting with commands to Alexa to change colors too!  

More Info: <https://blog.arduino.cc/2019/11/12/create-a-voice-controlled-device-with-alexa-and-arduino-iot-cloud-in-7-minutes/>



_We are a participant in the Amazon Services LLC Associates Program, an affiliate advertising program designed to provide a means for us to earn fees by linking to Amazon.com and affiliated sites._

