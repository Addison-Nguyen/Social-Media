#ifndef NETWORK_H
#define NETWORK_H

#include "privacy_levels.h"
#include "user.h"
#include "post.h"
#include <string>
#include <vector>


class Network {
public:

    // CONSTRUCTOR
    // pre:
    // post: Makes empty network
    Network();


    // METHODS

    // pre: Id number is less than the number of users
    // post: Takes id and returns the corresponding user
    User* getUser(int id);

    // pre: User already does not exist
    // post: Adds user to database
    void addUser(User* newUser);

    // pre: Both users given in the parameters exist
    // post: Adds a friend connection to two users
    int addConnection(std::string s1, std::string s2);

    // pre: Both users given in the parameters exist
    // post: Deletes a friend connection from two users
    int deleteConnection(std::string s1, std::string s2);

    // pre: Username exists in network
    // post: Returns the id of the given user
    int getId(std::string name);

    // pre: None
    // post: Returns the number of users in the network
    int numUsers();

    // pre: None
    // post: initializes network's information from a file
    void readUsers(char* fname);

    // pre: None
    // post: writes network's information to a file
    void writeUsers(char* fname);


    // GRAPH Methods

    // pre: User from and to exist
    // post: finds the shortest path between two users and produces vector of path
    std::vector<int> shortestPath(int from, int to);

    // pre: user from exists
    // post: Finds a user that is given distance away from given user and gives vector of shortest path
    std::vector<int> distanceUser(int from, int& to, int distance);

    // pre: user who exists
    // post: returns of vector of users who have score number of mutual friends but are not friends with user who
    std::vector<int> suggestFriends(int who, int& score);

    // pre: none
    // post: Returns a vector of all connected components or groups, which are represented by vectors as well
    std::vector<std::vector<int>> groups();


    // POST Methods

    // pre: owner exists
    // post: Adds post
    void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, privacyLevel privacy);

    // pre: owner exists
    // post: Gets howMany posts from ownerId
    std::string getPostsString(int ownerId, int howMany, privacyLevel accessibility);

    // pre: None
    // post: initializes network's posts from a file
    int readPosts(char* fname);

    // pre: None
    // post: compares 2 posts
    static bool postCmp(Post* p1, Post* p2);

    // pre: None
    // post: writes network's posts to a file
    int writePosts(char* fname);

private:
    std::vector<User*> users_;
    // class invariants
    // 1. Vector stores user pointers
    // 2. users_[i] have id i
};

#endif
