# idea

- digital sample and hold eurorack module
- nice adc and dac
- feed it cv and clock and it will spit out that sequence at the same or different clocks
- steve reich inspired fading will ensue

# hardware

- stm? atmega?
- cv out maybe like [mi yarns](https://github.com/pichenettes/eurorack/tree/master/yarns) or [o_C](https://ornament-and-cri.me/) or [stages](https://github.com/pichenettes/eurorack/tree/master/stages). For stages there are some nice [calibration notes](https://github.com/forestcaver/MIStagesStuff/blob/master/__CalibrationNotes.txt)
- cv in could be like o_C
- per channel:
  - one encoder with button
  - one 7 segment
  - cv in 
  - cv out
  - clock in
  
- [dev notes from emile](https://forum.mutable-instruments.net/t/general-questions-on-diy-synthesizers-w-stm32/15248/2)

- [great intro on how to get started with stm32 and eurorack by cutlasses music](http://www.cutlasses.co.uk/tech/developing-on-stm32/)

- ![neat dac](https://github.com/digitalmediabremen/tsateacs-comp/blob/main/img/stages_dac.png)
- ![neat buffered output with leds, -8.1v to 8.1v](https://github.com/digitalmediabremen/tsateacs-comp/blob/main/img/stages_vref+output.png)

- [great intro into eurorack module design + stm32](https://www.youtube.com/watch?v=Xf4CxC8exFM)

- time is getting tight. gonna Dev the thing on Ornament and Crime
	- gives me a good hardwarebase, og module will become a side project


# needed libs

- encoder
- audiolib?
- ledmatrixdriver


# needed pins

- 2x encoder
	- each takes two pins
- 2x encoder LED (PWM would be nice)
- LED matrix (Serial via i2c)
- dac (i2s, I guess?)
- 2x clock in (stupid digital is fine)
- 2x analog in (neg. 8V - pos. 8V, 16 octaves, that should be plenty.)
- 2x analog out (same as above)
- 2x encoder switch

# needed parts/living BOM

- 1x Teensy 3.5
- 1x mcp6002
- 1x tl072
- 2x lpt305 matrix displays
- 1x IS31FL3730 matrix driver
- 2x encoder with led and switch
- 1x lm4040 voltage ref.
- 1x some dac, using internal one rn
- 1x eurorack header

