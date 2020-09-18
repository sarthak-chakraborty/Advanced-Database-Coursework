import numpy as np
from PIL import Image, ImageOps
import sys
import os

scrapped_path = os.path.join(os.getcwd(), 'scrapped', 'data')

for article_folder in os.listdir(scrapped_path):
    print(article_folder)
    image_dirpath = os.path.join(scrapped_path, article_folder, 'images')

    for image in os.listdir(image_dirpath):
        if image.endswith('.txt'):
            continue
        img = Image.open(os.path.join(image_dirpath, image))
        img_grayscale = ImageOps.grayscale(img)

        hist = img_grayscale.histogram()

        fd_out = open(os.path.join(image_dirpath, image[:-4]+'.txt'), 'w')
        for num in hist:
            if num == hist[-1]:
                fd_out.write(str(num))
            else:
                fd_out.write("{} ".format(num))

        fd_out.close()
