#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

struct User {
    string email;
    string name;
    string username;
    string password;
    vector<string> followers;
    vector<string> messages;
    vector<string> feed;
    vector<string> hobbies;
};

map<string, User> users;

void createUser() {
    User user;
    cout << "Enter email: ";
    cin >> user.email;
    cout << "Enter name: ";
    cin >> user.name;
    cout << "Enter username: ";
    cin >> user.username;
    
    // Check if username is already taken
    if (users.find(user.username) != users.end()) {
        cout << "Username is already taken. Please choose a different one." << endl;
        return;
    }
    
    cout << "Enter password: ";
    cin >> user.password;
    
    users[user.username] = user;
    cout << "User created successfully!" << endl;
}

bool login(string& loggedInUser) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    
    // Check if user exists
    if (users.find(username) != users.end()) {
        User& user = users[username];
        // Check password
        if (user.password == password) {
            loggedInUser = username;
            cout << "Logged in as " << username << endl;
            return true;
        }
    }
    
    cout << "Invalid username or password." << endl;
    return false;
}

void logout(string& loggedInUser) {
    loggedInUser = "";
    cout << "Logged out successfully!" << endl;
}

void followUser(const string& loggedInUser) {
    string username;
    cout << "Enter username to follow: ";
    cin >> username;
    
    // Check if user exists
    if (users.find(username) != users.end()) {
        User& user = users[username];
        if (user.username != loggedInUser) {
            // Check if already following
            if (find(user.followers.begin(), user.followers.end(), loggedInUser) == user.followers.end()) {
                user.followers.push_back(loggedInUser);
                cout << "You are now following " << username << endl;
                return;
            }
            cout << "You are already following " << username << endl;
            return;
        }
        cout << "You cannot follow yourself." << endl;
        return;
    }
    
    cout << "User does not exist." << endl;
}

void unfollowUser(const string& loggedInUser) {
    string username;
    cout << "Enter username to unfollow: ";
    cin >> username;
    
    // Check if user exists
    if (users.find(username) != users.end()) {
        User& user = users[username];
        if (user.username != loggedInUser) {
            // Check if already following
            auto it = find(user.followers.begin(), user.followers.end(), loggedInUser);
            if (it != user.followers.end()) {
                user.followers.erase(it);
                cout << "You have unfollowed " << username << endl;
                return;
            }
            cout << "You are not following " << username << endl;
            return;
        }
        cout << "You cannot unfollow yourself." << endl;
        return;
    }
    
    cout << "User does not exist." << endl;
}

void sendMessage(const string& loggedInUser) {
    string recipient, message;
    cout << "Enter recipient's username: ";
    cin >> recipient;
    
    // Check if recipient exists
    if (users.find(recipient) != users.end()) {
        User& user = users[recipient];
        if (user.username != loggedInUser) {
            cout << "Enter your message: ";
            cin.ignore();
            getline(cin, message);
            user.messages.push_back(message);
            cout << "Message sent successfully!" << endl;
            return;
        }
        cout << "You cannot send a message to yourself." << endl;
        return;
    }
    
    cout << "Recipient does not exist." << endl;
}

void sendBroadcast(const string& loggedInUser) {
    string message;
    cout << "Enter your broadcast message: ";
    cin.ignore();
    getline(cin, message);
    
    // Update followers' feed
    for (auto& pair : users) {
        User& user = pair.second;
        if (user.username != loggedInUser) {
            user.feed.push_back(message);
        }
    }
    
    cout << "Broadcast message sent successfully!" << endl;
}

void seeMessages(const string& loggedInUser) {
    User& user = users[loggedInUser];
    
    cout << "Messages: " << endl;
    for (const string& message : user.messages) {
        cout << message << endl;
    }
    
    // Clear messages after reading
    user.messages.clear();
}

void seeFeed(const string& loggedInUser) {
    User& user = users[loggedInUser];
    
    cout << "Feed: " << endl;
    for (const string& post : user.feed) {
        cout << post << endl;
    }
    
    // Clear feed after reading
    user.feed.clear();
}

void addHobbies(const string& loggedInUser) {
    string hobby;
    cout << "Enter your hobby (Enter 'done' to finish): " << endl;
    
    User& user = users[loggedInUser];
    
    while (true) {
        cin >> hobby;
        
        if (hobby == "done") {
            break;
        }
        
        user.hobbies.push_back(hobby);
    }
}

void suggestFriends(const string& loggedInUser) {
    User& user = users[loggedInUser];
    
    vector<string> suggestedFriends;
    
    for (auto& pair : users) {
        const User& otherUser = pair.second;
        if (otherUser.username != loggedInUser) {
            // Check if other user has at least one common hobby
            for (const string& hobby : otherUser.hobbies) {
                if (find(user.hobbies.begin(), user.hobbies.end(), hobby) != user.hobbies.end()) {
                    suggestedFriends.push_back(otherUser.username);
                    break;
                }
            }
        }
    }
    
    cout << "Suggested friends based on matching hobbies: " << endl;
    for (const string& username : suggestedFriends) {
        cout << username << endl;
    }
}

int main() {
    string loggedInUser;
    int choice;
    
    while (true) {
        cout << "-------------------------" << endl;
        cout << "1. Create new user" << endl;
        cout << "2. Login" << endl;
        cout << "3. Logout" << endl;
        cout << "4. Follow someone" << endl;
        cout << "5. Unfollow someone" << endl;
        cout << "6. Send personal message" << endl;
        cout << "7. Send broadcast message" << endl;
        cout << "8. See my messages" << endl;
        cout << "9. See my feed"<< endl;
        cout << "10. Add hobbies" << endl;
        cout << "11. Suggest friends" << endl;
        cout << "12. Exit" << endl;
        cout << "-------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "-------------------------" << endl;
        
        switch (choice) {
            case 1:
                createUser();
                break;
            case 2:
                if (loggedInUser.empty()) {
                    login(loggedInUser);
                } else {
                    cout << "You are already logged in." << endl;
                }
                break;
            case 3:
                if (!loggedInUser.empty()) {
                    logout(loggedInUser);
                } else {
                    cout << "You are not logged in." << endl;
                }
                break;
            case 4:
                if (!loggedInUser.empty()) {
                    followUser(loggedInUser);
                } else {
                    cout << "Please log in to follow someone." << endl;
                }
                break;
            case 5:
                if (!loggedInUser.empty()) {
                    unfollowUser(loggedInUser);
                } else {
                    cout << "Please log in to unfollow someone." << endl;
                }
                break;
            case 6:
                if (!loggedInUser.empty()) {
                    sendMessage(loggedInUser);
                } else {
                    cout << "Please log in to send a message." << endl;
                }
                break;
            case 7:
                if (!loggedInUser.empty()) {
                    sendBroadcast(loggedInUser);
                } else {
                    cout << "Please log in to send a broadcast message." << endl;
                }
                break;
            case 8:
                if (!loggedInUser.empty()) {
                    seeMessages(loggedInUser);
                } else {
                    cout << "Please log in to see your messages." << endl;
                }
                break;
            case 9:
                if (!loggedInUser.empty()) {
                    seeFeed(loggedInUser);
                } else {
                    cout << "Please log in to see your feed." << endl;
                }
                break;
            case 10:
                if (!loggedInUser.empty()) {
                    addHobbies(loggedInUser);
                } else {
                    cout << "Please log in to add hobbies." << endl;
                }
                break;
            case 11:
                if (!loggedInUser.empty()) {
                    suggestFriends(loggedInUser);
                } else {
                    cout << "Please log in to suggest friends." << endl;
                }
                break;
            case 12:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    
    return 0;
}