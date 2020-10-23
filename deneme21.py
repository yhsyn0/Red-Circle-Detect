from cv2 import cv2
import numpy as np
import imutils

low_red = np.array([50, 150, 30]) #üst ve alt kırmızı değerleri
high_red = np.array([180, 255, 255])

cap= cv2.VideoCapture(0)

kernelOpen = np.ones((5,5)) #morphology alan ayarları
kernelClose = np.ones((20,20))
rect= False
try:
    
    while True:
        grabbed, frame= cap.read()
        status= "HEDEF YOK"

        if not grabbed:
            continue

        frame=cv2.resize(frame, (340,220))
        hsv=cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask=cv2.inRange(hsv, low_red, high_red)
        res= cv2.bitwise_and(frame, frame, mask=mask)

        maskOpen=cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernelOpen)
        maskClose=cv2.morphologyEx(maskOpen, cv2.MORPH_CLOSE, kernelClose) #pürüzleri yok et.

        maskSon=maskClose

        circ_img = frame.copy()
        circles = cv2.HoughCircles(res, cv2.HOUGH_GRADIENT, 1, 10, np.array([]), 200, 100, 100, 200)

        if circles == 1:   #kırmızı nesnenin daire olup olmadiğnı kontrol et

            resim, cnts, h= cv2.findContours(maskSon.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)# son pürüzsüz resimdeki kontürü bul
            cv2.drawContours(frame, cnts, -1, (0, 255, 0), 3) #kontür çizildi

            for c in range(len(cnts)):
                x,y,w,h=cv2.boundingRect(cnts(c))
                cv2.rectangle=(frame, (x,y), (x+w, y+h), (0,0,255), 2) #kontür dışına dikdörtgen cizdirme

            for c in cnts:
                M=cv2.moments(c)
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                cv2.circle(circ_img, (cX, cY), 7, (255, 0, 0), -1)  #merkezi işaretle

            status="HEDEFE ULASİLDİ"  #durum güncellendi. 
            cv2.putText(frame, status, (cX - 20, cY - 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 2)  #durum metni framin sol üst köşesine yazdırıldı.

            rect= True

            cv2.imshow("TESPİT", frame)
            cv2.imshow("Mask", maskSon)

            key=cv2.waitKey(1) & 0xFF

            if key== ord("q"):
                break

    cap.release()
    cv2.destroyAllWindows()
                    
except KeyboardInterrupt:
    cv2.destroyAllWindows()
    
    
	


	            


    
    
