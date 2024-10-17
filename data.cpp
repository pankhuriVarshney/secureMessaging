#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
using namespace std;

class Message {
public:
    string message;
    string sender;
    string receiver;

    Message(string mes, string sender, string receiver) : message(mes), sender(sender), receiver(receiver) {}
};

class ChatHistory {
public:
    static stack<Message> history;

    static void addMessage(const Message& msg) {
        history.push(msg);
    }

    static void displayHistory(const string& currentUser = "", bool publicOnly = false) {
        stack<Message> temp = history;
        while (!temp.empty()) {
            Message msg = temp.top();
            temp.pop();
            if (publicOnly && msg.receiver != "ALL") continue;
            if (currentUser.empty() || msg.sender == currentUser || msg.receiver == currentUser || msg.receiver == "ALL") {
                cout << msg.sender << " to " << msg.receiver << ": " << msg.message << endl;
            }
        }
    }
};

stack<Message> ChatHistory::history;

unordered_map<string, string> adminsList;
unordered_map<string, string> RegularUsersList;

class User {
protected:
    string userName;
    string password;

public:
    User(string name, string pass) : userName(name), password(pass) {}

    virtual void sendMessage() = 0;
    virtual void getMessage() = 0;

    bool validateCredentials(const string& enteredUserName, const string& enteredPassword) {
        return (enteredUserName == userName && enteredPassword == password);
    }

    string getUserName() const {
        return userName;
    }
};

class Admin : public User {
public:
    Admin(string userName, string password) : User(userName, password) {}

    void sendMessage() override {
        cout << "Enter the Receiver's ID (Enter 'ALL' to send message to all users): ";
        string receiver;
        cin.ignore();
        getline(cin, receiver);
        if (receiver != "ALL" && !(adminsList.count(receiver) || RegularUsersList.count(receiver))) {
            cout << "ERROR: Receiver ID Not Found\n";
            return;
        }
        cout << "Typing message: ";
        string mes;
        getline(cin, mes);
        ChatHistory::addMessage({mes, userName, receiver});
    }

    void getMessage() override {
        cout << "Chat History (latest to oldest):\n";
        ChatHistory::displayHistory();
    }
};

class RegularUser : public User {
public:
    RegularUser(string userName, string password) : User(userName, password) {}

    void sendMessage() override {
        cout << "Enter the Receiver's ID: ";
        string receiver;
        cin.ignore();
        getline(cin, receiver);
        if (!(adminsList.count(receiver) || RegularUsersList.count(receiver))) {
            cout << "ERROR: Receiver ID Not Found\n";
            return;
        }
        cout << "Typing message: ";
        string mes;
        getline(cin, mes);
        ChatHistory::addMessage({mes, userName, receiver});
    }

    void getMessage() override {
        cout << "Chat History (for " << userName << "):\n";
        ChatHistory::displayHistory(userName);
    }
};

class Guest : public User {
public:
    Guest(string userName) : User(userName, "") {}

    void sendMessage() override {
        cout << "PERMISSION DENIED: Guests cannot send messages.\n";
    }

    void getMessage() override {
        cout << "Chat History (for Guest):\n";
        ChatHistory::displayHistory("", true);
    }
};

void createUserAccount(char userType) {
    string userName, password;

    cout << "Enter a username: ";
    cin >> userName;
    cout << "Enter a password: ";
    cin >> password;
    if (adminsList.count(userName) || RegularUsersList.count(userName)) {
        cout << "Username already exists! Choose a different username.\n";
        return;
    }
    if (userType == 'A') adminsList[userName] = password;
    else RegularUsersList[userName] = password;

    cout << "Account created successfully!\n";
}

bool validateUser(char userType, string userName, string password) {
    unordered_map<string, string>* typeList = (userType == 'A') ? &adminsList : &RegularUsersList;
    auto it = typeList->find(userName);
    if (it != typeList->end() && it->second == password) {
        cout << userName << " login successful!\n";
        return true;
    }
    cout << "Invalid username or password.\n";
    return false;
}

int main() {
start:
    cout << "WELCOME TO THE CHAT!\n";
    cout << "1. Create an Admin Account\n";
    cout << "2. Create a Regular User Account\n";
    cout << "3. Login as Admin\n";
    cout << "4. Login as Regular User\n";
    cout << "5. Temporarily Login as Guest\n";
    cout << "6. Exit the Secure Messaging System\n";

    while (true) {
        cout << "MAIN MENU: Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            createUserAccount('A');
            break;
        case 2:
            createUserAccount('R');
            break;
        case 3: {
            string userName, password;
            cout << "Enter your username: ";
            cin >> userName;
            cout << "Enter your password: ";
            cin >> password;
            if (validateUser('A', userName, password)) {
                Admin admin(userName, password);
                cout << "WELCOME TO ADMIN CHAT!\n";
                cout << "1. Send a Message\n";
                cout << "2. View Group Chat History\n";
                cout << "5. Logout from Admin Account\n";
                while (true) {
                    cout << "ADMIN MENU: Enter your choice: ";
                    int secondChoice;
                    cin >> secondChoice;
                    switch (secondChoice) {
                    case 1: admin.sendMessage(); break;
                    case 2: admin.getMessage(); break;
                    case 5: goto start;
                    default: cout << "Invalid Choice!\n";
                    }
                    cout << "\n";
                }
            }
            break;
        }
        case 4: {
            string userName, password;
            cout << "Enter your username: ";
            cin >> userName;
            cout << "Enter your password: ";
            cin >> password;
            if (validateUser('R', userName, password)) {
                RegularUser ru(userName, password);
                cout << "WELCOME TO REGULAR USER CHAT!\n";
                cout << "1. Send a Message\n";
                cout << "2. View Group Chat History\n";
                cout << "5. Logout from Regular User Account\n";
                while (true) {
                    cout << "REGULAR USER MENU: Enter your choice: ";
                    int secondChoice;
                    cin >> secondChoice;
                    switch (secondChoice) {
                    case 1: ru.sendMessage(); break;
                    case 2: ru.getMessage(); break;
                    case 5: goto start;
                    default: cout << "Invalid Choice!\n";
                    }
                    cout << "\n";
                }
            }
            break;
        }
        case 5: {
            Guest guest("Guest");
            cout << "WELCOME TO GUEST USER CHAT!\n";
            cout << "1. View Group Chat History\n";
            cout << "3. Logout from Guest Account\n";
            while (true) {
                cout << "GUEST MENU: Enter your choice: ";
                int secondChoice;
                cin >> secondChoice;
                switch (secondChoice) {
                case 1: guest.getMessage(); break;
                case 3: goto start;
                default: cout << "Invalid Choice!\n";
                }
                cout << "\n";
            }
            break;
        }
        case 6:
            cout << "EXITING... SEE YOU LATER...\n";
            exit(0);
        default:
            cout << "Invalid choice.\n";
        }
        cout << "\n\n";
    }

    return 0;
}
