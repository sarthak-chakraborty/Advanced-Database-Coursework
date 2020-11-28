from pymongo import MongoClient
import operator
import sys

client = MongoClient('mongodb://localhost:27017/')
db = client.twitter
collection = db['tutorial']
k = 5

hashtags_count = {}
entries_in_db = collection.find({'retweeted_status':{'$exists':1}})

for entry in entries_in_db:
    hashtag_list = entry['entities']['hashtags']
    for hashtag in hashtag_list:
        text = hashtag['text'].lower()

        if text not in hashtags_count:
            hashtags_count[text] = 1
        else:
            hashtags_count[text] += 1
            
sorted_hashtags_count = sorted(hashtags_count.items(), key=operator.itemgetter(1), reverse=True)
topk_hashtags = sorted_hashtags_count[:k]

print("Top-5 Hashtags for retweets are:")
for entry in topk_hashtags:
    print(entry[0])
