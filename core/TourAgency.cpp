// core/TourAgency.cpp
#include "TourAgency.h"
#include <QSqlQuery>
#include <QSqlError>
int TourAgency::findClientById(int id) {
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i].getId() == id)
            return i;
    }
    return -1;
}

int TourAgency::findClientByPhone(const string& phone)
{

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << " База данных не открыта!";
        return -1;
    }

    QSqlQuery query;

    query.prepare("SELECT id FROM clients WHERE phone = :phone");
    query.bindValue(":phone", QString::fromStdString(phone));

    if (!query.exec()) {
        qDebug() << " Ошибка поиска клиента:" << query.lastError().text();
        return -1;
    }

    if (!query.next()) {
        qDebug() << " Клиент с телефоном" << phone.c_str() << "не найден в БД!";
        return -1;
    }

    int id = query.value("id").toInt();
    qDebug() << " В БД найден ID:" << id;

    for (int i = 0; i < (int)clients.size(); ++i) {
        if (clients[i].getId() == id) {
            qDebug() << " Клиент найден в векторе, индекс:" << i;
            return i;
        }
    }

    qDebug() << " Клиент с ID" << id << "не найден в векторе clients!";
    qDebug() << "Размер вектора clients:" << clients.size();

    return -1;
}
int TourAgency::findTourById(int id) {
    for (int i = 0; i < (int)tours.size(); i++) {
        if (tours[i].getId() == id) return i;
    }
    return -1;
}

int TourAgency::findServiceById(int id) {
    for (int i = 0; i < (int)services.size(); i++) {
        if (services[i].getId() == id) return i;
    }
    return -1;
}

bool TourAgency::hasClientOrders(int clientId) {
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getClientId() == clientId) {
            return true;
        }
    }
    return false;
}

int TourAgency::getNextClientId() {
    int maxid = 0;
    for (int i = 0; i < (int)clients.size(); i++) {
        if (clients[i].getId() > maxid) maxid = clients[i].getId();
    }
    return ++maxid;
}

int TourAgency::getNextOrderId() {
    int maxid = 0;
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getId() > maxid) maxid = orders[i].getId();
    }
    return ++maxid;
}



void TourAgency::loadClients()
{
    QSqlQuery query;
    clients.clear();
    if (query.exec("SELECT * FROM public.clients")) {
        while (query.next()) {
                 Client c;
                 c.setId(query.value("id").toInt());
                 c.setSurname(query.value("surname").toString().toStdString());
                 c.setName(query.value("name").toString().toStdString());
                 c.setPatronymic(query.value("patronymic").toString().toStdString());
                 c.setPhone(query.value("phone").toString().toStdString());
                 c.setEmail(query.value("email").toString().toStdString());
                 c.setPassport(query.value("passport").toString().toStdString());
                 clients.push_back(c);
        }
    } else {
        qDebug() << " Ошибка загрузки клиентов:" << query.lastError().text();
    }

    query.clear();
}

void TourAgency::saveClients()
{
    for (Client& c : clients) {
        QSqlQuery check;
        QString checkSql = "SELECT id FROM clients WHERE id = " + QString::number(c.getId());

        if (!check.exec(checkSql)) {
            qDebug() << " Ошибка проверки клиента:" << check.lastError().text();
            continue;
        }

        if (check.next()) {
            QSqlQuery update;
            QString updateSql = QString(
                                    "UPDATE clients SET "
                                    "surname = '%1', "
                                    "name = '%2', "
                                    "patronymic = '%3', "
                                    "phone = '%4', "
                                    "email = '%5', "
                                    "passport = '%6' "
                                    "WHERE id = %7"
                                    ).arg(QString::fromStdString(c.getSurname()))
                                    .arg(QString::fromStdString(c.getName()))
                                    .arg(QString::fromStdString(c.getPatronymic()))
                                    .arg(QString::fromStdString(c.getPhone()))
                                    .arg(QString::fromStdString(c.getEmail()))
                                    .arg(QString::fromStdString(c.getPassport()))
                                    .arg(c.getId());

            if (!update.exec(updateSql)) {
                qDebug() << " Ошибка обновления клиента:" << update.lastError().text();
            }
        } else {
            QSqlQuery insert;
            QString insertSql = QString(
                                    "INSERT INTO clients (surname, name, patronymic, phone, email, passport) "
                                    "VALUES ('%1', '%2', '%3', '%4', '%5', '%6')"
                                    ).arg(QString::fromStdString(c.getSurname()))
                                    .arg(QString::fromStdString(c.getName()))
                                    .arg(QString::fromStdString(c.getPatronymic()))
                                    .arg(QString::fromStdString(c.getPhone()))
                                    .arg(QString::fromStdString(c.getEmail()))
                                    .arg(QString::fromStdString(c.getPassport()));

            if (!insert.exec(insertSql)) {
                qDebug() << " Ошибка вставки клиента:" << insert.lastError().text();
            } else {
                int newId = insert.lastInsertId().toInt();
                c.setId(newId);
            }
        }
    }
    qDebug() << " Клиенты сохранены в БД";
}

void TourAgency::loadTours()
{

    QSqlQuery query;
     tours.clear();
    if (query.exec("SELECT * FROM public.tours")) {

    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString country = query.value("country").toString();
        QString startDate = query.value("start_date").toString();
        QString endDate = query.value("end_date").toString();
        double price = query.value("price").toDouble();
        int seats = query.value("available_seats").toInt();

        Tour t(
            id,
            title.toStdString(),
            country.toStdString(),
            startDate.toStdString(),
            endDate.toStdString(),
            price,
            seats
            );
        tours.push_back(t);
    }
    } else
    {
        qDebug()<<"Ошибка загрузки туров "<<query.lastError().text();
    }
    query.clear();

}


void TourAgency::saveTours()
{
    for (Tour& t : tours) {
        QSqlQuery check;
        QString checkSql = "SELECT id FROM tours WHERE id = " + QString::number(t.getId());

        if (!check.exec(checkSql)) {
            qDebug() << " Ошибка проверки тура:" << check.lastError().text();
            continue;
        }

        if (check.next()) {
            QSqlQuery update;
            QString updateSql = QString(
                                    "UPDATE tours SET "
                                    "title = '%1', "
                                    "country = '%2', "
                                    "start_date = '%3', "
                                    "end_date = '%4', "
                                    "price = %5, "
                                    "available_seats = %6 "
                                    "WHERE id = %7"
                                    ).arg(QString::fromStdString(t.getTitle()))
                                    .arg(QString::fromStdString(t.getCountry()))
                                    .arg(QString::fromStdString(t.getStartDate().toString()))
                                    .arg(QString::fromStdString(t.getEndDate().toString()))
                                    .arg(t.getPrice())
                                    .arg(t.getAvailableSeats())
                                    .arg(t.getId());

            if (!update.exec(updateSql)) {
                qDebug() << " Ошибка обновления тура:" << update.lastError().text();
            }
        } else {
            QSqlQuery insert;
            QString insertSql = QString(
                                    "INSERT INTO tours (title, country, start_date, end_date, price, available_seats) "
                                    "VALUES ('%1', '%2', '%3', '%4', %5, %6)"
                                    ).arg(QString::fromStdString(t.getTitle()))
                                    .arg(QString::fromStdString(t.getCountry()))
                                    .arg(QString::fromStdString(t.getStartDate().toString()))
                                    .arg(QString::fromStdString(t.getEndDate().toString()))
                                    .arg(t.getPrice())
                                    .arg(t.getAvailableSeats());

            if (!insert.exec(insertSql)) {
                qDebug() << " Ошибка вставки тура:" << insert.lastError().text();
            } else {
                int newId = insert.lastInsertId().toInt();
                t.setId(newId);
            }
        }
    }
    qDebug() << " Туры сохранены в БД";
}

void TourAgency::loadServices()
{
    QSqlQuery query;
    services.clear();
    if(query.exec("SELECT * FROM public.services"))
    {
    while (query.next()) {
        AdditionalService s;
        s.setId(query.value("id").toInt());
        s.setName(query.value("name").toString().toStdString());
        s.setServicePrice(query.value("price").toDouble());
        s.setDescription(query.value("description").toString().toStdString());
        services.push_back(s);
    }

    qDebug() << " Загружено услуг:" << services.size();
    }
else {
        qDebug()<<"Ошибка загрузки услуг: "<< query.lastError().text();
}
    query.clear();

}
void TourAgency::saveServices()
{
    for (AdditionalService& s : services) {
        QSqlQuery check;
        QString checkSql = "SELECT id FROM services WHERE id = " + QString::number(s.getId());

        if (!check.exec(checkSql)) {
            qDebug() << " Ошибка проверки услуги:" << check.lastError().text();
            continue;
        }

        if (check.next()) {
            QSqlQuery update;
            QString updateSql = QString(
                                    "UPDATE services SET "
                                    "name = '%1', "
                                    "price = %2, "
                                    "description = '%3' "
                                    "WHERE id = %4"
                                    ).arg(QString::fromStdString(s.getName()))
                                    .arg(s.getServicePrice())
                                    .arg(QString::fromStdString(s.getDescription()))
                                    .arg(s.getId());

            if (!update.exec(updateSql)) {
                qDebug() << " Ошибка обновления услуги:" << update.lastError().text();
            }
        } else {
            QSqlQuery insert;
            QString insertSql = QString(
                                    "INSERT INTO services (name, price, description) "
                                    "VALUES ('%1', %2, '%3')"
                                    ).arg(QString::fromStdString(s.getName()))
                                    .arg(s.getServicePrice())
                                    .arg(QString::fromStdString(s.getDescription()));

            if (!insert.exec(insertSql)) {
                qDebug() << " Ошибка вставки услуги:" << insert.lastError().text();
            } else {
                int newId = insert.lastInsertId().toInt();
                s.setId(newId);
            }
        }
    }
    qDebug() << " Услуги сохранены в БД";
}

void TourAgency::loadOrders()
{
    QSqlQuery query;
    orders.clear();

    qDebug() << "=== ЗАГРУЗКА ЗАКАЗОВ ИЗ БД ===";

    if (query.exec("SELECT * FROM public.orders")) {
        int count = 0;
        while (query.next()) {
            Order o;
            o.setId(query.value("id").toInt());
            o.setClientId(query.value("client_id").toInt());
            o.setTourId(query.value("tour_id").toInt());
            o.setTouristsCount(query.value("tourists_count").toInt());
            o.setTotalPrice(query.value("total_price").toDouble());
            o.setStatus(query.value("status").toString().toStdString());

            QString dateStr = query.value("order_date").toString();
            Date d;
            d.fromString(dateStr.toStdString());
            o.setOrderDate(d);

            orders.push_back(o);
            count++;
            qDebug() << "Загружен заказ ID:" << o.getId()
                     << "Клиент:" << o.getClientId()
                     << "Сумма:" << o.getTotalPrice();
        }
        qDebug() << " Загружено заказов:" << count;
    } else {
        qDebug() << " Ошибка загрузки заказов:" << query.lastError().text();
    }
    query.clear();
}
void TourAgency::saveOrders()
{
    QSqlQuery query;

    // Стартуем транзакцию
    QSqlDatabase::database().transaction();

    for (Order& o : orders) {
        query.clear();

        // Явно указываем ВСЕ столбцы, включая tour_price, строго по порядку
        query.prepare(
            "INSERT INTO orders (id, client_id, tour_id, tourists_count, tour_price, total_price, status, order_date) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?) "
            "ON CONFLICT (id) DO UPDATE SET "
            "client_id = EXCLUDED.client_id, "
            "tour_id = EXCLUDED.tour_id, "
            "tourists_count = EXCLUDED.tourists_count, "
            "tour_price = EXCLUDED.tour_price, "
            "total_price = EXCLUDED.total_price, "
            "status = EXCLUDED.status, "
            "order_date = EXCLUDED.order_date"
            );

        // Передаем значения строго в соответствии с вопросительными знаками выше:
        query.addBindValue(o.getId());                         // id
        query.addBindValue(o.getClientId());                   // client_id
        query.addBindValue(o.getTourId());                     // tour_id
        query.addBindValue(o.getTouristsCount());              // tourists_count
        query.addBindValue(0.00);                              // tour_price (заглушка 0.00 вместо NULL)
        query.addBindValue(o.getTotalPrice());                 // total_price
        query.addBindValue(QString::fromStdString(o.getStatus())); // status
        query.addBindValue(QString::fromStdString(o.getOrderDate().toString())); // order_date

        if (!query.exec()) {
            qDebug() << "❌ Ошибка сохранения заказа ID" << o.getId() << ":" << query.lastError().text();
            QSqlDatabase::database().rollback(); // Откатываем транзакцию при ошибке
            return;
        }
    }

    // Если все заказы успешно обработаны — сохраняем изменения в БД
    QSqlDatabase::database().commit();
    query.clear();

    qDebug() << "✅ Синхронизация заказов с БД завершена. Обработано:" << orders.size();
}
void TourAgency::loadAllData()
{
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << " Нет подключения к БД!";
        return;
    }
    QSqlQuery query;
    loadClients();
    loadTours();
    loadServices();
    loadOrders();
}

void TourAgency::saveAllData() {
    saveClients();
    saveTours();
    saveServices();
    saveOrders();
    cout << "\n=== ДАННЫЕ СОХРАНЕНЫ ===\n";
}


void TourAgency::addClient() {
    int newId = getNextClientId();
    Client newClient;
    newClient.setId(newId);

    string surname, name, patronymic, phone, email, passport;
    cout << "Введите ваши данные " << endl;
    cout << "Фамилия: "; cin >> surname;
    cout << "Имя: "; cin >> name;
    cout << "Отчество: "; cin >> patronymic;
    cout << "Мобильный телефон: "; cin >> phone;
    cout << "Email: "; cin >> email;
    cout << "Паспортные данные: "; cin >> passport;

    newClient.setSurname(surname);
    newClient.setName(name);
    newClient.setPatronymic(patronymic);
    newClient.setPhone(phone);
    newClient.setEmail(email);
    newClient.setPassport(passport);

    clients.push_back(newClient);
    saveClients();
    cout << "Клиент добавлен! ID: " << newId << endl;
}

void TourAgency::editClient() {
    if (clients.empty()) {
        cout << "Нет клиентов для редактирования\n";
        return;
    }

    cout << "\n=== РЕДАКТИРОВАНИЕ КЛИЕНТА ===\n";
    cout << "Искать по:\n";
    cout << "1. Номеру телефона\n";
    cout << "2. ФИО\n";
    cout << "Выбор: ";

    int searchChoice;
    cin >> searchChoice;
    int clientIndex = -1;

    if (searchChoice == 1) {
        string phone;
        cout << "Введите номер телефона: ";
        cin >> phone;
        clientIndex = findClientByPhone(phone);
        if (clientIndex == -1) {
            cout << "Клиент с телефоном " << phone << " не найден\n";
            return;
        }
    }
    else if (searchChoice == 2) {
        string surname, name, patronymic;
        cout << "Введите фамилию: ";
        cin >> surname;
        cout << "Введите имя: ";
        cin >> name;
        cout << "Введите отчество: ";
        cin >> patronymic;
        string fullName = surname + " " + name + " " + patronymic;
        clientIndex = findClientByPhone(fullName);
        if (clientIndex == -1) {
            cout << "Клиент с ФИО \"" << fullName << "\" не найден\n";
            return;
        }
    }
    else {
        cout << "Неверный выбор\n";
        return;
    }

    cout << "\nНайден клиент:\n";
    clients[clientIndex].print();

    int choice;
    cout << "1. Фамилия\n2. Имя\n3. Отчество\n4. Телефон\n5. Email\n6. Паспорт\n";
    cout << "Что редактировать? "; cin >> choice;

    string newValue;
    switch (choice) {
    case 1: cout << "Новая фамилия: "; cin >> newValue; clients[clientIndex].setSurname(newValue); break;
    case 2: cout << "Новое имя: "; cin >> newValue; clients[clientIndex].setName(newValue); break;
    case 3: cout << "Новое отчество: "; cin >> newValue; clients[clientIndex].setPatronymic(newValue); break;
    case 4: cout << "Новый телефон: "; cin >> newValue; clients[clientIndex].setPhone(newValue); break;
    case 5: cout << "Новый Email: "; cin >> newValue; clients[clientIndex].setEmail(newValue); break;
    case 6: cout << "Новый паспорт: "; cin >> newValue; clients[clientIndex].setPassport(newValue); break;
    default: cout << "Неверный выбор\n";
    }

    saveClients();
    cout << "Клиент успешно отредактирован!\n";
}

void TourAgency::createOrder() {
    if (currentClientId == -1) {
        cout << "Ошибка: вы не авторизованы!\n";
        return;
    }

    if (tours.empty()) {
        cout << "Туров пока нет!\n";
        return;
    }

    cout << "\n=== ОФОРМЛЕНИЕ ЗАКАЗА ===\n";
    listTours();

    int tourId;
    cout << "Введите ID тура: ";
    cin >> tourId;

    int tourIndex = findTourById(tourId);
    if (tourIndex == -1) {
        cout << "Тур не найден!\n";
        return;
    }

    int touristsCount;
    cout << "Введите количество туристов: ";
    cin >> touristsCount;

    if (!tours[tourIndex].hasSeats(touristsCount)) {
        cout << "Недостаточно мест! Доступно: " << tours[tourIndex].getAvailableSeats() << endl;
        return;
    }

    int newOrderId = getNextOrderId();
    Order newOrder;
    newOrder.create(newOrderId, currentClientId, tourId, touristsCount, tours[tourIndex].getPrice());

    tours[tourIndex].bookSeats(touristsCount);
    orders.push_back(newOrder);
    saveOrders();
    saveTours();

    cout << "\n ЗАКАЗ УСПЕШНО ОФОРМЛЕН!\n";
    cout << "Номер заказа: " << newOrderId << "\n";
    cout << "Сумма: " << newOrder.getTotalPrice() << " руб.\n";
    cout << "Статус: " << newOrder.getStatus() << "\n";
}

void TourAgency::cancelOrder() {
    if (currentClientId == -1) {
        cout << "Ошибка: вы не авторизованы!\n";
        return;
    }

    cout << "\n=== ВАШИ ЗАКАЗЫ ===\n";
    bool found = false;
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getClientId() == currentClientId && orders[i].getStatus() == "Подтверждён") {
            orders[i].print();
            found = true;
        }
    }

    if (!found) {
        cout << "Нет активных заказов.\n";
        return;
    }

    int orderId;
    cout << "\nВведите ID заказа для отмены: ";
    cin >> orderId;

    int orderIndex = -1;
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getId() == orderId && orders[i].getClientId() == currentClientId) {
            orderIndex = i;
            break;
        }
    }

    if (orderIndex == -1) {
        cout << "Заказ не найден!\n";
        return;
    }

    int tourIndex = findTourById(orders[orderIndex].getTourId());
    if (tourIndex != -1) {
        tours[tourIndex].cancelSeats(orders[orderIndex].getTouristsCount());
        saveTours();
    }

    orders[orderIndex].setStatus("Отменён");
    saveOrders();
    cout << " Заказ отменён!\n";
}

void TourAgency::listOrders() {
    if (currentClientId == -1) {
        cout << "Ошибка: вы не авторизованы!\n";
        return;
    }

    bool found = false;
    cout << "\n=== ВАШИ ЗАКАЗЫ ===\n";
    for (int i = 0; i < (int)orders.size(); i++) {
        if (orders[i].getClientId() == currentClientId) {
            orders[i].print();
            found = true;
        }
    }

    if (!found) {
        cout << "У вас нет заказов.\n";
    }
}

void TourAgency::listTours() {
    cout << "\n=== СПИСОК ТУРОВ ===\n";
    for (int i = 0; i < (int)tours.size(); i++) {
        tours[i].print();
    }
}

void TourAgency::listHotTours() {
    bool found = false;
    cout << "\n=== ГОРЯЩИЕ ТУРЫ ===\n";
    for (int i = 0; i < (int)tours.size(); i++) {
        if (tours[i].isHot()) {
            tours[i].print();
            found = true;
        }
    }
    if (!found) cout << "Горящих туров нет.\n";
}

void TourAgency::filterToursByCountry() {
    string country;
    bool found = false;
    cout << "Введите страну: ";
    cin.ignore();
    getline(cin, country);

    for (int i = 0; i < (int)tours.size(); i++) {
        if (tours[i].getCountry() == country) {
            tours[i].print();
            found = true;
        }
    }
    if (!found) {
        cout << "Туров в " << country << " не найдено.\n";
    }
}

void TourAgency::filterToursByMaxPrice() {
    double maxPrice;
    cout << "Введите максимальную цену: ";
    cin >> maxPrice;

    bool found = false;
    for (int i = 0; i < (int)tours.size(); i++) {
        if (tours[i].getPrice() <= maxPrice) {
            tours[i].print();
            found = true;
        }
    }

    if (!found) {
        cout << "Туров дешевле " << maxPrice << " руб. не найдено.\n";
    }
}

void TourAgency::listServices() {
    cout << "\n=== ДОПОЛНИТЕЛЬНЫЕ УСЛУГИ ===\n";
    for (int i = 0; i < (int)services.size(); i++) {
        services[i].print();
    }
}

double TourAgency::choseAddServices() {
    if (services.empty()) {
        cout << "Дополнительных услуг пока нет.\n";
        return 0;
    }

    listServices();
    double total = 0;
    int choice;

    while (true) {
        cout << "Введите ID услуги (0 - закончить): ";
        cin >> choice;

        if (choice == 0) break;

        int serviceIndex = findServiceById(choice);
        if (serviceIndex == -1) {
            cout << "Услуга не найдена!\n";
            continue;
        }

        cout << "Услуга \"" << services[serviceIndex].getName() << "\" добавлена!\n";
        total += services[serviceIndex].getServicePrice();
        tempnameserv.push_back(services[serviceIndex].getName());
    }

    if (total > 0) {
        cout << "\nОбщая стоимость услуг: " << total << " руб.\n";
    }

    return total;
}

void TourAgency::printReceipt(int orderId) {
    cout << "\n=== ЧЕК №" << orderId << " ===\n";
    cout << "Спасибо за покупку!\n";
}

void TourAgency::showToursWithFilter() {
    if (tours.empty()) {
        cout << "Извините, туров пока нет.\n";
        return;
    }

    int choice;
    do {
        cout << "\n=== ПОИСК ТУРОВ ===\n";
        cout << "1. Показать все туры\n";
        cout << "2. Поиск по стране\n";
        cout << "3. Поиск по цене\n";
        cout << "4. Только горящие туры\n";
        cout << "0. Назад\n";
        cout << "Выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: listTours(); break;
        case 2: filterToursByCountry(); break;
        case 3: filterToursByMaxPrice(); break;
        case 4: listHotTours(); break;
        case 0: cout << "Возврат...\n"; break;
        default: cout << "Неверный выбор!\n";
        }
    } while (choice != 0);
}

void TourAgency::run() {
    loadAllData();

    // Упрощенная версия для Qt
    cout << "Туристическое агентство запущено!\n";
}