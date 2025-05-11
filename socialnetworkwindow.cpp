#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include "network.h"
#include "user.h"
#include "privacy_levels.h"
#include <string>
#include <set>
#include <vector>
#include <QDebug>

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    // reads users and posts, thedisplays
    ui->setupUi(this);
    getuser = false;
    char users[] = "users1.txt";
    char posts[] = "posts1.txt";
    network.readUsers(users);
    network.readPosts(posts);
    display();

    // connects loginButton and myLoginClick
    connect(ui->loginButton,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::myLoginClick);
    // connects loginButton and backclick
    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::backClick);
    // connects suggestionsTable and suggestionClick
    connect(ui->suggestionsTable,
            &QTableWidget::cellClicked,
            this,
            &SocialNetworkWindow::suggestionClick);
    // connects friendsTable and profileClick
    connect(ui->friendsTable,
            &QTableWidget::cellClicked,
            this,
            &SocialNetworkWindow::profileClick);
    // connects friendButton and addFriendClick
    connect(ui->friendButton,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::addFriendClick);
}

void SocialNetworkWindow::display()
{
    // login
    if (getuser == false)
    {
        ui->backButton->hide();
        ui->friendButton->hide();
        ui->friendsTable->hide();
        ui->loginButton->show();
        ui->loginLabel->show();
        ui->loginText->show();
        ui->postsLabel->hide();
        ui->profileLabel->hide();
        ui->suggestionsTable->hide();
        ui->privacyLabel->hide();
        return;
    }

    // user's profile
    if (getuser== true && account == profile)
    {
        ui->backButton->hide();
        ui->friendButton->hide();
        ui->friendsTable->show();
        ui->loginButton->hide();
        ui->loginLabel->hide();
        ui->loginText->hide();
        ui->postsLabel->show();
        ui->profileLabel->show();
        ui->suggestionsTable->show();
        ui->privacyLabel->show();
    }

    // some other person's profile
    else if (getuser== true && account != profile)
    {
        // If person's profile is public
        if (profile->getPrivacyLevel() == PUBLIC)
        {
            ui->backButton->show();
            ui->friendButton->show();
            ui->friendsTable->show();
            ui->loginButton->hide();
            ui->loginLabel->hide();
            ui->loginText->hide();
            ui->postsLabel->show();
            ui->profileLabel->show();
            ui->suggestionsTable->hide();
            ui->privacyLabel->show();
        }

        // If person's profile is semi-private
        else if (profile->getPrivacyLevel() == SEMI_PRIVATE)
        {
            // gets distance between account and clicked profile
            int from = account->getId();
            int to = profile->getId();
            std::vector<int> connection = network.shortestPath(from, to);

            // shows list of friends if friend or friend of friend
            if (connection.size() == 2 || connection.size() == 3)
            {
                ui->backButton->show();
                ui->friendButton->show();
                ui->friendsTable->show();
                ui->loginButton->hide();
                ui->loginLabel->hide();
                ui->loginText->hide();
                ui->postsLabel->show();
                ui->profileLabel->show();
                ui->suggestionsTable->hide();
                ui->privacyLabel->show();
            }

            // else does not show list
            else
            {
                ui->backButton->show();
                ui->friendButton->show();
                ui->friendsTable->hide();
                ui->loginButton->hide();
                ui->loginLabel->hide();
                ui->loginText->hide();
                ui->postsLabel->show();
                ui->profileLabel->show();
                ui->suggestionsTable->hide();
                ui->privacyLabel->show();
            }
        }

        // if account is private
        else
        {
            // gets distance between account and clicked profile
            int from = account->getId();
            int to = profile->getId();
            std::vector<int> connection = network.shortestPath(from, to);

            // shows list of friends if friend
            if (connection.size() == 2)
            {
                ui->backButton->show();
                ui->friendButton->show();
                ui->friendsTable->show();
                ui->loginButton->hide();
                ui->loginLabel->hide();
                ui->loginText->hide();
                ui->postsLabel->show();
                ui->profileLabel->show();
                ui->suggestionsTable->hide();
                ui->privacyLabel->show();
            }

            // else does not show list
            else
            {
                ui->backButton->show();
                ui->friendButton->show();
                ui->friendsTable->hide();
                ui->loginButton->hide();
                ui->loginLabel->hide();
                ui->loginText->hide();
                ui->postsLabel->show();
                ui->profileLabel->show();
                ui->suggestionsTable->hide();
                ui->privacyLabel->show();
            }
        }
    }
}

void SocialNetworkWindow::myLoginClick()
{
    // gets what is in login text and attempts to get account
    std::string s = ui->loginText->toPlainText().toStdString();
    id = network.getId(s);
    account = network.getUser(id);

    // if not able to, error message of "Enter another name"
    if (account == nullptr)
    {
        ui->loginLabel->setText("Enter another name");
    }

    // if login successful
    else
    {
        // gets whose profile it is, gets user
        getuser = true;
        profile = account;

        // makes suggestion table
        int suggest = 1;
        std::vector<int> suggestions = network.suggestFriends(id, suggest);
        ui->suggestionsTable->setColumnCount(1);
        ui->suggestionsTable->setRowCount(suggestions.size());
        for (int i = 0; i < suggestions.size(); ++i)
        {
            User* suggest = network.getUser(suggestions[i]);
            std::string s = suggest->getName();
            ui->suggestionsTable->setItem(0, i, new QTableWidgetItem(QString::fromStdString(s)));
        }

        // gets user's posts
        QString posts = QString::fromStdString(network.getPostsString(id, 5, PUBLIC));
        ui->postsLabel->setText(posts);

        // gets user's friends
        std::set<int> & allFriends = account->getFriends();
        ui->friendsTable->setColumnCount(1);
        ui->friendsTable->setRowCount(allFriends.size());
        int givenrow = 0;

        for (auto i: allFriends)
        {
            User* selectedFriend = network.getUser(i);
            std::string s = selectedFriend->getName();
            ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
            givenrow += 1;
        }

        // Shows privacy level of user's profile
        if (account->getPrivacyLevel() == PUBLIC)
            ui->privacyLabel->setText("Public Profile");
        else if (account->getPrivacyLevel() == SEMI_PRIVATE)
            ui->privacyLabel->setText("Semi-Private Profile");
        else
            ui->privacyLabel->setText("Private Profile");

        // shows it's user's profile
        ui->profileLabel->setText("My Profile");
        display();

    }
}

void SocialNetworkWindow::profileClick(int row, int col)
{
    // gets user that you clicked on
    QTableWidgetItem *cell = ui->friendsTable->item(row, col);
    QString getProfile = cell->text();
    int profileId = network.getId(getProfile.toStdString());
    profile = network.getUser(profileId);

    // if the user you clicked on is user logged in, will show the same page as if you logged in
    if (profile == account)
    {
        int suggest = 1;
        std::vector<int> suggestions = network.suggestFriends(id, suggest);
        ui->suggestionsTable->setColumnCount(1);
        ui->suggestionsTable->setRowCount(suggestions.size());
        for (int i = 0; i < suggestions.size(); ++i)
        {
            User* suggest = network.getUser(suggestions[i]);
            std::string s = suggest->getName();
            ui->suggestionsTable->setItem(0, i, new QTableWidgetItem(QString::fromStdString(s)));
        }

        QString posts = QString::fromStdString(network.getPostsString(id, 5, PUBLIC));
        ui->postsLabel->setText(posts);

        std::set<int> allFriends = account->getFriends();
        ui->friendsTable->setColumnCount(1);
        ui->friendsTable->setRowCount(allFriends.size());
        int givenrow = 0;
        for (auto i: allFriends)
        {
            User* selectedFriend = network.getUser(i);
            std::string s = selectedFriend->getName();
            ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
            givenrow += 1;
        }

        // Shows privacy level of user's profile
        if (account->getPrivacyLevel() == PUBLIC)
            ui->privacyLabel->setText("Public Profile");
        else if (account->getPrivacyLevel() == SEMI_PRIVATE)
            ui->privacyLabel->setText("Semi-Private Profile");
        else
            ui->privacyLabel->setText("Private Profile");

        ui->profileLabel->setText("My Profile");
        display();
    }

    // if profile is someone else
    else
    {
        // gets their posts
        // gets distance between users
        int from = account->getId();
        int to = profile->getId();
        std::vector<int> connection = network.shortestPath(from, to);
        // if friends, shows all posts
        if (connection.size() == 2)
        {
            QString posts = QString::fromStdString(network.getPostsString(profileId, 5, PUBLIC));
            ui->postsLabel->setText(posts);
        }
        // if friend of friend, shows public and semi-private posts
        else if (connection.size() == 3)
        {
            QString posts = QString::fromStdString(network.getPostsString(profileId, 5, SEMI_PRIVATE));
            ui->postsLabel->setText(posts);
        }
        // if strangers, show only public posts
        else if (connection.size() >= 4 || connection.size() < 2)
        {
            QString posts = QString::fromStdString(network.getPostsString(profileId, 5, PRIVATE));
            ui->postsLabel->setText(posts);
        }

        // gets their friends
        std::set<int> allFriends = profile->getFriends();
        ui->friendsTable->setColumnCount(1);
        ui->friendsTable->setRowCount(allFriends.size());
        int givenrow = 0;
        for (auto i: allFriends)
        {
            User* selectedFriend = network.getUser(i);
            std::string s = selectedFriend->getName();
            ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
            givenrow += 1;
        }

        // Shows privacy level of user's profile
        if (profile->getPrivacyLevel() == PUBLIC)
            ui->privacyLabel->setText("Public Profile");
        else if (profile->getPrivacyLevel() == SEMI_PRIVATE)
            ui->privacyLabel->setText("Semi-Private Profile");
        else
            ui->privacyLabel->setText("Private Profile");

        // gets their profile
        QString name = QString::fromStdString(profile->getName());
        ui->profileLabel->setText(name + "'s Profile");
        display();
    }
}

void SocialNetworkWindow::suggestionClick(int row, int col)
{
    // gets user that you clicked on
    QTableWidgetItem *cell = ui->suggestionsTable->item(row, col);
    QString getProfile = cell->text();
    int profileId = network.getId(getProfile.toStdString());
    profile = network.getUser(profileId);

    // gets their posts, same process as one in profileClick
    int from = account->getId();
    int to = profile->getId();
    std::vector<int> connection = network.shortestPath(from, to);
    if (connection.size() == 2)
    {
        QString posts = QString::fromStdString(network.getPostsString(profileId, 5, PUBLIC));
        ui->postsLabel->setText(posts);
    }
    else if (connection.size() == 3)
    {
        QString posts = QString::fromStdString(network.getPostsString(profileId, 5, SEMI_PRIVATE));
        ui->postsLabel->setText(posts);
    }
    else if (connection.size() >= 4 || connection.size() < 2)
    {
        QString posts = QString::fromStdString(network.getPostsString(profileId, 5, PRIVATE));
        ui->postsLabel->setText(posts);
    }

    // gets their friends
    std::set<int> allFriends = profile->getFriends();
    ui->friendsTable->setColumnCount(1);
    ui->friendsTable->setRowCount(allFriends.size());
    int givenrow = 0;
    for (auto i: allFriends)
    {
        User* selectedFriend = network.getUser(i);
        std::string s = selectedFriend->getName();
        ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
        givenrow += 1;
    }

    // Shows privacy level of user's profile
    if (profile->getPrivacyLevel() == PUBLIC)
        ui->privacyLabel->setText("Public Profile");
    else if (profile->getPrivacyLevel() == SEMI_PRIVATE)
        ui->privacyLabel->setText("Semi-Private Profile");
    else
        ui->privacyLabel->setText("Private Profile");

    // gets their profile
    QString name = QString::fromStdString(profile->getName());
    ui->profileLabel->setText(name + "'s Profile");
    display();
}

void SocialNetworkWindow::backClick()
{
    profile = account;

    // gets suggested friends
    int suggest = 1;
    std::vector<int> suggestions = network.suggestFriends(id, suggest);
    ui->suggestionsTable->setColumnCount(1);
    ui->suggestionsTable->setRowCount(suggestions.size());
    for (int i = 0; i < suggestions.size(); ++i)
    {
        User* suggest = network.getUser(suggestions[i]);
        std::string s = suggest->getName();
        ui->suggestionsTable->setItem(0, i, new QTableWidgetItem(QString::fromStdString(s)));
    }

    // gets user's posts
    QString posts = QString::fromStdString(network.getPostsString(id, 5, PUBLIC));
    ui->postsLabel->setText(posts);

    // gets user's friends
    std::set<int> allFriends = account->getFriends();
    ui->friendsTable->setColumnCount(1);
    ui->friendsTable->setRowCount(allFriends.size());
    int givenrow = 0;
    for (auto i: allFriends)
    {
        User* selectedFriend = network.getUser(i);
        std::string s = selectedFriend->getName();
        ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
        givenrow += 1;
    }

    // Shows privacy level of user's profile
    if (account->getPrivacyLevel() == PUBLIC)
        ui->privacyLabel->setText("Public Profile");
    else if (account->getPrivacyLevel() == SEMI_PRIVATE)
        ui->privacyLabel->setText("Semi-Private Profile");
    else
        ui->privacyLabel->setText("Private Profile");

    // shows it's user's profile
    ui->profileLabel->setText("My Profile");
    display();
}

void SocialNetworkWindow::addFriendClick()
{
    // updates network and adds friend
    network.addConnection(profile->getName(), account->getName());
    char users[] = "users1.txt";
    network.writeUsers(users);

    // updates friend list
    std::set<int> allFriends = profile->getFriends();
    ui->friendsTable->setColumnCount(1);
    ui->friendsTable->setRowCount(allFriends.size());
    int givenrow = 0;
    for (auto i: allFriends)
    {
        User* selectedFriend = network.getUser(i);
        std::string s = selectedFriend->getName();
        ui->friendsTable->setItem(0, givenrow, new QTableWidgetItem(QString::fromStdString(s)));
        givenrow += 1;
    }

    // updates post list
    int id = profile->getId();
    QString posts = QString::fromStdString(network.getPostsString(id, 5, PUBLIC));
    ui->postsLabel->setText(posts);
    display();

}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

