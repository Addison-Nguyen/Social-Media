#ifndef USER_H
#define USER_H

#include "post.h"
#include "privacy_levels.h"
#include <string>
#include <set>
#include <vector>

class User {
public:

    // CONSTRUCTORS

    // pre:
    // post: Makes arbitrary user
    User();

    // pre:
    // post: Makes user with specific values
    User(int id, std::string name, int year, int zip, std::set<int> friends, privacyLevel P);


    // GETTERS

    // pre: None
    // post: Returns user's ID
    int getId();

    // pre: None
    // post: Returns user's name
    std::string getName();

    // pre: None
    // post: Returns user's birth year
    int getYear();

    // pre: None
    // post: Returns user's zipcode
    int getZip();

    // pre: None
    // post: Returns set of user's friends
    std::set<int> & getFriends();

    // pre: None
    // post: Returns privacy level
    privacyLevel getPrivacyLevel();


    // FRIEND Methods

    // pre: Friend does not exist yet
    // post: Adds id as a new friend
    void addFriend(int id);

    // pre: Id exists in friend list
    // post: Deletes id as friend
    void deleteFriend(int id);


    // POST Methods

    // pre: None
    // post: Adds new post
    void addPost(Post* newPost);

    // pre: None
    // post: Gets all posts
    std::vector<Post*> getPosts();

    // pre: None
    // post: Gets all posts
    std::string getPostsString(int howMany, privacyLevel accessibility);

private:
    int id_;
    std::string name_;
    int year_;
    int zip_;
    std::set<int> friends_;
    std::vector<Post*> messages_;
    privacyLevel privacy_;
    // class invariants
    // 1. Every user has an id, name, year, zip, friend list, and privacy level associated with them
    // 2. The user can delete or add friends when applicable
    // 3. Each user has a vector of posts
};

#endif
