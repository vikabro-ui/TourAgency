#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql/QtSql>
#include <QSqlQuery>
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QStatusBar>
#include <QDialog>
#include <QWidget>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "core/TourAgency.h"
#include <QStackedWidget>
#include <QSpinBox>
#include <QHeaderView>

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    TourAgency *agency;
    QPushButton* btnLogin;
    QPushButton *btnLog;
    QPushButton* btnClients;
    QPushButton* btnTours;
    QPushButton* btnOrders;
    QPushButton* btnServices;
    QStackedWidget* pages;
    bool isAuthorized;
    void setupUI();
    QLineEdit *loginPasswordEdit;
    QLineEdit* phoneEdit;
    QPushButton* loginBtn;
    QPushButton* registerBtn;
    QLabel *errorLabel;
    QLabel* userStatusLabel;

    //page 1
    QLabel* clientIdLabel;
    QLabel* clientSurnameLabel;
    QLabel* clientNameLabel;
    QLabel* clientPatronymicLabel;
    QLabel* clientPhoneLabel;
    QLabel* clientEmailLabel;
    QLabel* clientPassportLabel;
    void setupClientsPage();
    void updateClientInfo();
    void editClientInfo();

    // page 2
    void setupToursPage();
    void loadToursToTable();
    void filterTours(int filterIndex);
    void openFilterDialog();
    QTableWidget* toursTable;
    void applyFilter(int filterType, const QString& country, const QString& priceStr);

    // page 3
    void setupOrdersPage();
    void loadOrdersTable();
    void createNewOrder();
    void cancelOrder();
    QTableWidget* ordersTable;
    QPushButton* newOrderBtn;
    QPushButton *cancelOrderBtn;

    //page 4
    void setupServicesPage();
    void loadServicesTable();
    QTableWidget* servicesTable;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLogin();
    void onRegister();
    void updateTabsAccess();
    void setupLoginPage();


};

#endif