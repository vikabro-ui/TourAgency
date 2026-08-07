#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <sstream>
#include "BaseEntity.h"
using namespace std;

class Client : public BaseEntity {
private:
    string surname;
    int id;
    string name;
    string patronymic;
    string phone;
    string email;
    string passport;

public:
    Client() : id(0) {}
    Client(string s, string n, string p, string ph, string e, string pb)
        : surname(s), name(n), patronymic(p), phone(ph), email(e), passport(pb) {}

    int getId() const { return id; }
    string getSurname() const { return surname; }
    string getName() const { return name; }
    string getPatronymic() const { return patronymic; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    string getPassport() const { return passport; }
    string getFullName() const { return surname + " " + name + " " + patronymic; }

    void setId(int newId) { id = newId; }
    void setSurname(const string& s) { surname = s; }
    void setName(const string& n) { name = n; }
    void setPatronymic(const string& p) { patronymic = p; }
    void setPhone(const string& p) { phone = p; }
    void setEmail(const string& e) { email = e; }
    void setPassport(const string& p) { passport = p; }

    void print() const override {}
    string toString() const override {
        return to_string(id) + "|" + surname + "|" + name + "|" + patronymic + "|"
               + phone + "|" + email + "|" + passport;
    }
    void fromString(const string& line) override {
        stringstream ss(line);
        string token;
        getline(ss, token, '|'); id = stoi(token);
        getline(ss, surname, '|');
        getline(ss, name, '|');
        getline(ss, patronymic, '|');
        getline(ss, phone, '|');
        getline(ss, email, '|');
        getline(ss, passport, '|');
    }
    void input(int newId) override { id = newId; }
};

#endif