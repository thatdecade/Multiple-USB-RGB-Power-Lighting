# Three_USB_Color_Light
The goal of this project is to set the color of two neopixel rings based on which USB cable is powered.  I plugged the USB cables into my game consoles, so when a console is powered, the shelf llght will have the corresponding color.

**Part List**

* Trinket 5V - <https://amzn.to/3gpQX6z>
* NeoPixel Ring - 16 RGB - <https://amzn.to/34wuUcn>
* Wired Under Cabinet Puck Lights - <https://amzn.to/2YssTdl>
* Backpack PCB - <https://oshpark.com/shared_projects/CATpwF4r>
* Three diodes - <https://octopart.com/search?q=CMF04%28TE12L%2CQ%2CM%29>
* Three 10k resistors - <https://octopart.com/search?q=RC0603FR-0710KL>
* Three USB Cables (dollar store)

**Pin Assignments**

Not all pins on the Trinket are equal.  Be sure to read Adafruit's help pages <https://learn.adafruit.com/introducing-trinket/pinouts>  Pins 3 and 4 should not be used as inputs and will conflict with talking to a PC for programming.

* GPIO #0 - Red Input, Attach to +5V wire from the first USB cable
* GPIO #1 - Blue Input, Attach to +5V wire from the second USB cable
* GPIO #2 - Green Input, Attach to +5V wire from the third USB cable
* GPIO #3 - Neopixel Data

**Assembly**

Solder the resistors and diodes to the backpack, then wire into the trinket.  Strip the USB cables and identify the 5V and Ground wires.  Use a multimeter to be sure, but the 5V and Ground wires can also be identified by thickness.  The power wires should be thicker than the data wires.

*There is a RGB LED and three 220 ohm resistors on the backback pcb for debugging, leave them unpopulated.*

Take apart the puck light and remove the PCB.  Recycling the wiring that came with the puck light to wire into your neopixel rings.  Use hot glue or epoxy to add the neopixel ring behind the puck light's diffuser.

----

**How it Works**

Plug each USB cable into your separate devices.  The trinket can be powered by any or all of the USB cables.  Periodically, the trinket will read the input pins to determine which USB cable is powered and will play the programmed animation and colors.

The pull down resistors and diodes help setup the conditions on the input pins, so the trinket can both be powered by any of the USB cables while also reading which USB cable is providing power.

----

**Additional Notes**

I used the NeoPixel ring I listed above because that is what I had extra.  If I were to make one from scratch, I would use the [RGBW Neopixel Rings](https://octopart.com/search?q=adafruit+2854) instead.  So I could use the Puck Light for normal lighting too.

The backpack pcb is optional.  You can just as easily wire up the diodes and resistors on some protoboard.  


