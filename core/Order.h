#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Date.h"
using namespace std;

class Order {
private:
    int id;
    int clientId;
    int tourId;
    Date orderDate;
    int touristsCount;
    double tourPrice;
    vector<int> serviceIds;
    double totalPrice;
    double servicesTotal;
    string status;

public:
    Order() : id(0), clientId(0), tourId(0), touristsCount(0),
        tourPrice(0), totalPrice(0), servicesTotal(0) {}

    int getId() const { return id; }
    int getClientId() const { return clientId; }
    int getTourId() const { return tourId; }
    Date getOrderDate() const { return orderDate; }
    int getTouristsCount() const { return touristsCount; }
    double getTotalPrice() const { return totalPrice; }
    string getStatus() const { return status; }
void setServicesTotal(double value) { servicesTotal = value; }
    void setId(int newId) { id = newId; }
    void setClientId(int cid) { clientId = cid; }
    void setTourId(int tid) { tourId = tid; }
    void setStatus(const string& s) { status = s; }
    void setTouristsCount(int count) { touristsCount = count; }
    void setTotalPrice(double price) { totalPrice = price; }
    void setOrderDate(const Date& date) { orderDate = date; }
    void setTourPrice(double price) { tourPrice = price; }
    void setServiceIds(const vector<int>& ids) { serviceIds = ids; }
    void create(int newId, int cId, int tId, int tourists, double tPrice) {
        id = newId;
        clientId = cId;
        tourId = tId;
        touristsCount = tourists;
        tourPrice = tPrice;
        orderDate.setCurrentDate();
        status = "Подтверждён";
        servicesTotal = 0;
        calculateTotal();
    }

    void calculateTotal() {
        totalPrice = (tourPrice + servicesTotal) * touristsCount;
    }

    void print() const {}
};

#endif