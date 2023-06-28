import time
from datetime import datetime
from PIL import Image
import pytesseract
from ppadb.client import Client as AdbClient
import cv2 as cv

def connect():
    client = AdbClient(host="127.0.0.1", port=5037) # Default is "127.0.0.1" and 5037

    devices = client.devices()

    if len(devices) == 0:
        print('No devices')
        quit()

    device = devices[0]

    print(f'Connected to {device}')

    return device, client


if __name__ == '__main__':
    device, client = connect()

    # open up camera app
    # device.shell('input keyevent 27')

    #take photo
    device.shell('input keyevent 24')

    imagePaths = device.shell("ls /sdcard/DCIM/Camera").split()

    # fitlers out images that take weren't taken on the curr day and don't have the PXL prefix
    todayPXLPathes = sorted([e for e in imagePaths if e[:12] == "PXL_{}".format(datetime.today().strftime("%Y%m%d"))])
    mostRecentImagePath = "/sdcard/DCIM/Camera/{}".format(todayPXLPathes[-1])
    device.pull(mostRecentImagePath, "./mostRecent.jpg")
    device.shell(f"rm {mostRecentImagePath}")

    # OCR
    mostRecentImage = cv.imread('./mostRecent.jpg')
    imgray = cv.cvtColor(mostRecentImage, cv.COLOR_BGR2GRAY)
    ret, thresh = cv.threshold(imgray, 127, 255, 0)
    contours, hierarchy = cv.findContours(thresh, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    cv.drawContours(mostRecentImage, contours, -1, (0, 255, 0), 3)
    cv.imshow("", mostRecentImage)
    cv.waitKey(0)
    cv.destroyAllWindows()

    print(pytesseract.image_to_data(Image.open('./mostRecent.jpg')))
