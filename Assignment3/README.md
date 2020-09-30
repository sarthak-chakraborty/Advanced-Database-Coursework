## R-Tree based Search Engine

Sarthak Chakraborty, Omar Eqbal, Nikhil Nayan Jha, Vivek Gupta, Rohan Iswarkar


### Intructions
- Use `crawler_ndtv.py` and `crawler_indianEx.py` to crawl the news sites and store the result in `scrapped/` directory.
- The organization of the data directory must be as follows:
	    
        scrapped/
    	|
    	|__ data/
    	|   |
    	|   |__ 1
    	|   |   |
        |   |   |__images/
        |   |   |__content.txt
        |   |
        |   |__2
        |   |  |
        |   ...
    	|
    	|__ info.csv

- In `src/` directory, run `python imgToHist.py` and `python tfidf.py` to get image histogram and tfidf matrix of the documents ready.
- Now run `python app.py` and at `https://127.0.0.1:5000`, the desired web interface can be observed.