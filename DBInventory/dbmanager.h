#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "DBInventory_global.h"

#include <QObject>

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QString>


class DBINVENTORY_EXPORT DBManager : public QObject
{
    Q_OBJECT

private:
    explicit DBManager(QObject* parent = nullptr);
    ~DBManager();

    Q_DISABLE_COPY(DBManager)
    static DBManager* mPtr;

public:
    static DBManager* instance();

    void init();
    void close();


public:

    /*
     * Добавляет новый тип предмета в таблицу предметов
    */
    void addItemToItems(const DBTypes::ITEM_OBJECT& item);

    /*
     * Получить все предметы из таблицы предметов
    */
    QHash<DBTypes::ItemType, DBTypes::ITEM_OBJECT> getItems() const;

    /*
     * Получить конкретный предмет из таблицы предметов
    */
    DBTypes::ITEM_OBJECT getItem(const DBTypes::ItemType& type) const;

    /*
     * Добавляет предмет в инвентарь
    */
    void addItemToInventory(const QString& item, const DBTypes::ItemType& type, const QString& pos);

    /*
     * Возвращает все предметы в инвенторе
     * @Key - Name (Наименование предмета)
     * @Value - Count (Количество)
    */
    DBTypes::HASH_ITEMS getInventoryItems() const;

    /*
     * Возварщает количество предмета по названию
     * @param const QString& name_item
    */
    uint getCountInventoryItem(const QString& item_name, const QString& pos) const;

    /*
     * Увеличить количество
    */
    void addCountInventoryItem(const DBTypes::ItemType& type, const QString& item_name, const int& count, const QString& pos);

    /*
     * Уменьшить количество
     */
    void subCountInventoryItem(const QString& item_name, const QString& pos);

    /*
     * Переместить предмет в другую ячейку
    */
    void moveItemInventory(const DBTypes::ItemType& type, const QString& item_name,
                           const int& count, const QString& pos_from, const QString& pos_to);

    /*
     * Удалить таблицу инвенторя из БД
    */
    void clearTableInventory();

    /*
     * Удалить запись по имени и по позиции из таблицы инвенторя
    */
    void removeItem(const QString& item_name, const QString& item_pos);

signals:
    void error(const QString&) const;


private:
    QSqlDatabase m_db;

};

#define dbManager() DBManager::instance()

#endif // DBMANAGER_H
