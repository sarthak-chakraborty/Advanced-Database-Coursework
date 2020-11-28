import sys
from pymongo import MongoClient


def readFile(filename):
    friend, follower = {}, {}
    f = open(filename)

    for line in f:
        t = line.split(',')
        u1 = t[0].strip()
        u2 = t[1].strip()

        if u1 not in friend:
            friend[u1] = 1
        else:
            friend[u1] += 1

        if u2 not in follower:
            follower[u2] = 1
        else:
            follower[u2] += 1

    f.close()
    return friend, follower
    


def main():
    if len(sys.argv) < 2:
        print("Filename not given")
        exit()

    filename = sys.argv[1]
    client = MongoClient('mongodb://localhost:27017/')
    db = client.twitter
    collection = db['tutorial']

    print("Reading file to get friend and follower counts...")
    friend_count, follower_count = readFile(filename)

    print("Updating the database with friend and follower counts...")
    for user, count in friend_count.items():
        collection.update_many({'user.id_str' : user}, {'$set':{'user.new_friends_count':count}})

    for user, count in follower_count.items():
        collection.update_many({'user.id_str':user}, {'$set':{'user.new_followers_count':count}})


if __name__ == "__main__":
    main()