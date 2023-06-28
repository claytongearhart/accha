import pytesseract
from PIL import Image
import os

currDir = os.getcwd()
print(pytesseract.image_to_data(Image.open('./mostRecent 3.jpg'), lang="eng", config= f'--psm 6'))
