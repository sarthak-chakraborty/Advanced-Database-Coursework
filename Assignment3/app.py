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
import time

from src import queryToTfidf as tfidf
from src import queryImgToHist as imgHist
from src import makedata


app = Flask(__name__, template_folder='static/templates')
# Compile the cpp files
subprocess.call(["g++","-std=c++14","src/rtree_insert.cpp","-o","insert"])
subprocess.call(["g++","-std=c++14","src/knn_search.cpp","-o","knn"])

@app.route('/', methods=['GET', 'POST'])
def hello():
    if request.method == 'GET':
        return render_template('index.html')
    else:
        keywords = request.form['keywords']
        k = request.form['kn']
        photo = request.files['photo']

        for files in os.listdir(os.getcwd()):
            if files.endswith('jpg') or files.endswith('png') or files.endswith('jpeg'):
                os.remove(files)

        photo.save(photo.filename)

        # Remove Images
        out_images_dir = os.path.join(os.getcwd(), 'static', 'images')
        query_dir = os.path.join(os.getcwd(), 'static', 'query')

        if os.path.exists(out_images_dir):
            shutil.rmtree(out_images_dir)
        os.makedirs(out_images_dir)

        if os.path.exists(query_dir):
            shutil.rmtree(query_dir)
        os.makedirs(query_dir)
        
        
        # Get document similarity
        split_keywords = keywords.split(';')
        related_docs_indices = tfidf.doc_similarity(split_keywords)

        print(related_docs_indices)

        # Compile Image data for related documents
        makedata.make_data(related_docs_indices)
        imgHist.img_to_Hist(os.path.join(os.getcwd(), photo.filename))


        # Insert pics in R-Tree and run kNN query search
        subprocess.call(["./insert"])
        result = subprocess.Popen(["./knn", k], stdout=subprocess.PIPE)
        out = result.stdout.read().decode('utf-8')
        tn = out.split('\n')[:-1]

        rank = int(k)
        for elem in tn:
            t = elem.split()
            src = os.path.join(os.getcwd(),'scrapped','data',t[0],'images','image{}.jpg'.format(t[1]))
            dst = os.path.join(out_images_dir, '{}_{}_image{}.jpg'.format(rank,t[0],t[1]))
            shutil.copyfile(src, dst)
            rank -= 1

        print(tn)
        shutil.copyfile(photo.filename, os.path.join(query_dir,  str(int(time.time())) + '_' + photo.filename))

        return ''


@app.route('/results')
def results():
    mypath = "static/images"
    querypath = "static/query"
    info_f = open('scrapped/info.csv')
    lines = info_f.readlines()
    links = []
    files = []

    for f in sorted(listdir(mypath), key=lambda x:int(x.split('_')[0])):
        files.append(join(mypath, f))
        doc_num = int(f.split('_')[1])
        line = lines[doc_num][:-1].split(',')
        links.append(line[-1])

    for file in os.listdir(querypath):
        query_img_name = join(querypath, file)

    result = {
        'files':files,
        'links':links,
        'query': query_img_name
    }

    return json.dumps(result)


if __name__ == '__main__':
    app.run(debug=True)
