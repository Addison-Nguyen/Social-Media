#include "post.h"
#include <string>

Post::Post()
{
    messageId_ = 0;
    ownerId_ = 0;
    message_ = "";
    likes_ = 0;
    privacy_ = PUBLIC;
}

Post::Post(int messageId, int ownerId, std::string message, int likes, privacyLevel P)
{
    messageId_ = messageId;
    ownerId_ = ownerId;
    message_ = message;
    likes_ = likes;
    privacy_ = P;
}

std::string Post::toString()
{
    if (getPrivacyLevel() == PUBLIC)
        return "(Public) " + getMessage() + " Liked by " + std::to_string(getLikes()) + " people.\n";
    else if (getPrivacyLevel() == SEMI_PRIVATE)
        return "(Semi-Private) " + getMessage() + " Liked by " + std::to_string(getLikes()) + " people.\n";
    else
        return "(Private) " + getMessage() + " Liked by " + std::to_string(getLikes()) + " people.\n";
}

int Post::getMessageId()
{
    return messageId_;
}

int Post::getOwnerId()
{
    return ownerId_;
}

std::string Post::getMessage()
{
    return message_;
}

int Post::getLikes()
{
    return likes_;
}

std::string Post::getAuthor()
{
    return "";
}

privacyLevel Post::getPrivacyLevel()
{
    return privacy_;
}


IncomingPost::IncomingPost() : Post()
{
    author_ = "";
}

IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, privacyLevel P, std::string author) : Post(messageId, ownerId, message, likes, P)
{
    author_ = author;
}

std::string IncomingPost::toString()
{
    std::string s = Post::toString();
    return getAuthor() + " wrote: " + s;
}

std::string IncomingPost::getAuthor()
{
    return author_;
}
