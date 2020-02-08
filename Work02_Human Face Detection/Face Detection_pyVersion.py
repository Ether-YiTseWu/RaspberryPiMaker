# Import library
from picamera.array import PiRGBArray
from picamera       import PiCamera
import time
import cv2

faceCascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

cam            = PiCamera()
cam.resolution = (320, 240)
cam.framerate  = 30
raw            = PiRGBArray(cam, size=(320, 240))

time.sleep(0.1) # Camera warm up

for frameBGR in cam.capture_continuous(raw, format="bgr", use_video_port=True):
    imgBGR = frameBGR.array # num.py array
    imgBW  = cv2.cvtColor(imgBGR, cv2.COLOR_BGR2GRAY)
    listFace = faceCascade.detectMultiScale(imgBW, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30), flags = cv2.CASCADE_SCALE_IMAGE)
    for (x, y, w, h) in listFace:
        cv2.rectangle(imgBGR, (x, y), (x+w, y+h), (255, 255, 0), 4)

    print (len(listFace))
    cv2.imshow('Video', imgBGR)
    key = cv2.waitKey(1) & 0xFF

    raw.truncate(0) # Clear stream, a must!

    if key == ord("q"): # Press 'q' to quit
        break
