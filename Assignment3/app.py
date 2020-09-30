from flask import Flask
from flask import render_template
from os import listdir
import json
from os.path import isfile, join
from flask import Flask, request
import subprocess
from subprocess import Popen, PIPE

from src import tfidf
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
        
        # Get document similarity
        split_keywords = keywords.split(';')
        related_docs_indices = tfidf.doc_similarity(split_keywords)

        # Compile Image data for relted documents
        makedata.make_data(related_docs_indices)

        # Insert pics in R-Tree and run kNN query search
        subprocess.Popen(["./insert"])
        result = subprocess.Popen(["./knn", k], stdout=subprocess.PIPE)

        out = result.stdout.read()

        return ''


@app.route('/results')
def results():
    mypath = "static/images"
    onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    return json.dumps(onlyfiles)


if __name__ == '__main__':
    app.run(debug=True)
