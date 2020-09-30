from flask import Flask
from flask import render_template
from os import listdir
import os
import json
from os.path import isfile, join
from flask import Flask, request
import subprocess
from subprocess import Popen, PIPE
import shutil

from src import queryToTfidf as tfidf
from src import queryImgToHist as imgHist
from src import makedata


app = Flask(__name__, template_folder='static/templates')


@app.route('/', methods=['GET', 'POST'])
def hello():
    if request.method == 'GET':
        return render_template('index.html')
    else:
        keywords = request.form['keywords']
        k = request.form['kn']
        photo = request.files['photo']
        photo.save(photo.filename)

        # Remove Images
        out_images_dir = os.path.join(os.getcwd(), 'static', 'images')
        if os.path.exists(out_images_dir):
            shutil.rmtree(out_images_dir)
            os.makedirs(out_images_dir)
        
        # Get document similarity
        split_keywords = keywords.split(';')
        related_docs_indices = tfidf.doc_similarity(split_keywords)

        print(related_docs_indices)

        # Compile Image data for relted documents
        makedata.make_data(related_docs_indices)
        imgHist.img_to_Hist(os.path.join(os.getcwd(), photo.filename))

        # Compile the cpp files
        subprocess.call(["g++","-std=c++14","src/rtree_insert.cpp","-o","insert"])
        subprocess.call(["g++","-std=c++14","src/knn_search.cpp","-o","knn"])

        # Insert pics in R-Tree and run kNN query search
        subprocess.call(["./insert"])
        print(k)
        result = subprocess.Popen(["./knn", k], stdout=subprocess.PIPE)
        out = result.stdout.read().decode('utf-8')
        tn = out.split('\n')[:-1]

        for elem in tn:
            t = elem.split()
            src = os.path.join(os.getcwd(),'scrapped','data',t[0],'images','image{}.jpg'.format(t[1]))
            dst = os.path.join(out_images_dir, '{}_image{}.jpg'.format(t[0],t[1]))
            shutil.copyfile(src, dst)

        print(tn)

        return ''


@app.route('/results')
def results():
    mypath = "static/images"
    onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    return json.dumps(onlyfiles)


if __name__ == '__main__':
    app.run(debug=True)
