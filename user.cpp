#include "user.h"
#include <string>
#include <set>
#include <vector>

User::User()
{
    id_ = -1;
    name_ = "";
    year_ = -1;
    zip_ = -1;
    friends_ = std::set<int>();
    privacy_ = PUBLIC;
}

User::User(int id, std::string name, int year, int zip, std::set<int> friends, privacyLevel P)
{
    id_ = id;
    name_ = name;
    year_ = year;
    zip_ = zip;
    friends_ = friends;
    privacy_ = P;
}

int User::getId()
{
    return id_;
}

std::string User::getName()
{
    return name_;
}

int User::getYear()
{
    return year_;
}

int User::getZip()
{
    return zip_;
}

std::set<int> & User::getFriends()
{
    return friends_;
}

privacyLevel User::getPrivacyLevel()
{
    return privacy_;
}

void User::addFriend(int id)
{
    // checks if friend already exists
    for (auto friends: friends_)
    {
        if (friends == id)
            return;
    }
    // adds friend if friend does not exist
    friends_.insert(id);
    return;
}

void User::deleteFriend(int id)
{
    // checks if friend exists and deletes friend
    for (auto friends: friends_)
    {
        if (friends == id)
        {
            friends_.erase(id);
            return;
        }
    }
    // returns nothing if friend does not exist
    return;
}

void User::addPost(Post* newPost)
{
    messages_.push_back(newPost);
}

std::vector<Post*> User::getPosts()
{
    return messages_;
}

std::string User::getPostsString(int howMany, privacyLevel accessibility)
{
    std::string answer;
    int count = 0;

    // shows all posts
    if (accessibility == PUBLIC)
    {
    	// Loops until count gets to howMany or goes through all posts, does this from most recent to oldest
        for (int i = getPosts().size() - 1; i >= 0; --i)
        {
            answer = answer + messages_[i] -> toString() + "\n";
            count += 1;
            if (count == howMany)
                return answer;
        }
    }

    // can only show public or semi-private posts
    else if (accessibility == SEMI_PRIVATE)
    {
        // Loops until count gets to howMany or goes through all posts, does this from most recent to oldest
        for (int i = getPosts().size() - 1; i >= 0; --i)
        {
            // checks if post is public or semi-private
            if (messages_[i] -> getPrivacyLevel() == PUBLIC || messages_[i] -> getPrivacyLevel() == SEMI_PRIVATE)
            {
                answer = answer + messages_[i] -> toString() + "\n";
                count += 1;
            }
            if (count == howMany)
                return answer;
        }
    }

    // can only show public posts
    else
    {
        // Loops until count gets to howMany or goes through all posts, does this from most recent to oldest
        for (int i = getPosts().size() - 1; i >= 0; --i)
        {
            // checks if post is public
            if (messages_[i] -> getPrivacyLevel() == PUBLIC)
            {
                answer = answer + messages_[i] -> toString() + "\n";
                count += 1;
            }
            if (count == howMany)
                return answer;
        }
    }
    // At end if count doesn't reach howMany
    return answer;
}
