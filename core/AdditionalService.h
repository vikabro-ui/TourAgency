#ifndef ADDITIONALSERVICE_H
#define ADDITIONALSERVICE_H

#include <string>
#include <sstream>
#include "BaseEntity.h"
using namespace std;

class AdditionalService : public BaseEntity {
private:
    int id;
    string name;
    double servicePrice;
    string description;

public:
    AdditionalService() : id(0), servicePrice(0) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getServicePrice() const { return servicePrice; }
    string getDescription() const { return description; }

    void setId(int newId) { id = newId; }
    void setName(const string& n) { name = n; }
    void setServicePrice(double p) { servicePrice = p; }
    void setDescription(const string& d) { description = d; }

    void print() const override {}
    string toString() const override {
        return to_string(id) + "|" + name + "|" + to_string(servicePrice) + "|" + description;
    }
    void fromString(const string& line) override {
        stringstream ss(line);
        string token;
        getline(ss, token, '|'); id = stoi(token);
        getline(ss, name, '|');
        getline(ss, token, '|'); servicePrice = stod(token);
        getline(ss, description, '|');
    }
    void input(int newId) override { id = newId; }
};

#endif