"""
Converts query image into its histogram representation
and saves it in the same directory
"""

import numpy as np
from PIL import Image, ImageOps
import sys
import os

if(len(sys.argv) < 2):
    sys.exit('Image file name required')

image = sys.argv[1]

img = Image.open(os.path.join(os.getcwd(), image))
img_grayscale = ImageOps.grayscale(img)

hist = img_grayscale.histogram()

fd_out = open(os.path.join(os.getcwd(), 'query_image.txt'), 'w')
for i in range(len(hist)):
    if i == len(hist) - 1:
        fd_out.write(str(hist[i]))
    else:
        fd_out.write("{} ".format(hist[i]))


fd_out.close()
