#ifndef TOUR_H
#define TOUR_H

#include <string>
#include <sstream>
#include "BaseEntity.h"
#include "Date.h"
using namespace std;

class Tour : public BaseEntity {
private:
    int id;
    string title;
    string country;
    Date startDate;
    Date endDate;
    double price;
    int availableSeats;

public:
    Tour() : id(0), price(0), availableSeats(0) {}
    Tour(int i, string t, string c, string startStr, string endStr, double pr, int av)
        : id(i), title(t), country(c), price(pr), availableSeats(av) {
        startDate.fromString(startStr);
        endDate.fromString(endStr);
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getCountry() const { return country; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    double getPrice() const { return price; }
    int getAvailableSeats() const { return availableSeats; }

    void setId(int newId) { id = newId; }
    void setTitle(const string& t) { title = t; }
    void setCountry(const string& c) { country = c; }
    void setPrice(double p) { price = p; }
    void setAvailableSeats(int s) { availableSeats = s; }

    bool isHot() const { return availableSeats <= 2; }
    double getCurrentPrice() const { return isHot() ? price * 0.9 : price; }
    bool hasSeats(int count) const { return availableSeats >= count; }
    void bookSeats(int count) { if (hasSeats(count)) availableSeats -= count; }
    void cancelSeats(int count) { availableSeats += count; }

    void print() const override {}
    string toString() const override {
        return to_string(id) + "|" + title + "|" + country + "|"
               + startDate.toString() + "|" + endDate.toString() + "|"
               + to_string(price) + "|" + to_string(availableSeats);
    }
    void fromString(const string& line) override {
        stringstream ss(line);
        string token;
        getline(ss, token, '|'); id = stoi(token);
        getline(ss, title, '|');
        getline(ss, country, '|');
        getline(ss, token, '|'); startDate.fromString(token);
        getline(ss, token, '|'); endDate.fromString(token);
        getline(ss, token, '|'); price = stod(token);
        getline(ss, token, '|'); availableSeats = stoi(token);
    }
    void input(int newId) override { id = newId; }
};

#endif