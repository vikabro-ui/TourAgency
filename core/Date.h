#ifndef DATE_H
#define DATE_H

#include <string>
#include <ctime>
using namespace std;

class Date {
public:
    int day, month, year;

    Date() : day(1), month(1), year(2000) {}

    string toString() const {
        return to_string(day) + "." + to_string(month) + "." + to_string(year);
    }

    void fromString(string str) {
        if (str.empty()) return;
        int pos1 = str.find(".");
        int pos2 = str.rfind(".");
        if (pos1 == -1 || pos2 == -1 || pos1 == pos2) {
            day = 1; month = 1; year = 2000;
            return;
        }
        day = stoi(str.substr(0, pos1));
        month = stoi(str.substr(pos1 + 1, pos2 - pos1 - 1));
        year = stoi(str.substr(pos2 + 1));
    }

    void setCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }
};

#endif