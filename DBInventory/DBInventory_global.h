#ifndef DBINVENTORY_GLOBAL_H
#define DBINVENTORY_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QObject>
#include <QString>
#include <QHash>
#include <QMetaType>

#if defined(DBINVENTORY_LIBRARY)
#  define DBINVENTORY_EXPORT Q_DECL_EXPORT
#else
#  define DBINVENTORY_EXPORT Q_DECL_IMPORT
#endif

namespace DBTypes {

const QString dbName = QStringLiteral("inventory_db.sqlite");
const QString dbDriver = QStringLiteral("QSQLITE");

typedef QMultiHash<QString, QString> HASH_ITEMS;

enum ItemType {
    Apple = 0,
    Cherry,
    Kiwi,
    Strawberry,
    None = 0x0100
};

typedef struct ItemObject {
    ItemType item_id;
    QString item_name;
    QString item_image;
    QString item_sound;
} ITEM_OBJECT;

}

Q_DECLARE_METATYPE(DBTypes::ItemType);

#endif // DBINVENTORY_GLOBAL_H
