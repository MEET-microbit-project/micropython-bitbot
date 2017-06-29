from microbit import *
import bitbot

#display.scroll("H")

# bitbot.buzz(100)

bitbot.set_speed(50, -50)
sleep(100)

bitbot.set_speed(0, 0)

while(True):
    if bitbot.brightness()[0] < 20:
        bitbot.set_speed(50, -50)
    else:
        bitbot.set_speed(0, 0)
        # bitbot.buzz(50)
    sleep(100)
