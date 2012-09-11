#!/usr/bin/env python

import cv

class Target:

    def __init__(self):
        self.capture = cv.CaptureFromCAM(0)
        cv.NamedWindow("Target", 1)

    def run(self):
        # Capture first frame to get size
        frame = cv.QueryFrame(self.capture)
        frame_size = cv.GetSize(frame)
        color_image = cv.CreateImage(cv.GetSize(frame), 8, 3)
        hsv_image = cv.CreateImage(cv.GetSize(frame), 8, 3)
        thresholded = cv.CreateImage(cv.GetSize(frame), 8, 1)
        thresholded2 = cv.CreateImage(cv.GetSize(frame), 8, 1)


        first = True

        while True:
            closest_to_left = cv.GetSize(frame)[0]
            closest_to_right = cv.GetSize(frame)[1]

            color_image = cv.QueryFrame(self.capture)

            # Smooth to get rid of false positives
            cv.Smooth(color_image, color_image, cv.CV_GAUSSIAN, 3, 0)

            # Convert the image to HSV
            cv.CvtColor(color_image, hsv_image, cv.CV_RGB2HSV)

            hsv_min = cv.Scalar(0, 50, 170, 0)
            hsv_max = cv.Scalar(10, 180, 256, 0)
            hsv_min2 = cv.Scalar(170, 50, 170, 0)
            hsv_max2 = cv.Scalar(256, 180, 256, 0)

            cv.InRangeS(color_image, hsv_min, hsv_max, thresholded)
            cv.InRangeS(color_image, hsv_min2, hsv_max2, thresholded2)
            cv.Or(thresholded, thresholded2, thresholded)

            storage = cv.CreateMemStorage(0)
            cv.ShowImage("Target", thresholded)

            # Listen for ESC key
            c = cv.WaitKey(7) % 0x100
            if c == 27:
                break

if __name__=="__main__":
    t = Target()
    t.run()

