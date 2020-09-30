import numpy as np
import sys
import os
import nltk
from nltk import word_tokenize
from nltk.corpus import stopwords
from nltk.tokenize import RegexpTokenizer
from nltk.stem import PorterStemmer 
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import linear_kernel
import pickle

ps = PorterStemmer()

def doc_similarity(keywords):
    processed_keyword = [ps.stem(word.lower()) for word in keywords]
    query_doc = " ".join(processed_keyword)

    with open('src/tf.pkl', 'rb') as f_in:
        tf = pickle.load(f_in)

    with open('src/tfidf_matrix.pkl', 'rb') as f_in:
        tfidf_matrix = pickle.load(f_in)

    X = tf.transform([query_doc])
    
    cosine_similarities = linear_kernel(X, tfidf_matrix).flatten()
    related_docs_indices = cosine_similarities.argsort()[-50:][::-1]

    return related_docs_indices+1

