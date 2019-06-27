#!/usr/bin/env python

# Push button to record from mic. Releasing button plays back recording
# Author: David Jun

from tlc59711 import TLC59711
import pyspi
import RPi.GPIO as GPIO
import time

PIN_BUTTON = 7
leds = TLC59711(pyspi, auto_show=False)

toggle = True
def button_callback(channel):
    if toggle:
        leds[0] = (5000, 15000, 15000)
    else:
        leds[0] = (0,0,0)
    leds.show()
    
    toggle = not toggle
    
  
def main():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(PIN_BUTTON, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(PIN_BUTTON, GPIO.FALLING, callback=button_callback, bouncetime=100)

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
    finally:
        GPIO.cleanup() 
        print "   Cleaned up pins"
 
##########
 
if __name__ == "__main__":
    main()
