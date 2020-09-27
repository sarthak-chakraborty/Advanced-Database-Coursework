import numpy as np
import sys
import os
import nltk
from nltk import word_tokenize
from nltk.corpus import stopwords
from nltk.tokenize import RegexpTokenizer
from nltk.stem import PorterStemmer 
from sklearn.feature_extraction.text import TfidfVectorizer


stop_words = set(stopwords.words('english'))
ps = PorterStemmer()


scrapped_path = os.path.join(os.getcwd(), 'scrapped', 'data')

sentences = []
file_list = []
for article_folder in os.listdir(scrapped_path):
    content_dirpath = os.path.join(scrapped_path, article_folder, 'content.txt')
    file_list.append(content_dirpath)

    with open(content_dirpath, 'r', encoding='latin1') as f:
        f_content = f.read()

        tokenizer = RegexpTokenizer(r'\w+')
        tokens = tokenizer.tokenize(f_content)
        reduced = [token.lower() for token in tokens if token.lower() not in stop_words and  len(token)>2]
        stemmed = [ps.stem(word) for word in reduced]

        sentences.append(" ".join(stemmed))


# sentences = ["an apple a day keeps the doctor away", "please move away", "apple is red"]

tf = TfidfVectorizer(input='content', encoding='latin1', analyzer='word', ngram_range=(1,1),
                     min_df = 0, use_idf=True, smooth_idf=True, sublinear_tf=True)

tfidf_matrix =  tf.fit_transform(sentences)

print(tfidf_matrix.shape)

print(tf.get_feature_names())