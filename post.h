#ifndef POST_H
#define POST_H

#include <iostream>
#include <string>
#include "privacy_levels.h"

class Post
{
public:

    // CONSTRUCTORS

    // pre: None
    // post: Creates empty post
    Post();

    // pre: None
    // post: Makes post with specific values
    Post(int messageId, int ownerId, std::string message, int likes, privacyLevel P);


    // GETTERS

    // pre: none
    // Post: Returns a string that contains the message, likes, and privacy level
    virtual std::string toString();

    // pre: none
    // Post: Gets id of message
    int getMessageId();

    // pre: none
    // Post: Gets id of owner
    int getOwnerId();

    // pre: none
    // Post: Gets message
    std::string getMessage();

    // pre: none
    // Post: Gets the likes
    int getLikes();

    // pre: none
    // Post: Gets the author, returns empty string on default
    virtual std::string getAuthor();

    // pre: none
    // Post: Gets if the post is public, semi-private, or private
    privacyLevel getPrivacyLevel();

private:
    int messageId_;
    int ownerId_ ;
    std::string message_;
    int likes_;
    privacyLevel privacy_;
    // class invariants
    // 1. Every post has a messageID, ownerID, message, the number of likes, and privacy level
    // 2. On default, every post's author is the owner
};

class IncomingPost: public Post
{
public:

    // CONSTRUCTORS

    // pre: None
    // post: Creates empty incoming post
    IncomingPost();

    // pre: None
    // post: Makes incoming post with specific values
    IncomingPost(int messageId, int ownerId, std::string message, int likes, privacyLevel P, std::string author);


    // GETTERS

    // pre: none
    // Post: Returns a string that contains the message and the likes
    std::string toString();

    // pre: none
    // Post: Gets the author
    std::string getAuthor();

private:
    std::string author_;
    // class invariants
    // 1. IncomingPost is child of class Post
    // 2. IncomingPost has everything from Post plus author
};

#endif
