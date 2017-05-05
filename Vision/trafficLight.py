from operator import itemgetter
import RPi.GPIO as pio
import numpy as np
import picamera
import cv2
import io

def inColorRange(color, low, high):
    if (color[0] <= high[0] and color[1] <= high[1] and color[2] <= high[2]):
        if (color[0] >= low[0] and color[1] >= low[1] and color[2] >= low[2]):
            return True
    return False

def takePicture(camera):
    stream = io.BytesIO()
    camera.capture(stream, format='jpeg')

    buff = np.fromstring(stream.getvalue(), dtype=np.uint8)

    return cv2.imdecode(buff, 1)

def findLights(img):
    redPin = False
    ylwPin = False
    grnPin = False
    
    img = cv2.medianBlur(img, 5)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    circles = cv2.HoughCircles(gray, cv2.cv.CV_HOUGH_GRADIENT, 1, minDist=40, param1=50, param2=30, minRadius=0, maxRadius=30)
    #print 'Checked'

    if circles is not None:
        circles = np.round(circles[0, :]).astype('int')
        circles = sorted(circles, key=itemgetter(0))

        i = 0
        trafficCircles = None
        while i + 2 < len(circles):

            if circles[i][0] + 16 >= circles[i + 2][0]:
                trafficCircles = [circles[i], circles[i + 1], circles[i + 2]]
                break
            i = i + 1
        
        if trafficCircles is not None:
            meanv = [0, 0, 0]
            index = 0
            trafficCircles = sorted(trafficCircles, key=itemgetter(1))
            for (x, y, r) in trafficCircles:
                cv2.circle(img, (x, y), r, (255, 0, 0), 2)
                #print(str(x) + ',' + str(y) + ': ' + str(r))

                ###Check average color of area
                roi = hsv[y - r / 3 : y + r / 3, x - r / 3 + 1 : x + r / 3 + 1]

                ###Modify hsv value
                roi[:, :, 0] = (roi[:, :, 0] + 65) % 180
                
                mean = cv2.mean(roi)
                #print mean

                meanv[index] = mean[2]
                index = index + 1

            if (meanv[0] > meanv[1] and meanv[0] > meanv[2] and meanv[0] > 200):
                redPin = True
                ylwPin = False
                grnPin = False
            elif (meanv[1] > meanv[0] and meanv[1] > meanv[2] and meanv[1] > 200):
                redPin = False
                ylwPin = True
                grnPin = False
            elif (meanv[2] > meanv[1] and meanv[2] > meanv[1] and meanv[2] > 180):
                redPin = False
                ylwPin = False
                grnPin = True
            print meanv
            
            #'''
            cv2.imshow('img', img)
            cv2.waitKey(0)
            cv2.destroyAllWindows()
            #'''
    #print redPin
    #print ylwPin
    #print grnPin
    return (redPin, ylwPin, grnPin)

def findCircles(img):
    img = cv2.medianBlur(img, 5)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    circles = cv2.HoughCircles(gray, cv2.cv.CV_HOUGH_GRADIENT, 1, minDist=35, param1=25, param2=40, minRadius=0, maxRadius=30)
    print 'Checked'

    if circles is not None:
        circles = np.round(circles[0, :]).astype('int')
        circles = sorted(circles, key=itemgetter(0))

        for (x, y, r) in circles:
            cv2.circle(img, (x, y), r, (0, 0, 0), 2)
        '''
        cv2.imshow('img', img)
        cv2.waitKey(0)
        cv2.destroyAllWindows()
        '''

#Entry Point

pio.setmode(pio.BCM)
redPin = 2
ylwPin = 3
grnPin = 4

pio.setup(redPin, pio.OUT)
pio.setup(ylwPin, pio.OUT)
pio.setup(grnPin, pio.OUT)

img = cv2.imread('TrafficLight2.jpg')

with picamera.PiCamera() as camera:
    camera.resolution = (320, 240)
    
    while True:
        for i in range(5):
            pins = findLights(takePicture(camera))

            if 1 in pins:
                break
            
        pio.output(redPin, pins[0])
        pio.output(ylwPin, pins[1])
        pio.output(grnPin, pins[2])
