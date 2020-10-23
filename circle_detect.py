import numpy as np
import argparse
from cv2 import cv2
import time
#from distance import dist

def detect():
    #'/dev/video2'
    cap = cv2.VideoCapture(0) #'/dev/video2'
    while cap.isOpened():

        _,frame = cap.read()
        if not _ :
            break

        output = frame.copy()

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        lower_red= np.array([10,153,101])
        upper_red= np.array([179,255,255])

        mask = cv2.inRange(hsv, lower_red, upper_red)

        res= cv2.bitwise_and(frame, frame, mask=mask)

        gray = cv2.cvtColor(res,cv2.COLOR_BGR2GRAY)
        gray = cv2.Canny(gray,100,200)
        gray = cv2.GaussianBlur(gray,(5,5),0)
        gray = cv2.medianBlur(gray,5)

        kernel = np.ones((2,2),np.uint8)
        
        gray = cv2.dilate(gray,None,iterations = 1)
        gray = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel)
        
        kernel = np.ones((1,1),np.uint8)
        gray = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel)

        circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 0.9, 200, param1=70, param2=45, minRadius=0, maxRadius=0)

        if circles is not None:
            circles = np.round(circles[0, :]).astype("int")
            for (x, y, r) in circles:
                cv2.circle(output, (x, y), r, (0, 255, 0), 4)
                cv2.circle(output,(x,y),3,(0,255,255),-1)
            
                print ("Column Number:{}".format(x))
                print ("Row Number: {}".format(y))
                print ("Radius is: {}".format(r))
                #dist(frame,[x,y])

        cv2.imshow('gray',gray)
        cv2.imshow('frame',output)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()
if __name__ == "__main__":
    detect()