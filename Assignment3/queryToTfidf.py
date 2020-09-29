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

query = 'Donald Trump'

tokenizer = RegexpTokenizer(r'\w+')

keywords = tokenizer.tokenize(query)

processed_keyword = [ps.stem(word.lower()) for word in keywords]
query_doc = " ".join(processed_keyword)
print(query_doc)

with open('tf.pkl', 'rb') as f_in:
    tf = pickle.load(f_in)

with open('tfidf_matrix.pkl', 'rb') as f_in:
    tfidf_matrix = pickle.load(f_in)

X = tf.transform([query_doc])

print(tfidf_matrix.shape)
print(X.shape)

cosine_similarities = linear_kernel(X, tfidf_matrix).flatten()
print(cosine_similarities.shape)
related_docs_indices = cosine_similarities.argsort()[-10:][::-1]

print(related_docs_indices + 1)
print(cosine_similarities[related_docs_indices])
