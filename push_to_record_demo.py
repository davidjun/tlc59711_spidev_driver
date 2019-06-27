#!/usr/bin/env python3

# Push button to record from mic. Releasing button plays back recording
# Author: David Jun

from tlc59711 import TLC59711
import pyspi
import RPi.GPIO as GPIO
import time
import alsaaudio

GPIO.setwarnings(False)

PIN_BUTTON = 7
leds = TLC59711(pyspi, auto_show=False)
microphone = "plughw:2,0"

PUSHED = 0
RELEASED = 1
IDLE = -1
button_state = IDLE

def button_callback(channel):
    global button_state
    global PUSHED
    global RELEASED

    if(channel == PIN_BUTTON):
        val = GPIO.input(channel)
        if(val == 0):
            # pushed
            button_state = PUSHED
            leds[0] = (5000, 15000, 15000)
        else:
            # released
            button_state = RELEASED
            leds[0] = (0,0,0)
        leds.show()    
  
def main():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(PIN_BUTTON, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(PIN_BUTTON, GPIO.BOTH, callback=button_callback, bouncetime=300)

    try:
        while True:
            if(button_state == PUSHED):
                inp = alsaaudio.PCM(alsaaudio.PCM_CAPTURE, alsaaudio.PCM_NORMAL, microphone)
                inp.setchannels(1)
                inp.setrate(16000)
                inp.setformat(alsaaudio.PCM_FORMAT_S16_LE)
                inp.setperiodsize(500)
                audio = ""
                leds[1] = (15000, 0, 0)
                leds.show()
                while(GPIO.input(PIN_BUTTON)==0): # we keep recording while the button is pressed
                    l, data = inp.read()
                    if l:
                        audio += data
                rf = open('/home/pi/data/recording.wav', 'w')
                rf.write(audio)
                rf.close()
                inp = None
                leds[1] = (0, 0, 0)
                leds.show()

                button_state = IDLE

            # TODO: do a sanity check (state of led vs pushbutton and fix here)
            time.sleep(0.2)
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
    finally:
        GPIO.cleanup() 
        print("   Cleaned up pins")
 
##########
 
if __name__ == "__main__":
    main()
