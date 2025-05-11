#include "network.h"
#include "privacy_levels.h"
#include "user.h"
#include "post.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <algorithm>

Network::Network()
{
    users_ = std::vector<User*>();
}

User* Network::getUser(int id)
{
    // Sees if id number is less than size
    // Will mean that id number is there since users_[i] have id i
    if (id < users_.size() && id >= 0)
        return users_[id];
    else
        return nullptr;
}

void Network::addUser(User* newUser)
{
    // Checks if newUser already exists before adding
    for (auto user: users_)
    {
        if (user == newUser)
            return;
    }
    users_.push_back(newUser);
    return;
}

int Network::addConnection(std::string s1, std::string s2)
{
    User* friend1;
    User* friend2;

    // will check if the 2 strings matches 2 respective
    int sum = 0;

    // checks for all users if name matches either name and assigns matched user to friend 1 or friend 2
    for (auto user: users_)
    {
        if (user->getName() == s1 || user->getName() == s2)
        {
            if (sum == 0)
            {
                ++sum;
                friend1 = user;
            }
            else if (sum == 1)
            {
                ++sum;
                friend2 = user;
            }
            // if there are more than 2 matches for some reason, returns -1
            else
                return -1;
        }
    }

    // if users given by the strings exist, adds friend for both
    if (sum == 2)
    {
        friend1->addFriend(friend2->getId());
        friend2->addFriend(friend1->getId());
        return 0;
    }
    // if not, returns -1
    return -1;
}

int Network::deleteConnection(std::string s1, std::string s2)
{
    // makes 2 users
    User* friend1;
    User* friend2;

    // will check if the 2 strings matches 2 respective
    int sum = 0;

    // checks for all users if name matches either name and assigns matched user to friend 1 or friend 2
    for (auto user: users_)
    {
        if (user->getName() == s1 || user->getName() == s2)
        {
            if (sum == 0)
            {
                ++sum;
                friend1 = user;
            }
            else if (sum == 1)
            {
                ++sum;
                friend2 = user;
            }
            // if there are more than 2 matches for some reason, returns -1
            else
                return -1;
        }
    }

    // if users given by the strings exist, deletes friend for both
    if (sum == 2)
    {
        friend1->deleteFriend(friend2->getId());
        friend2->deleteFriend(friend1->getId());
        return 0;
    }
    // if not, returns -1
    return -1;
}

int Network::getId(std::string name)
{
    // checks for all users if name matches given name
    for (auto id: users_)
    {
        if (id->getName() == name)
            return id->getId();
    }
    // if no matches, reutrn -1
    return -1;

}

int Network::numUsers()
{
    return users_.size();
}

void Network::readUsers(char* fname)
{
    std::string myString;
    int myInt;
    int counter(0);

    // reads given file
    std::ifstream networkFile(fname);

    // gets the first line, which shows the total users
    getline(networkFile, myString);
    std::stringstream userCount;
    userCount << myString;
    userCount >> myInt;

    // Loops from 0 to total users - 1
    while (counter < myInt)
    {
        std::string Name, temp;
        int Zip, Year, idNum;
        std::set<int> Friends;
        privacyLevel privacy;

        // skips id line
        getline(networkFile, myString);

        // gets name
        getline(networkFile, myString);
        std::stringstream getname(myString);
        while (getname >> temp)
        {
            Name = Name + temp + " ";
        }
        // gets the last space out
        if (Name.length() != 0)
            Name.resize(Name.length() - 1);

        // gets birth year
        getline(networkFile, myString);
        std::stringstream(myString) >> Year;

        // gets zip code
        getline(networkFile, myString);
        std::stringstream(myString) >> Zip;

        // gets friends
        getline(networkFile, myString);
        std::stringstream getid(myString);
        while (getid >> idNum)
        {
            Friends.insert(idNum);
        }

        // gets privacyLevel
        getline(networkFile, myString);
        std::stringstream(myString) >> temp;
        if (temp == "public")
            privacy = PUBLIC;
        else if (temp == "semi-private")
            privacy = SEMI_PRIVATE;
        else
            privacy = PRIVATE;

        // Creates user and puts it in network
        User* newAddition = new User(counter, Name, Year, Zip, Friends, privacy);
        addUser(newAddition);

        // Adds 1 to counter to go to next user
        counter += 1;
    }
    networkFile.close();
}

void Network::writeUsers(char* fname)
{
    // Ready to write in file
    std::ofstream networkFile(fname);
    // Writes how many users there are
    networkFile << numUsers() << std::endl;
    // Loops amount of users times
    for (int i = 0; i < numUsers(); ++i)
    {
        // chosen is users_[i]
        User* chosen = users_[i];
        // grabs the friends set
        std::set<int> friends = chosen->getFriends();
        // Writes id number, name, year, and zip code respectively
        networkFile << i << std::endl;
        networkFile << "\t" << chosen->getName() << std::endl;
        networkFile << "\t" << chosen->getYear() << std::endl;
        networkFile << "\t" << chosen->getZip() << std::endl;
        networkFile << "\t";
        // Writes the whole friends list
        for (auto i = friends.begin(); i != friends.end(); ++i)
            networkFile << *i << " ";

        // Writes privacy level
        if (chosen->getPrivacyLevel() == PUBLIC)
            networkFile << "\t" << "public" << std::endl;
        else if (chosen->getPrivacyLevel() == SEMI_PRIVATE)
            networkFile << "\t" << "semi-private" << std::endl;
        else
            networkFile << "\t" << "private" << std::endl;

        networkFile << "\n";
    }
    networkFile.close();
}

std::vector<int> Network::shortestPath(int from, int to)
{
    // if user from and/or user to do not exist, return empty vector
    std::vector<int> empty;
    if (from >= numUsers() || to >= numUsers())
        return empty;

    std::queue<int> q;
    // visited checks whether user was visited and prev gives the previous user
    std::vector<bool> visited(numUsers(), 0);
    std::vector<int> prev(numUsers(), -1);

    // from is now visited and pushed to queue
    visited[from] = true;
    q.push(from);

    // runs until queue is empty
    while (q.size() > 0)
    {
        // gets int from front, pops int from queue, gets the user from id number
        int cur = q.front();
        q.pop();
        User* current = getUser(cur);

        // gets all of user's friends
        for (auto friendConnection: current->getFriends())
        {
            // if friend has not been visited yet, gets friend's prev user, marks friend as visited, and pushes friend to queue
            if (!visited[friendConnection])
            {
                prev[friendConnection] = cur;
                visited[friendConnection] = true;
                q.push(friendConnection);
            }
        }
    }

    std::vector<int> output;
    int cur = to;
    while (cur != -1)
    {
        output.push_back(cur);
        cur = prev[cur];
    }
    reverse(output.begin(), output.end());
    return output;
}

std::vector<int> Network::distanceUser(int from, int& to, int distance)
{
    std::queue<int> q;
    // visited checks whether user was visited and prev gives the previous user
    std::vector<bool> visited(numUsers(), 0);
    std::vector<int> prev(numUsers(), -1);
    std::vector<int> dist(numUsers(), -1);

    // from is now visited and pushed to queue
    visited[from] = true;
    q.push(from);
    dist[from] = 0;

    // runs until queue is empty
    while (q.size() > 0)
    {
        // gets int from front, pops int from queue, gets the user from id number
        int cur = q.front();
        q.pop();
        User* current = getUser(cur);

        // gets all of user's friends
        for (auto friendConnection: current->getFriends())
        {
            // if friend has not been visited yet, gets friend's prev user, marks friend as visited, and pushes friend to queue
            if (!visited[friendConnection])
            {
                prev[friendConnection] = cur;
                dist[friendConnection] = dist[cur] + 1;
                visited[friendConnection] = true;
                q.push(friendConnection);
            }
        }
    }
    for (int i = 0; i < numUsers(); ++i)
    {
        if (i != from && dist[i] == distance)
        {
            std::vector<int> output;
            to = i;
            int cur = i;
            while (cur != -1)
            {
                output.push_back(cur);
                cur = prev[cur];
            }
            reverse(output.begin(), output.end());
            return output;
        }
    }
    std::vector<int> empty;
    to = -1;
    return empty;
}

std::vector<int> Network::suggestFriends(int who, int& score)
{
    // gets user from who, makes answer vector, makes set that contains all friends of user who
    User* suggest = getUser(who);
    std::vector<int> passed;
    std::set<int> allFriends = suggest->getFriends();

    // goes through all users except user who and its friends
    for (int user = 0; user < numUsers(); ++user)
    {
        if (user != who && allFriends.count(user) == 0)
        {
            // count will be the amount of mutual friends user has withuser who
            int count = 0;
            User* potential = getUser(user);
            for (auto listFriends: potential->getFriends())
                if (allFriends.count(listFriends) >= 0)
                    count += 1;

            // if count equals score, user is added to vector
            if (count == score)
                passed.push_back(user);
            // if count is more than score, score is changed to count, vector is cleared, user is added to vector
            else if(count > score)
            {
                score = count;
                passed.clear();
                passed.push_back(user);
            }
        }
    }

    if (passed.size() == 0)
        score = 0;
    return passed;
}

std::vector<std::vector<int>> Network::groups()
{
    // allGroups is what will be returned, visited checks whether user has been visited
    std::vector<std::vector<int>> allGroups;
    std::vector<bool> visited(numUsers(), 0);

    // goes through all users
    for (int user = 0; user < numUsers(); ++user)
    {
        // if user has not been visited yet, dfs will happen
        if (!visited[user])
        {
            std::stack<int> stck;
            // group will be one connected component
            std::vector<int> group;

            // user is now visited, and pushed to stack and the group
            visited[user] = true;
            stck.push(user);
            group.push_back(user);

            // runs until stack is empty
            while (stck.size() > 0)
            {
                // gets int from top, pops int from stack, gets the user from id number
                int cur = stck.top();
                stck.pop();
                User* current = getUser(cur);

                // gets all of user's friends
                for (auto friendConnection: current->getFriends())
                {
                    // if friend has not been visited yet, marks friend as visited and pushes friend to stack and group
                    if (!visited[friendConnection])
                    {
                        visited[friendConnection] = true;
                        stck.push(friendConnection);
                        group.push_back(friendConnection);
                    }
                }
            }
            allGroups.push_back(group);
        }
    }
    return allGroups;
}

void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, privacyLevel privacy)
{
    // Gets user from ownerId
    User* owner = getUser(ownerId);

    // Gets amount of total posts in network to get thae messageID
    int messageNum = 0;
    for (int i = 0; i < numUsers(); ++i)
    {
        User* temp = getUser(i);
        messageNum = messageNum + temp->getPosts().size();
    }

    // Makes incoming post or regular post
    if (isIncoming == true)
    {
        IncomingPost* add = new IncomingPost(messageNum, ownerId, message, likes, privacy, authorName);
        owner->addPost(add);
    }
    else
    {
        Post* add = new Post(messageNum, ownerId, message, likes, privacy);
        owner->addPost(add);
    }
}

std::string Network::getPostsString(int ownerId, int howMany, privacyLevel accessibility)
{
    // Gets user and returns getPostsString from User
    User* owner = getUser(ownerId);
    return owner->getPostsString(howMany, accessibility);
}

int Network::readPosts(char* fname)
{
    std::string myString;
    int myInt;
    int counter(0);

    // reads given file
    std::ifstream networkFile(fname);
    if (!networkFile.is_open())
        return -1;
    // gets the first line, which shows the total posts
    getline(networkFile, myString);
    std::stringstream userCount;
    userCount << myString;
    userCount >> myInt;

    // Loops from 0 to total posts - 1
    while (counter < myInt)
    {
        std::string message, author, temp;
        int likes, ownerID;
        bool isincoming;
        privacyLevel privacy;
        std::set<int> Friends;

        // skips id line
        getline(networkFile, myString);

        // gets message
        getline(networkFile, myString);
        std::stringstream getMessage(myString);
        while (getMessage >> temp)
        {
            message = message + temp + " ";
        }
        // gets the last space out
        if (message.length() != 0)
            message.resize(message.length() - 1);

        // gets owner id
        getline(networkFile, myString);
        std::stringstream(myString) >> ownerID;

        // gets likes
        getline(networkFile, myString);
        std::stringstream(myString) >> likes;

        // gets privacy
        getline(networkFile, myString);
        std::stringstream(myString) >> temp;
        if (temp == "public")
            privacy = PUBLIC;
        else if (temp == "semi-private")
            privacy = SEMI_PRIVATE;
        else
            privacy = PRIVATE;

        // gets author or blank line
        getline(networkFile, myString);
        // checks if line is blank and then adds a regular post to the network
        if (myString == "")
        {
            isincoming = false;
            author = "";
            // Makes post and adds it to network
            addPost(ownerID, message, likes, isincoming, author, privacy);
        } //(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic)

        // if the line is not blank, gets author and ispublic, adds incoming post
        else
        {
            isincoming = true;
            std::stringstream getMessage(myString);
            while (getMessage >> temp)
            {
                author = author + temp + " ";
            }
            // gets the last space out
            if (author.length() != 0)
                author.resize(author.length() - 1);

            // Makes incoming post and adds it to network
            addPost(ownerID, message, likes, isincoming, author, privacy);
        }
        // Adds 1 to counter to go to next user
        counter += 1;
    }

    networkFile.close();
    return 0;
}

bool Network::postCmp(Post* p1, Post* p2)
{
    return p1->getMessageId() < p2->getMessageId();
}

int Network::writePosts(char* fname)
{
    // Ready to write in file
    std::ofstream networkFile(fname);
    if (!networkFile.is_open())
        return -1;

    // Gets all posts in a vector and sorts it
    std::vector <Post*> v;
    for (int i = 0; i < numUsers(); ++i)
    {
        User* temp = getUser(i);
        std::vector<Post*> get = temp->getPosts();
        for (auto i : get)
            v.push_back(i);
    }
    std::sort(v.begin(), v.end(), postCmp);

    networkFile << v.size() << std::endl;
    // Loops amount of posts times
    for (int i = 0; i < v.size(); ++i)
    {
        // chosen is
        Post* chosen = v[i];
        // grabs the post set
        // Writes message, owner id, and likes respectively
        networkFile << i << std::endl;
        networkFile << "\t" << chosen->getMessage() << std::endl;
        networkFile << "\t" << chosen->getOwnerId() << std::endl;
        networkFile << "\t" << chosen->getLikes() << std::endl;

        // Writes privacy level
        if (chosen->getPrivacyLevel() == PUBLIC)
            networkFile << "\t" << "public" << std::endl;
        else if (chosen->getPrivacyLevel() == SEMI_PRIVATE)
            networkFile << "\t" << "semi-private" << std::endl;
        else
            networkFile << "\t" << "private" << std::endl;

        // if there is no author, writes 2 blank lines
        if (chosen->getAuthor() == "")
        {
            networkFile << "\n";
        }
        // writes author if there is one
        else
        {
            networkFile << "\t" << chosen->getAuthor() << std::endl;
        }
    }
    networkFile.close();
    return 0;
}
