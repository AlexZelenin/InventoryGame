#include "dbmanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMap>

#include <QDebug>

DBManager* DBManager::mPtr = 0;

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{

}

DBManager::~DBManager()
{

}

DBManager* DBManager::instance()
{
    if (!mPtr)
        mPtr = new DBManager();
    return mPtr;
}

void DBManager::init()
{
    m_db = QSqlDatabase::addDatabase(DBTypes::dbDriver);
    m_db.setDatabaseName(DBTypes::dbName);

    if (!m_db.open()) {
        qDebug() << m_db.lastError().text();
        emit error(m_db.lastError().text());
    }

    m_db.exec("PRAGMA synchronous = OFF");
    m_db.exec("PRAGMA journal_mode = MEMORY");
    m_db.exec("PRAGMA temp_store = MEMORY");

    const QString& q_items = "CREATE TABLE IF NOT EXISTS items("
                "item_id INTEGER PRIMARY KEY, "
                "item_name TEXT NOT NULL, "
                "item_image TEXT, "
                "item_sound TEXT "
                ");";

    const QString& q_item_inventory = "CREATE TABLE IF NOT EXISTS items_inventory("
                "item_id INTEGER, "
                "item_name TEXT NOT NULL, "
                "item_count INTEGER NOT NULL, "
                "item_pos TEXT UNIQUE "
                ");";

    QSqlQuery query(m_db);

    if (!query.exec(q_items)) {
        qDebug() << Q_FUNC_INFO << "Error: " << query.lastError().text();
        emit error(query.lastError().text());
    }

    if (!query.exec(q_item_inventory)) {
        qDebug() << Q_FUNC_INFO << "Error: " << m_db.lastError().text();
        emit error(query.lastError().text());
    }

    QMultiHash<DBTypes::ItemType, QPair<QString, QString> > items_image;
    items_image.insert(DBTypes::Apple, {"Apple", ":/images/Apple.png"});
    items_image.insert(DBTypes::Cherry, {"Cherry", ":/images/Cherry.png"});
    items_image.insert(DBTypes::Kiwi, {"Kiwi", ":/images/Kiwi.png"});
    items_image.insert(DBTypes::Strawberry, {"Strawberry", ":/images/Strawberry.png"});

    foreach(const DBTypes::ItemType& key, items_image.uniqueKeys()) {
        DBTypes::ITEM_OBJECT item;
        item.item_id = key;
        item.item_name = items_image.value(key).first;
        item.item_image = items_image.value(key).second;
        item.item_sound = "qrc:/sounds/apple1.mp3";
        addItemToItems(item);
    }
}

void DBManager::close()
{
    if (m_db.isOpen())
        m_db.close();
}

void DBManager::addItemToItems(const DBTypes::ITEM_OBJECT &item)
{
    QSqlQuery query(m_db);

    query.prepare("INSERT OR IGNORE INTO items (item_id, item_name, item_image, item_sound) "
                                        "VALUES (:item_id, :item_name, :item_image, :item_sound)");

    query.bindValue(":item_id", item.item_id);
    query.bindValue(":item_name", item.item_name);
    query.bindValue(":item_image", item.item_image);
    query.bindValue(":item_sound", item.item_sound);

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
    }
}

QHash<DBTypes::ItemType, DBTypes::ITEM_OBJECT> DBManager::getItems() const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM items");

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        return {};
    }

    QHash<DBTypes::ItemType, DBTypes::ITEM_OBJECT> listItems;

    int id_item_id = query.record().indexOf("item_id");
    int id_item_name = query.record().indexOf("item_name");
    int id_item_image = query.record().indexOf("item_image");
    int id_item_sound = query.record().indexOf("item_sound");

    while (query.next()) {
        DBTypes::ITEM_OBJECT item;
        item.item_id = query.value(id_item_id).value<DBTypes::ItemType>();
        item.item_name = query.value(id_item_name).toString();
        item.item_image = query.value(id_item_image).toString();
        item.item_sound = query.value(id_item_sound).toString();

        listItems.insert(item.item_id, item);
    }
    return listItems;
}

DBTypes::ITEM_OBJECT DBManager::getItem(const DBTypes::ItemType &type) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM items WHERE item_id = (:item_id)");

    query.bindValue(":item_id", type);

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << ": Error" << query.lastError().text();
        return {};
    }

    int id_item_id = query.record().indexOf("item_id");
    int id_item_name = query.record().indexOf("item_name");
    int id_item_image = query.record().indexOf("item_image");
    int id_item_sound = query.record().indexOf("item_sound");

    DBTypes::ITEM_OBJECT item;

    while (query.next()) {
        item.item_id = query.value(id_item_id).value<DBTypes::ItemType>();
        item.item_name = query.value(id_item_name).toString();
        item.item_image = query.value(id_item_image).toString();
        item.item_sound = query.value(id_item_sound).toString();
    }

    return item;
}

void DBManager::addItemToInventory(const QString &item, const DBTypes::ItemType& type, const QString& pos)
{
    QSqlQuery query(m_db);

    query.prepare("INSERT OR IGNORE INTO items_inventory (item_id, item_name, item_count, item_pos) "
                                        "VALUES (:item_id, :item_name, :item_count, :item_pos)");

    query.bindValue(":item_id", type);
    query.bindValue(":item_name", item);
    query.bindValue(":item_count", "1");
    query.bindValue(":item_pos", pos);

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
    }
}

DBTypes::HASH_ITEMS DBManager::getInventoryItems() const
{
    QSqlQuery query(m_db);

    query.prepare("SELECT * FROM items_inventory");

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        return {};
    }

    DBTypes::HASH_ITEMS hashItems;
    while(query.next()) {
        hashItems.insert(query.value(1).toString(), query.value(2).toString());
    }
    return hashItems;
}

uint DBManager::getCountInventoryItem(const QString &item_name, const QString& pos) const
{
    if (item_name.isEmpty())
        return 0;

    QSqlQuery query(m_db);

    query.prepare("SELECT item_count FROM items_inventory WHERE item_name = (:item_name) AND item_pos = (:item_pos)");
    query.bindValue(":item_name", item_name);
    query.bindValue(":item_pos", pos);

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        return {};
    }

    if (query.first()) {
        return query.value(0).toUInt();
    }
    return 0;
}

void DBManager::addCountInventoryItem(const DBTypes::ItemType& type, const QString& item_name, const int &count, const QString& pos)
{
    QSqlQuery query(m_db);

    query.prepare("INSERT INTO items_inventory (item_id, item_name, item_count, item_pos) VALUES (:item_id, :item_name, :item_count, :item_pos) "
                                           "ON CONFLICT(item_pos) DO UPDATE SET "
                                           "item_pos = EXCLUDED.item_pos, "
                                           "item_count = item_count + EXCLUDED.item_count "
                                           "WHERE item_name = (:item_name) AND item_id = (:item_id) "
                                           );

    query.bindValue(":item_id", type);
    query.bindValue(":item_name", item_name);
    query.bindValue(":item_count", count);
    query.bindValue(":item_pos", pos);


    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
        emit error(query.lastError().text());
    }
}

void DBManager::subCountInventoryItem(const QString& item_name, const QString& pos)
{
    QSqlQuery query(m_db);

    query.prepare("UPDATE items_inventory SET item_count = item_count - (:count) WHERE item_name = (:item_name) AND item_pos = (:item_pos)");
    query.bindValue(":count", "1");
    query.bindValue(":item_pos", pos);
    query.bindValue(":item_name", item_name);

    if (!query.exec()) {
        emit error(query.lastError().text());
        qDebug() << Q_FUNC_INFO << "Error" << query.lastError().text();
    }
}

void DBManager::moveItemInventory(const DBTypes::ItemType& type, const QString &item_name,
                                  const int &count, const QString& pos_from, const QString &pos_to)
{
    QSqlQuery query(m_db);

    DBManager::addCountInventoryItem(type, item_name, count, pos_to);

    query.prepare("DELETE FROM items_inventory WHERE item_name = (:item_name) AND item_pos = (:item_pos)");
    query.bindValue(":item_name", item_name);
    query.bindValue(":item_pos", pos_from);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        emit error(query.lastError().text());
        return;
    }
}

void DBManager::clearTableInventory()
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM items_inventory");

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        emit error(query.lastError().text());
    }
}

void DBManager::removeItem(const QString &item_name, const QString &item_pos)
{
    QSqlQuery query(m_db);

    query.prepare("DELETE FROM items_inventory WHERE item_name = (:item_name) AND item_pos = (:item_pos)");

    query.bindValue(":item_name", item_name);
    query.bindValue(":item_pos", item_pos);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << query.lastError().text();
        emit error(query.lastError().text());
    }
}
