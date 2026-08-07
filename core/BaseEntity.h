#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <string>
using namespace std;

class BaseEntity {
protected:
    int id;

public:
    virtual void print() const = 0;
    virtual string toString() const = 0;
    virtual void fromString(const string& line) = 0;
    virtual void input(int newId) = 0;
    int getId() const { return id; }
    virtual ~BaseEntity() {}
};

#endif