#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
QSqlDatabase db;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(900, 570);
    isAuthorized = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("tour_agency");
    db.setUserName("postgres");
    db.setPassword("12082008");
    db.setPort(5432);

    if (db.open()) {
        qDebug() << " Подключено к PostgreSQL!";
    } else {
        qDebug() << " Ошибка:" << db.lastError().text();
        qDebug() << " Работаем с файлами!";
    }
    agency = new TourAgency();
    agency->loadAllData();
    setupUI();
    updateTabsAccess();
}

MainWindow::~MainWindow()
{delete agency;}

void MainWindow::updateTabsAccess()
{
    btnClients->setEnabled(isAuthorized);
    btnTours->setEnabled(isAuthorized);
    btnOrders->setEnabled(isAuthorized);
    btnServices->setEnabled(isAuthorized);

    if (isAuthorized) {
        btnClients->setChecked(true);
        pages->setCurrentIndex(1);
    } else {
        btnLogin->setChecked(true);
        pages->setCurrentIndex(0);
    }
}

void MainWindow::setupUI()
{
    QWidget *mainWidget = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);
    setCentralWidget(mainWidget);

    QWidget* menuWidget = new QWidget();
    menuWidget->setFixedWidth(150);
    QVBoxLayout *menulayout=new QVBoxLayout(menuWidget);
    menulayout->setAlignment(Qt::AlignTop);
    menulayout->setSpacing(5);

    btnLogin = new QPushButton("🔐 Вход");
    btnClients = new QPushButton("👤 Клиент");
    btnTours = new QPushButton("✈ Туры");
    btnOrders = new QPushButton("📋 Заказы");
    btnServices = new QPushButton("🎯 Услуги");

    QSize buttonSize(130, 60);

    btnLogin->setFixedSize(buttonSize);
    btnClients->setFixedSize(buttonSize);
    btnTours->setFixedSize(buttonSize);
    btnOrders->setFixedSize(buttonSize);
    btnServices->setFixedSize(buttonSize);

    menulayout->addWidget(btnLogin);
    menulayout->addWidget(btnClients);
    menulayout->addWidget(btnTours);
    menulayout->addWidget(btnOrders);
    menulayout->addWidget(btnServices);
    menulayout->addStretch();

    mainLayout->addWidget(menuWidget);
    menulayout->setSpacing(40);
    pages=new QStackedWidget();
    setupLoginPage();
    setupClientsPage();
    setupToursPage();
    setupOrdersPage();
    setupServicesPage();
    mainLayout->addWidget(pages);

    connect(btnLogin, &QPushButton::clicked, this, [=](){pages->setCurrentIndex(0);});
    connect(btnClients, &QPushButton::clicked, this, [=]() {pages->setCurrentIndex(1);});
    connect(btnTours, &QPushButton::clicked, this, [=]() {pages->setCurrentIndex(2);});
    connect(btnOrders, &QPushButton::clicked, this, [=]() {pages->setCurrentIndex(3);});
    connect(btnServices, &QPushButton::clicked, this, [=]() {pages->setCurrentIndex(4);});
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegister);
    connect(btnLog, &QPushButton::clicked, this, &MainWindow::onLogin);
}

void MainWindow::setupClientsPage()
{
    QWidget* page1 = new QWidget();
    QVBoxLayout *layout=new QVBoxLayout(page1);
    layout->setAlignment(Qt::AlignTop);

    QLabel *title=new QLabel("👤 ЛИЧНЫЙ КАБИНЕТ");
    title->setStyleSheet("font-size: 22px; font-weight: bold");
    layout->addWidget(title);

    QGroupBox *infoGroup=new QGroupBox("Мои данные");
    QFormLayout *formLayout=new QFormLayout;

    clientIdLabel = new QLabel("—");
    clientSurnameLabel = new QLabel("—");
    clientNameLabel = new QLabel("—");
    clientPatronymicLabel = new QLabel("—");
    clientPhoneLabel = new QLabel("—");
    clientEmailLabel = new QLabel("—");
    clientPassportLabel = new QLabel("—");

    formLayout->addRow("ID:", clientIdLabel );
    formLayout->addRow("Фамилия:", clientSurnameLabel);
    formLayout->addRow("Имя:", clientNameLabel);
    formLayout->addRow("Отчество:", clientPatronymicLabel);
    formLayout->addRow("Телефон:", clientPhoneLabel);
    formLayout->addRow("Email:", clientEmailLabel);
    formLayout->addRow("Паспорт:", clientPassportLabel);

    infoGroup->setLayout(formLayout);
    layout->addWidget(infoGroup);
    layout->addSpacing(20);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* editBtn = new QPushButton("✏ Редактировать");
    btnLayout->addWidget(editBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
    layout->addStretch();
    pages->addWidget(page1);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editClientInfo);
}

void MainWindow::updateClientInfo()
{
    if(!isAuthorized)
    {
        QMessageBox::warning(this,"Ошибка", "Вы не авторизованы!");
        return;
    }

    int clientId = agency->getCurrentClientId();
    int index = agency->findClientById(clientId);

    if (index == -1) {
        QMessageBox::warning(this, "Ошибка", "Клиент не найден!");
        return;
    }

    Client &client=agency->getClients()[index];
    clientIdLabel->setText(QString::number(client.getId()));
    clientSurnameLabel->setText(QString::fromStdString(client.getSurname()));
    clientNameLabel->setText(QString::fromStdString(client.getName()));
    clientPatronymicLabel->setText(QString::fromStdString(client.getPatronymic()));
    clientPhoneLabel->setText(QString::fromStdString(client.getPhone()));
    clientEmailLabel->setText(QString::fromStdString(client.getEmail()));
    clientPassportLabel->setText(QString::fromStdString(client.getPassport()));
}

void MainWindow::editClientInfo()
{
    if (!isAuthorized) {
        QMessageBox::warning(this, "Ошибка", "Вы не авторизованы!");
        return;
    }

    int clientId = agency->getCurrentClientId();
    int index = agency->findClientById(clientId);

    if (index == -1) {
        QMessageBox::warning(this, "Ошибка", "Клиент не найден!");
        return;
    }
     Client& client = agency->getClients()[index];
    QDialog dialog(this);
    dialog.setWindowTitle("Редактирование данных");
    dialog.resize(350, 400);

    QVBoxLayout *layout=new QVBoxLayout(&dialog);

    QLabel* title = new QLabel(" РЕДАКТИРОВАНИЕ");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    layout->addSpacing(10);

    QLineEdit* surname = new QLineEdit(QString::fromStdString(client.getSurname()));
    surname->setPlaceholderText("Фамилия");
    QLineEdit* name = new QLineEdit(QString::fromStdString(client.getName()));
    name->setPlaceholderText("Имя");
    QLineEdit* patronymic = new QLineEdit(QString::fromStdString(client.getPatronymic()));
    patronymic->setPlaceholderText("Отчество");
    QLineEdit* phone = new QLineEdit(QString::fromStdString(client.getPhone()));
    phoneEdit->setPlaceholderText("Телефон");
    QLineEdit* email = new QLineEdit(QString::fromStdString(client.getEmail()));
    email->setPlaceholderText("Email");
    QLineEdit* passport = new QLineEdit(QString::fromStdString(client.getPassport()));
    passport->setPlaceholderText("Паспорт");
    layout->addWidget(surname);
    layout->addWidget(name);
    layout->addWidget(patronymic);
    layout->addWidget(phone);
    layout->addWidget(email);
    layout->addWidget(passport);
    layout->addSpacing(15);

    QDialogButtonBox *buttons=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
     layout->addWidget(buttons);

    connect (buttons, &QDialogButtonBox::accepted,  &dialog, &QDialog::accept);
     connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(dialog.exec()==QDialog::Accepted)
     {
         if (surname->text().isEmpty() || name->text().isEmpty() || phone->text().isEmpty())
        {
             QMessageBox::warning(this, "Ошибка", "Фамилия, имя и телефон обязательны!");
             return;
         }

         client.setSurname(surname->text().toStdString());
         client.setName(name->text().toStdString());
         client.setPatronymic(patronymic->text().toStdString());
         client.setPhone(phone->text().toStdString());
         client.setEmail(email->text().toStdString());
         client.setPassport(passport->text().toStdString());

         agency->saveClients();
         updateClientInfo();

         QMessageBox::information(this, "Успех", "Данные обновлены!");
    }
}

void MainWindow::setupLoginPage()
{
    QWidget* page0 = new QWidget();
    QVBoxLayout *layout=new QVBoxLayout(page0);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title=new QLabel("🔐 ВХОД В СИСТЕМУ");
    title->setStyleSheet("font-size: 20px; font-weight:bold;");
    layout->addWidget(title);

    layout->addSpacing(30);

    phoneEdit= new QLineEdit;
    phoneEdit->setPlaceholderText("Номер телефона");
    phoneEdit->setFixedWidth(250);
    layout->addWidget(phoneEdit,0,Qt::AlignCenter);
    layout->addSpacing(20);

    btnLog=new QPushButton("Вход");
    btnLog->setFixedWidth(150);
    layout->addWidget(btnLog, 0, Qt::AlignCenter);

    registerBtn = new QPushButton("Зарегистрироваться");
    registerBtn->setFixedWidth(150);
    layout->addWidget(registerBtn, 0, Qt::AlignCenter);

    layout->addStretch();

    pages->addWidget(page0);
}
void MainWindow::onLogin()
{
    QString phone=phoneEdit->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите номер телефона!");
        return;
    }

    int index=agency->findClientByPhone(phone.toStdString());
    if(index==-1)
    {
        QMessageBox::warning(this, "Ошибка", "Клиент с таким номером не найден!");
        return;
    }

    isAuthorized=true;
    agency->setCurrentClientId(agency->getClients()[index].getId());
    agency->setCurrentClientName(agency->getClients()[index].getFullName());

    updateTabsAccess();
updateClientInfo();
    QMessageBox::information(this, "Успех", "Добро пожаловать, "+ QString::fromStdString(agency->getCurrentClientName()) + "!");
    agency->loadOrders();
    loadOrdersTable();
}

void MainWindow::onRegister()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Регистрация");
    dialog.resize(350, 400);
    QVBoxLayout *layout=new QVBoxLayout(&dialog);

    QLabel* title = new QLabel("📝 РЕГИСТРАЦИЯ");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(10);
    QLineEdit *surname=new QLineEdit;
    surname->setPlaceholderText("Фамилия*");
    QLineEdit *name=new QLineEdit;
    name->setPlaceholderText("Имя*");
    QLineEdit *patronymic = new QLineEdit();
    patronymic->setPlaceholderText("Отчество");
    QLineEdit *phone=new QLineEdit;
    phone->setPlaceholderText("Телефон*");
    QLineEdit *passport = new QLineEdit();
    passport->setPlaceholderText("Паспортные данные");

    layout->addWidget(surname);
    layout->addWidget(name);
    layout->addWidget(patronymic);
    layout->addWidget(phone);
    layout->addWidget(passport);
    QDialogButtonBox *buttons=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(dialog.exec()==QDialog::Accepted)
    {
        if (surname->text().isEmpty() || name->text().isEmpty() || phone->text().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Заполните обязательные поля(*)!");
            return;
        }
        if (agency->findClientByPhone(phone->text().toStdString()) != -1) {
            QMessageBox::warning(this, "Ошибка", "Клиент с таким телефоном уже существует!");
            return;
        }
        int newId=agency->getNextClientId();

        Client newClient(
            surname->text().toStdString(),
            name->text().toStdString(),
            patronymic->text().toStdString(),
            phone->text().toStdString(),
            passport->text().toStdString(),
            ""
            );
        newClient.setId(newId);
        agency->getClients().push_back(newClient);
        agency->saveClients();

        QMessageBox::information(this,"Успех", "Вы успешно зарегестрировались!");

    }
}

void MainWindow::setupToursPage()
{
    QWidget* page2 = new QWidget();

    QVBoxLayout *layout= new QVBoxLayout(page2);
    layout->setSpacing(15);

    QLabel* title = new QLabel("✈ ВСЕ ТУРЫ");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(10);

    toursTable=new QTableWidget;
    toursTable->setColumnCount(7);
    toursTable->setHorizontalHeaderLabels({"ID", "Название", "Страна", "Дней", "Цена (руб)", "Мест", "Статус"});
    toursTable->horizontalHeader()->setStretchLastSection(true);
    toursTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toursTable->setAlternatingRowColors(true);
    layout->addWidget(toursTable);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* filterBtn = new QPushButton("🔍 Фильтр");
    filterBtn->setFixedWidth(150);
    filterBtn->setStyleSheet("background-color: #3498db; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    btnLayout->addWidget(filterBtn);

    layout->addLayout(btnLayout);

    pages->addWidget(page2);
    agency->loadTours();
    loadToursToTable();
    connect(filterBtn, &QPushButton::clicked, this, &MainWindow::openFilterDialog);
}

void MainWindow::openFilterDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Фильтр туров");
    dialog.resize(350, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* title = new QLabel("🔍 НАСТРОЙКА ФИЛЬТРА");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QGroupBox *typeGroup=new QGroupBox("Тип тура");
    QVBoxLayout *typeLayout=new QVBoxLayout;
    QComboBox *typeCombo=new QComboBox;
    typeCombo->addItem("Все туры");
    typeCombo->addItem("Только горящие");
    typeCombo->addItem("Обычые туры");
    typeLayout->addWidget(typeCombo);
    typeGroup->setLayout(typeLayout);
    layout->addWidget(typeGroup);

    QGroupBox *priceGroup=new QGroupBox("Максимальная цена");
    QVBoxLayout *priceLayout=new QVBoxLayout;
    QLineEdit *priceEdit=new QLineEdit;
    priceEdit->setPlaceholderText("например 500");
    priceLayout->addWidget(priceEdit);
    priceGroup->setLayout(priceLayout);
    layout->addWidget(priceGroup);

    QGroupBox* countryGroup = new QGroupBox("Страна");
    QVBoxLayout* countryLayout = new QVBoxLayout();
    QLineEdit* countryEdit = new QLineEdit();
    countryEdit->setPlaceholderText("Введите страну...");
    countryLayout->addWidget(countryEdit);
    countryGroup->setLayout(countryLayout);
    layout->addWidget(countryGroup);

      layout->addSpacing(10);

    QHBoxLayout *btnLayout=new QHBoxLayout;
    QPushButton *applybtn=new QPushButton("Применить");
    QPushButton *resetbtn=new QPushButton ("Сбросить");
    QPushButton *cancelbtn=new QPushButton("Отмена");
    applybtn->setStyleSheet("background-color: #27ae60; color: white;");
    resetbtn->setStyleSheet("background-color: #f39c12; color: white;");
    btnLayout->addWidget(applybtn);
    btnLayout->addWidget(resetbtn);
    btnLayout->addWidget(cancelbtn);
    layout->addLayout(btnLayout);

    connect(applybtn, &QPushButton::clicked, &dialog, [&]()
    {
        int fyltertype=typeCombo->currentIndex();
        QString country=countryEdit->text().trimmed();
        QString price=priceEdit->text().trimmed();
        applyFilter(fyltertype, country,price);
         dialog.accept();
    });

    connect(resetbtn, &QPushButton::clicked, &dialog, [&]()
    {
        typeCombo->setCurrentIndex(0);
        countryEdit->clear();
        priceEdit->clear();
        loadToursToTable();
    });

    connect (cancelbtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    dialog.exec();
}
void MainWindow::loadToursToTable()
{
    vector <Tour> &tours=agency->getTours();

    toursTable->setRowCount((int)tours.size());
    for(int i=0;i<(int)tours.size();++i)
    {
        Tour& t = tours[i];
        toursTable->setItem(i, 0, new QTableWidgetItem(QString::number(t.getId())));
        toursTable->setItem(i,1, new QTableWidgetItem(QString::fromStdString(t.getTitle())));
        toursTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(t.getCountry())));

        Date start=t.getStartDate();
        Date end=t.getEndDate();
        int days=(end.year-start.year)*365+(end.month-start.month)*30+(end.day-start.day);
        if (days < 0) days = -days;

        toursTable->setItem(i,3, new QTableWidgetItem(QString::number(days)));
        toursTable->setItem(i, 4, new QTableWidgetItem(QString::number(t.getPrice())));
        toursTable->setItem(i, 5, new QTableWidgetItem(QString::number(t.getAvailableSeats())));

        QString status=t.isHot()?"Горящий": "Обычный";
        toursTable->setItem(i,6, new QTableWidgetItem(status));
    }
     toursTable->resizeColumnsToContents();
}
void MainWindow::applyFilter(int filterType, const QString& country, const QString& priceStr )
{
    vector<Tour> &tours=agency->getTours();
    vector<Tour> res;
    if(filterType==0)
    {
         res = tours;
    }

    if(filterType==1)
    {
        for(int i=0;i<(int)tours.size();++i)
        {
            if(tours[i].isHot())
            {
                res.push_back(tours[i]);
            }
        }
    }

    if(filterType==2)
    {
        for(int i=0;i<(int)tours.size();++i)
        {
            if(!tours[i].isHot())
            {
                res.push_back(tours[i]);
            }
        }
    }

    if (!country.isEmpty())
    {
        for (int i = (int)res.size() - 1; i >= 0; --i)
        {
            QString tourCountry = QString::fromStdString(res[i].getCountry());
            if (!tourCountry.contains(country, Qt::CaseInsensitive))
            {
                res.erase(res.begin() + i);
            }
        }
    }

    if (!priceStr.isEmpty())
    {
        double maxPrice = priceStr.toDouble();
        for (int i = (int)res.size() - 1; i >= 0; --i)
        {
            if (res[i].getCurrentPrice() > maxPrice)
            {
                res.erase(res.begin() + i);
            }
        }
    }

    toursTable->setRowCount((int)res.size());

    for (int i = 0; i < (int)res.size(); ++i)
    {
        Tour& t = res[i];
        Date start=t.getStartDate();
        Date end=t.getEndDate();
        int days=(end.year-start.year)*365+(end.month-start.month)*30+(end.day-start.day);
        if (days < 0) days = -days;
        toursTable->setItem(i, 0, new QTableWidgetItem(QString::number(t.getId())));
        toursTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(t.getTitle())));
        toursTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(t.getCountry())));
        toursTable->setItem(i, 3, new QTableWidgetItem(QString::number(days)));
        toursTable->setItem(i, 4, new QTableWidgetItem(QString::number(t.getCurrentPrice())));
        toursTable->setItem(i, 5, new QTableWidgetItem(QString::number(t.getAvailableSeats())));

        QString status = t.isHot() ? "🔥 Горящий" : "Обычный";
        QTableWidgetItem* statusItem = new QTableWidgetItem(status);
        if (t.isHot())
        {
            statusItem->setBackground(Qt::red);
            statusItem->setForeground(Qt::white);
        }
        toursTable->setItem(i, 6, statusItem);
    }

    toursTable->resizeColumnsToContents();

    QMessageBox::information(this, "Результат", QString("Найдено туров: %1").arg(res.size()));
}

void MainWindow::setupOrdersPage()
{
    QWidget* page3 = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page3);
    layout->setSpacing(15);
    QLabel* title = new QLabel("📋 МОИ ЗАКАЗЫ");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(10);
    ordersTable = new QTableWidget();
    ordersTable->setColumnCount(6);
    ordersTable->setHorizontalHeaderLabels({
        "ID", "Тур", "Кол-во", "Сумма (руб)", "Статус", "Дата"
    });
    ordersTable->horizontalHeader()->setStretchLastSection(true);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setAlternatingRowColors(true);
    layout->addWidget(ordersTable);

    layout->addSpacing(10);
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    newOrderBtn = new QPushButton(" Новый заказ");
    newOrderBtn->setFixedWidth(180);
    newOrderBtn->setStyleSheet("background-color: #2ecc71; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    btnLayout->addWidget(newOrderBtn);
    btnLayout->addSpacing(20);
    cancelOrderBtn = new QPushButton(" Отменить заказ");
    cancelOrderBtn->setFixedWidth(180);
    cancelOrderBtn->setStyleSheet("background-color: #e74c3c; color: white; font-size: 14px; padding: 10px; border-radius: 5px;");
    btnLayout->addWidget(cancelOrderBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);
    layout->addStretch();

    pages->addWidget(page3);
    connect(newOrderBtn, &QPushButton::clicked, this, &MainWindow::createNewOrder);
    connect(cancelOrderBtn, &QPushButton::clicked, this, &MainWindow::cancelOrder);
}

void MainWindow::createNewOrder()
{
    if (!isAuthorized)
    {
        QMessageBox::warning(this, "Ошибка", "Авторизуйтесь, чтобы создать заказ!");
        return;
    }

    vector<Tour>& tours = agency->getTours();
    if (tours.empty())
    {
        QMessageBox::warning(this, "Ошибка", "Нет доступных туров!");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Новый заказ");
    dialog.resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* title = new QLabel("🛒 НОВЫЙ ЗАКАЗ");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(10);

    QComboBox *tourCombo=new QComboBox();
    for(Tour &t:tours)
    {
        QString info = QString::fromStdString(t.getTitle()) + " (" + QString::fromStdString(t.getCountry()) +
                       ") - " + QString::number(t.getPrice()) + " руб. | Мест: " + QString::number(t.getAvailableSeats());
        tourCombo->addItem(info, t.getId());
    }
    layout->addWidget(tourCombo);

    QHBoxLayout* countLayout = new QHBoxLayout();
    countLayout->addWidget(new QLabel("Количество туристов:"));
    QSpinBox* countSpin=new QSpinBox;
    countSpin->setRange(1,10);
    countSpin->setValue(1);
    countLayout->addWidget(countSpin);
    countLayout->addStretch();
    layout->addLayout(countLayout);
    layout->addSpacing(10);

    QGroupBox* serviceGroup = new QGroupBox("Дополнительные услуги");
    QVBoxLayout* serviceLayout = new QVBoxLayout();

    vector<AdditionalService>& services = agency->getServices();
    QVector<QCheckBox*> serviceCheckboxes;

    for (int i = 0; i < (int)services.size() && i < 10; ++i) {
        QString text = QString::fromStdString(services[i].getName()) +" (+" + QString::number(services[i].getServicePrice()) + " руб.)";
        QCheckBox* cb = new QCheckBox(text);
        cb->setProperty("serviceIndex", i);
        serviceLayout->addWidget(cb);
        serviceCheckboxes.append(cb);
    }

    if (serviceCheckboxes.isEmpty()) {
        serviceLayout->addWidget(new QLabel("Нет доступных услуг"));
    }

    serviceGroup->setLayout(serviceLayout);
    layout->addWidget(serviceGroup);

    layout->addSpacing(10);

    QDialogButtonBox* buttons=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if(dialog.exec()==QDialog::Accepted)
    {
        int tourId = tourCombo->currentData().toInt();
        int touristcount=countSpin->value();

        int tourIndex = agency->findTourById(tourId);
        if (tourIndex == -1) {
            QMessageBox::warning(this, "Ошибка", "Тур не найден!");
            return;
        }

        double servicesTotal=0;
        for (QCheckBox* cb : serviceCheckboxes)
        {
            if (cb->isChecked())
            {
                int index = cb->property("serviceIndex").toInt();
                if (index < (int)services.size()) {
                    servicesTotal += services[index].getServicePrice() * touristcount;
                }
            }
        }

        Tour& tour = agency->getTours()[tourIndex];

        if(!tour.hasSeats(touristcount))
        {
            QMessageBox::warning(this, "Ошибка", "Недостаточно мест! Доступно: " + QString::number(tour.getAvailableSeats()));
            return;
        }

        Order newOrder;
        int newOrderId = agency->getNextOrderId();
        newOrder.create(
            newOrderId,
            agency->getCurrentClientId(),
            tourId,
            touristcount,
            tour.getPrice() * touristcount
            );
        newOrder.setServicesTotal(servicesTotal);
        newOrder.calculateTotal();
        tour.bookSeats(touristcount);
        agency->getOrders().push_back(newOrder);

        qDebug() << "=== СОЗДАНИЕ ЗАКАЗА ===";
        qDebug() << "Заказ ID:" << newOrderId;
        qDebug() << "Клиент ID:" << agency->getCurrentClientId();
        qDebug() << "Тур ID:" << tourId;
        qDebug() << "Кол-во туристов:" << touristcount;
        qDebug() << "Сумма:" << newOrder.getTotalPrice();
        qDebug() << "Всего заказов в памяти:" << agency->getOrders().size();

        agency->saveOrders();
        agency->saveTours();

        qDebug() << "=== ПЕРЕЗАГРУЗКА ДАННЫХ ===";
        agency->loadOrders();
        agency->loadTours();

        qDebug() << "Заказов после перезагрузки:" << agency->getOrders().size();

        loadOrdersTable();
        loadToursToTable();

        int realOrderId = newOrderId;
        for (Order& o : agency->getOrders()) {
            if (o.getClientId() == agency->getCurrentClientId() &&
                o.getTourId() == tourId &&
                o.getTouristsCount() == touristcount) {
                realOrderId = o.getId();
                break;
            }
        }

        QString message = "✅ Заказ №" + QString::number(realOrderId) + " создан!\n" +
                          "💰 Сумма: " + QString::number(newOrder.getTotalPrice()) + " руб.";
        if (servicesTotal > 0) {
            message += "\n📦 Доп. услуги: " + QString::number(servicesTotal) + " руб.";
        }
        QMessageBox::information(this, "Успех", message);
    }
}

void MainWindow::loadOrdersTable()
{
    if (!isAuthorized || !ordersTable)
    {
        if (ordersTable) ordersTable->setRowCount(0);
        return;
    }

    vector<Order>& orders = agency->getOrders();
    int clientId = agency->getCurrentClientId();

    qDebug() << "=== ЗАГРУЗКА ТАБЛИЦЫ ЗАКАЗОВ ===";
    qDebug() << "Всего заказов:" << orders.size();
    qDebug() << "ID клиента:" << clientId;

    int count = 0;
    for (Order& o : orders)
    {
        if (o.getClientId() == clientId)
        {
            count++;
            qDebug() << "Найден заказ ID:" << o.getId()
                     << "Тур:" << o.getTourId()
                     << "Сумма:" << o.getTotalPrice();
        }
    }

    qDebug() << "Заказов для пользователя:" << count;

    ordersTable->setRowCount(count);
    int row = 0;
    for (int i = 0; i < (int)orders.size(); ++i)
    {
        if (orders[i].getClientId() == clientId)
        {
            ordersTable->setItem(row, 0, new QTableWidgetItem(QString::number(orders[i].getId())));
            int tourIndex = agency->findTourById(orders[i].getTourId());
            QString tourName = (tourIndex != -1) ?
                                   QString::fromStdString(agency->getTours()[tourIndex].getTitle()) :
                                   "Тур №" + QString::number(orders[i].getTourId());
            ordersTable->setItem(row, 1, new QTableWidgetItem(tourName));

            ordersTable->setItem(row, 2, new QTableWidgetItem(QString::number(orders[i].getTouristsCount())));
            ordersTable->setItem(row, 3, new QTableWidgetItem(QString::number(orders[i].getTotalPrice())));

            QString status = QString::fromStdString(orders[i].getStatus());
            QTableWidgetItem* statusItem = new QTableWidgetItem(status);
            if (status == "Отменён") {
                statusItem->setBackground(Qt::red);
                statusItem->setForeground(Qt::white);
            } else if (status == "Подтверждён") {
                statusItem->setBackground(Qt::green);
                statusItem->setForeground(Qt::white);
            }
            ordersTable->setItem(row, 4, statusItem);

            ordersTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(orders[i].getOrderDate().toString())));
            row++;
        }
    }
    ordersTable->resizeColumnsToContents();
    qDebug() << "=== ТАБЛИЦА ОБНОВЛЕНА ===";
}

void MainWindow::cancelOrder()
{
    if (!isAuthorized)
    {
        QMessageBox::warning(this, "Ошибка", "Авторизуйтесь!");
        return;
    }

    vector<Order>& orders = agency->getOrders();
    int clientId = agency->getCurrentClientId();
    vector<Order> userOrders;
    for(int i=0;i<orders.size();++i)
    {
        if(orders[i].getClientId()==clientId)
        {
            userOrders.push_back(orders[i]);
        }
    }
    if (userOrders.empty()) {
        QMessageBox::warning(this, "Ошибка", "У вас нет заказов!");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Отмена заказа");
    dialog.resize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* title = new QLabel(" ВЫБЕРИТЕ ЗАКАЗ ДЛЯ ОТМЕНЫ");
    title->setStyleSheet("font-size: 16px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    layout->addSpacing(10);

    QComboBox *orderCombo=new QComboBox;
    for(int i=0;i<userOrders.size();i++)
    {
        QString status = QString::fromStdString(userOrders[i].getStatus());
        QString info = "Заказ №" + QString::number(userOrders[i].getId()) +
                       " | Сумма: " + QString::number(userOrders[i].getTotalPrice()) + " руб." +" | Статус: " + status;

        orderCombo->addItem(info, userOrders[i].getId());
    }

    layout->addWidget(orderCombo);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* cancelBtn = new QPushButton(" Отменить заказ");
    cancelBtn->setStyleSheet("background-color: #e74c3c; color: white; padding: 8px; border-radius: 5px;");
    QPushButton* closeBtn = new QPushButton(" Закрыть");
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);

    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(cancelBtn, &QPushButton::clicked, &dialog, [&]() {
        int orderId = orderCombo->currentData().toInt();

        int orderIndex = -1;
        for (int i = 0; i < (int)orders.size(); ++i) {
            if (orders[i].getId() == orderId) {
                orderIndex = i;
                break;
            }
        }

        if (orderIndex == -1) {
            QMessageBox::warning(this, "Ошибка", "Заказ не найден!");
            dialog.reject();
            return;
        }
        if (orders[orderIndex].getStatus() == "Отменён") {
            QMessageBox::warning(this, "Ошибка", "Этот заказ уже отменён!");
            return;
        }

        int tourId = orders[orderIndex].getTourId();
        int tourIndex = agency->findTourById(tourId);
        if (tourIndex != -1) {
            int count = orders[orderIndex].getTouristsCount();
            agency->getTours()[tourIndex].cancelSeats(count);
            agency->saveTours();
        }

        orders[orderIndex].setStatus("Отменён");
        agency->saveOrders();


        agency->loadOrders();
        agency->loadTours();

        loadOrdersTable();

        QMessageBox::information(this, "Успех", "Заказ №" + QString::number(orderId) + " отменён!");
        dialog.accept();
    });

    dialog.exec();
}

void MainWindow::setupServicesPage()
{
    QWidget* page4 = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page4);
    layout->setSpacing(15);

    QLabel* title = new QLabel("🎯 ДОПОЛНИТЕЛЬНЫЕ УСЛУГИ");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    layout->addWidget(title);

    layout->addSpacing(10);

    servicesTable = new QTableWidget();
    servicesTable->setColumnCount(4);
    servicesTable->setHorizontalHeaderLabels({
        "ID", "Название", "Цена (руб)", "Описание"
    });
    servicesTable->horizontalHeader()->setStretchLastSection(true);
    servicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    servicesTable->setAlternatingRowColors(true);
    layout->addWidget(servicesTable);

    layout->addStretch();

    pages->addWidget(page4);

    loadServicesTable();
}

void MainWindow::loadServicesTable()
{
    vector<AdditionalService>& services = agency->getServices();
    servicesTable->setRowCount((int)services.size());

    for (int i = 0; i < (int)services.size(); ++i) {
        AdditionalService& s = services[i];

        servicesTable->setItem(i, 0, new QTableWidgetItem(QString::number(s.getId())));
        servicesTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(s.getName())));
        servicesTable->setItem(i, 2, new QTableWidgetItem(QString::number(s.getServicePrice())));
        servicesTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(s.getDescription())));
    }

    servicesTable->resizeColumnsToContents();
}