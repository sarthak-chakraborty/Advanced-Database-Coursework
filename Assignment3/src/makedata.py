"""
Compiles the histogram representation of all the images
and writes in a single textfile which can be read by 
rtree_insert.cpp
"""

import os
import sys

def make_data(docs):
    scrapped_path = os.path.join(os.getcwd(), 'scrapped', 'data')

    OUT_FILE = 'img_data.txt'
    fd_out = open(OUT_FILE, 'w')

    # for article_folder in os.listdir(scrapped_path):
    for article in docs:
        article_folder = str(article)
        image_dirpath = os.path.join(scrapped_path, article_folder, 'images')

        for image in os.listdir(image_dirpath):
            if image.endswith('.txt'):
                fd_in = open(os.path.join(image_dirpath, image), 'r')

                fd_out.write(article_folder + " " + image[5:-4] + " ")

                for line in fd_in:
                    t = line.split()
                    assert(len(t) == 256)
                    for num in t:
                        fd_out.write(num + " " + num + " ")
                    fd_out.write("\n")
                
                fd_in.close()

    fd_out.close()



