#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H
#include "network.h"
#include "user.h"
#include "privacy_levels.h"
#include <set>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    // pre: none
    // post: shows and hides widgets depending on what is happening
    void display();

    // pre: none
    // post: reads what is in the login text and shows account profile page if logged in
    void myLoginClick();

    // pre: none
    // post: reads whatyou clicked in the friends table and goes to their profile
    void profileClick(int row, int col);

    // pre: none
    // post: reads whatyou clicked in the suggestions table and goes to their profile
    void suggestionClick(int row, int col);

    // pre: none
    // post: goes back to account profile
    void backClick();

    // pre: none
    // post: adds connection between account and current profile, and updates user.txt
    void addFriendClick();

private:
    Ui::SocialNetworkWindow *ui;
    Network network;
    User* account;
    User* profile;
    bool getuser;
    int id;
    // class invariants
    // 1. ui holds login page, and all profile pages
    // 2. ui allows someone to login to their account, and will give error message if wrong login
    // 3. profile pages shows whose profile it belongs to, their friends, and their 5 or less recent posts
    // 4. if on logged in user's profile page, will suggest friends
    // 5. back button goes back to user's page
    // 6. add friend button will add friend and update network
};
#endif // SOCIALNETWORKWINDOW_H
