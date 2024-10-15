#include <iostream>
#include <stack>
#include <string>
using namespace std;


class Message{
    public:
    string message;
    string sender;
    string receiver;

    Message(string mes, string sender, string receiver) : message(mes), sender(sender), receiver(receiver) {};
};

class ChatHistory{
    static stack<Message> history;

    static void addMessage(const Message& msg){
        history.push(msg);
    }

    static void displayHistory(){
        stack<Message> temp = history;
        while (!temp.empty()) {
            Message msg = temp.top();
            temp.pop();
            cout << msg.sender << " to " << msg.receiver << ": " << msg.message << endl;
        }
    }
};

stack<Message> ChatHistory::history;


class User: public Message
{
protected:
    string userName;
    string password;

public:
    User(string name, string password) : userName(name), password(password) {};
    virtual void sendMessage()=0;
    virtual void getMessage()=0;
};


class Admin : public User
{
public:
    Admin(string userName, string password) : User(userName, password) {};

    void sendMessage(){
        cout<< "Enter the Reciever's ID (Enter 'ALL' to send message to all users): ";
        string receiver;
        getline(cin, receiver);
        cout<< "Typing...";
        string mes;
        getline(cin, mes);
        Message newMessage(mes, "ADMIN", receiver);
        ChatHistory::addMessage(newMessage);
    };
};

class RegularUser : public User
{
};

class Guest : public User
{
};

int main()
{
}