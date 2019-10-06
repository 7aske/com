#!/usr/bin/env python3

import RPi.GPIO as GPIO
import atexit
import time
import sys

running = True

message = ""



GPIO.setmode(GPIO.BCM)

sig = 27
clck = 17
read = 22
    
bitrate = 1 / 2048
if len(sys.argv) == 2:
    bitrate = 1 / int(sys.argv[1])
GPIO.setup(sig, GPIO.OUT)
GPIO.setup(clck, GPIO.OUT)
GPIO.setup(read, GPIO.IN)

def send_message(message):
    for char in message:
        out = char if char not in ["\n", "\0"] else " "
        print(str(out)+" ".ljust(5), end="")
        asbytes = bin(ord(char))[2:].zfill(8)
        for b in asbytes:
            print(b, end="")
            GPIO.output(sig, True if b == "1" else False)
            GPIO.output(clck, True)
            time.sleep(bitrate)
            GPIO.output(sig, False)
            GPIO.output(clck, False)
            time.sleep(bitrate)
            sys.stdout.flush()
        print()


try:
    while running:
        message = input("-> ")
        message += "\n\0"
        send_message(message)
except KeyboardInterrupt:
    running = False


@atexit.register
def cleanup():
    GPIO.output(sig, False)
    GPIO.output(clck, False)
    GPIO.cleanup()
