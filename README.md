# Advanced Database Systems (CS60113) Coursework

### Assignment 1
Implemented the *insert* and *search* module of R-Tree for various dimensions of data and report a comparative analysis. 
The implementation follows the algorithm proposed in the paper ["R-Tree: A Dynamic Indexing Structure for Spatial Searching"](http://www-db.deis.unibo.it/courses/SI-LS/papers/Gut84.pdf)


### Assignment 2
Implemented *k-Nearest Neighbour* search algorithm for R-Trees. It first creates an R-Tree for a set of points, and then uses the algorithm proposed in the paper ["Nearest Neighbor Queries"](https://dl.acm.org/doi/10.1145/223784.223794) to retrieve `k` nearest neighbours.


### Assignment 3
Implemented an Image and Content based *search engine* for a news filtering. Scrapped news articles from NDTV and Indian Express websites and generated tf-idf vectors for the texts. Images, represented as histograms were stored using R-Tree indexing scheme. Given a *query keyword*, *query image* and `k`, the search engine filters the documents having the query keyword and from the images of the filtered articles, *k-Nearest Neighbour* search algorithm of **Assignment 2** was used to search the images and display them.


### Assignment 4
Presentation and Analysis on the paper ["A critique of ANSI SQL isolation levels"](https://dl.acm.org/doi/10.1145/223784.223785)


### Assignment 5
Comparison of the query time for various database engines. Comapred MySql Database with innodb` and `columnstore` engines, MariaDB and Monet DB. Created a Fact table and 3 Dimension tables, inserted identical values in them and then compared the query time for 5 queries.


### Assignment 6
Implemented several queries for MongoDB using `mongoshell` and `pymongo` to insert and return the records in a collection. We have used a json file of twitter records as the database entries.


### Assignment 7
Implemented a simple Blockchain to calculate the SGPA of a student for a particular semester. Used `truffle` for compiling and `Ganache` as the personal Ethereum Blockchain. Also implemented a javascript file using `web3js` to interact with `Ganache`.