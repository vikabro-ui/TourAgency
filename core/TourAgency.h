#ifndef TOURAGENCY_H
#define TOURAGENCY_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Client.h"
#include "Tour.h"
#include "Order.h"
#include "AdditionalService.h"
using namespace std;

class TourAgency {
private:
    vector<Client> clients;
    vector<Tour> tours;
    vector<AdditionalService> services;
    vector<Order> orders;
    int currentClientId;
    string currentClientName;
    vector<string> tempnameserv;

public:
    TourAgency() : currentClientId(-1), currentClientName("") {}

    vector<Client>& getClients() { return clients; }
    vector<Tour>& getTours() { return tours; }
    vector<Order>& getOrders() { return orders; }
    vector<AdditionalService>& getServices() { return services; }

    int getCurrentClientId() const { return currentClientId; }
    string getCurrentClientName() const { return currentClientName; }
    void setCurrentClientId(int id) { currentClientId = id; }
    void setCurrentClientName(const string& name) { currentClientName = name; }

    // Методы (объявления)
    int findClientById(int id);
    int findClientByPhone(const string& phone);
    int findTourById(int id);
    int findServiceById(int id);
    bool hasClientOrders(int clientId);
    int getNextClientId();
    int getNextOrderId();

    void loadClients();
    void saveClients();
    void loadTours();
    void saveTours();
    void loadServices();
    void saveServices();
    void loadOrders();
    void saveOrders();
    void loadAllData();
    void saveAllData();

    void addClient();
    void editClient();
    void createOrder();
    void cancelOrder();
    void listOrders();
    void showToursWithFilter();
    void listTours();
    void listHotTours();
    void filterToursByCountry();
    void filterToursByMaxPrice();
    void listServices();
    double choseAddServices();
    void printReceipt(int orderId);
    void run();
};

#endif