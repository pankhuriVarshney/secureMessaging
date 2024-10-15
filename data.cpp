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

    static void displayHistory() {
        stack<Message> temp = history;
        while (!temp.empty()) {
            Message msg = temp.top();
            temp.pop();
            cout << msg.sender << " to " << msg.receiver << ": " << msg.message << endl;
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

    virtual bool validateCredentials(const string& enteredUserName, const string& enteredPassword) {
        return (enteredUserName == userName && enteredPassword == password);
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
        if(receiver!="ALL"&&!((adminsList.find(receiver) != adminsList.end())||(RegularUsersList.find(receiver) != RegularUsersList.end()))){
            cout<<"ERROR: Reciever ID Not Found\n";
            return;
        }
        cout << "Typing message: ";
        string mes;
        getline(cin, mes);
        Message newMessage(mes, userName, receiver);
        ChatHistory::addMessage(newMessage);
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
        cout << "Enter the Receiver's ID (Enter 'ALL' to send message to all users): ";
        string receiver;
        cin.ignore();
        getline(cin, receiver);
        if(receiver!="ALL"&&!((adminsList.find(receiver) != adminsList.end())||(RegularUsersList.find(receiver) != RegularUsersList.end()))){
            cout<<"ERROR: Reciever ID Not Found\n";
            return;
        }
        cout << "Typing message: ";
        string mes;
        getline(cin, mes);
        Message newMessage(mes, userName, receiver);
        ChatHistory::addMessage(newMessage);
    }

    void getMessage() override {
        cout << "Chat History (for "<<userName<<"):\n";
        ChatHistory::displayHistory();
    }
};

class Guest : public User {
public:
    Guest(string userName, string password) : User(userName, password) {}

    void sendMessage() override {
        cout << "Guests cannot send messages.\n";
    }

    void getMessage() override {
        cout << "Chat History (for Guest):\n";
        ChatHistory::displayHistory();
    }
};


void createUserAccount(char userType) {
    string userName, password;

    cout << "Enter a username: ";
    cin >> userName;
    cout << "Enter a password: ";
    cin >> password;
     if ((adminsList.find(userName) != adminsList.end())||(RegularUsersList.find(userName) != RegularUsersList.end())) {
                cout << "Username already exists! Choose a different username.\n";
                return;
            }
    switch(userType){
        case 'A':{
            adminsList[userName] = password;
            break;
        }
        case 'R':{
            RegularUsersList[userName]=password;
            break;
        }
    }

    cout << "Account created successfully!\n";
}

bool validateUser(char userType, string userName, string password) {
    
    unordered_map<string, string> *typeList=nullptr;
    switch(userType){
        case 'A':{
            typeList = &adminsList;
            break;
        }
        case 'R':{
           typeList = &RegularUsersList;
           break;
        }
    }
    auto it = typeList->find(userName);
    if (it != typeList->end() && it->second == password) {
        cout <<userName << " login successful!\n";
        return true;
    } 
    else {
        cout << "Invalid username or password.\n";
        return false;
    }
}

int main() {
    cout << "WELCOME TO THE CHAT!\n";
    cout << "1. Create an Admin Account\n";
    cout << "2. Create a Regular User Account\n";
    cout << "3. Create a Guest Account\n";
    cout << "4. Login as Admin\n";
    cout << "5. Login as Regular User\n";
    cout << "6. Login as Guest\n";

    while(true){
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            createUserAccount('A');
            break;
        case 2:
            createUserAccount('R');
            break;
        case 3:
            createUserAccount('G');
            break;
        case 4: {
            string userName, password;
            cout << "Enter your username: ";
            cin >> userName;
            cout << "Enter your password: ";
            cin >> password;
            if (validateUser('A', userName, password)) {
                Admin admin(userName, password);
                admin.sendMessage();
                admin.getMessage();
            }
            break;
        }
        case 5: {
            string userName, password;
            cout << "Enter your username: ";
            cin >> userName;
            cout << "Enter your password: ";
            cin >> password;
            if (validateUser('R', userName, password)) {
                RegularUser ru(userName, password);
                ru.sendMessage();
                ru.getMessage();
            }
            break;
        }
        case 6: {
            Guest guest("Guest", "");
            guest.getMessage();
            break;
        }
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
