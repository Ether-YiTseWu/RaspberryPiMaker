import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
led = 11

GPIO.setmode(GPIO.BOARD)
GPIO.setup(led, GPIO.OUT)
GPIO.output(led, 1)