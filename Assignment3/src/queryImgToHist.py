"""
Converts query image into its histogram representation
and saves it in the same directory
"""

import numpy as np
from PIL import Image, ImageOps
import sys
import os


def normalize(arr):
    arr = arr.astype('float')
    minval = arr.min()
    maxval = arr.max()
    if minval != maxval:
        arr -= minval
        arr *= (255.0/(maxval-minval))
    return arr


def img_to_Hist(image):
    img = Image.open(os.path.join(os.getcwd(), image))
    img_grayscale = ImageOps.grayscale(img)

    # arr = np.array(img_grayscale)
    # img2 = Image.fromarray(normalize(arr).astype('uint8'))
    # img3 = ImageOps.equalize(img2, mask=None)

    img3 = img_grayscale

    hist = img3.histogram()

    fd_out = open(os.path.join(os.getcwd(), 'query_image.txt'), 'w')
    for i in range(len(hist)):
        if i == len(hist) - 1:
            fd_out.write(str(hist[i]))
        else:
            fd_out.write("{} ".format(hist[i]))

    fd_out.close()
