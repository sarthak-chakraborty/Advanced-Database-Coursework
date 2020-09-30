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


stop_words = set(stopwords.words('english'))
ps = PorterStemmer()


def calc_tfidf_matrix():
    scrapped_path = os.path.join(os.getcwd(), '..', 'scrapped', 'data')

    sentences = []
    file_list = []
    for article_folder in sorted(os.listdir(scrapped_path), key=int):
        content_dirpath = os.path.join(scrapped_path, article_folder, 'content.txt')
        file_list.append(content_dirpath)

        with open(content_dirpath, 'r', encoding='latin1') as f:
            f_content = f.read()

            tokenizer = RegexpTokenizer(r'\w+')
            tokens = tokenizer.tokenize(f_content)
            reduced = [token.lower() for token in tokens if token.lower() not in stop_words and  len(token)>2]
            tagged = nltk.pos_tag(reduced)
            stemmed = [ps.stem(word[0]) for word in tagged if word[1]=='NN' or word[1]=='NNS' or word[1]=='NNP' or word[1]=='NNPS']

            sentences.append(" ".join(stemmed))

    tf = TfidfVectorizer(input='content', encoding='latin1', analyzer='word', ngram_range=(1,3),
                        min_df = 0, use_idf=True, smooth_idf=True, sublinear_tf=True)
    tfidf_matrix =  tf.fit_transform(sentences)

    print(tfidf_matrix.shape)

    return tfidf_matrix, tf


def doc_similarity(keywords):
    tfidf_matrix, tf = calc_tfidf_matrix()

    processed_keyword = [ps.stem(word.lower()) for word in keywords]
    query_doc = " ".join(processed_keyword)
    # print(query_doc)
    X = tf.transform([query_doc])

    # print(X.shape)

    cosine_similarities = linear_kernel(X, tfidf_matrix).flatten()
    # print(cosine_similarities.shape)
    related_docs_indices = cosine_similarities.argsort()[-10:][::-1]

    print(related_docs_indices + 1)
    print(cosine_similarities[related_docs_indices])

    return related_docs_indices+1
