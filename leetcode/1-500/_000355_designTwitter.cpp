typedef int UserID;
typedef int TweetID ;
struct Tweet {
    TweetID tweet_id;
    int timestamp;
};
typedef deque<Tweet> Tweets;
typedef set<UserID> FollowSet;

struct TweetComp {
    bool operator()(const Tweet& lhs, const Tweet& rhs) const {
        return rhs.timestamp > lhs.timestamp;
    }
};

class Twitter {
    const int KEEP_MAX=10;  // will keep max 10 latest tweets from a user

    map<UserID, Tweets> tweets;
    unordered_map<TweetID, UserID> tweet_to_user;
    map<UserID, FollowSet> follows;

    int clock=0;  // will tick up per global tweet

public:
    Twitter() {
        
    }
    
    void postTweet(int userId, int tweetId) {
        Tweet tweet = {tweetId, clock};
        clock++;
        tweets[userId].push_front(tweet);
        if (tweets[userId].size() > KEEP_MAX) {
            tweets[userId].pop_back();
        }
        tweet_to_user[tweetId] = userId;
    }
    
    vector<int> getNewsFeed(int userId) {
        std::priority_queue<Tweet, vector<Tweet>, TweetComp> pq;
        vector<int> results;
        // Initialize the pq;
        // Put the users own tweets in the pq.
        for (const auto& tweet : tweets[userId]) {            
            pq.push(tweet);
        }
        // Now put the users followers tweets in the pq.
        for (const auto& fid : follows[userId]) {            
            for (const auto& tweet: tweets[fid]) {                
                pq.push(tweet);
            }
        }
        for (int i=0; i<10 && !pq.empty(); i++) {
            Tweet t = pq.top(); 
            pq.pop();
            results.push_back(t.tweet_id);
        }
        return results;
    }
    
    void follow(int followerId, int followeeId) {
        follows[followerId].insert(followeeId);
    }
    
    void unfollow(int followerId, int followeeId) {
        follows[followerId].erase(followeeId);
    }
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */
