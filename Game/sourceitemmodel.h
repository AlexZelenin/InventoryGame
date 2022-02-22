#ifndef SOURCEITEM_H
#define SOURCEITEM_H

#include <QObject>
#include <QString>
#include <QAbstractItemModel>

#include "item.h"

/*
 * Класс SourceItemModel
 *
 * Описывает модель исходной таблицы и переносимых данных в класс Inventory
*/

class SourceItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum TableSize {
        None = 0,
        CountRows = 3,
        CountColumns = 3
    };

    enum Role {
        SourceRole = Qt::UserRole + 1,
        InventoryRole,
        ItemTypeRole
    };

    SourceItemModel() {}
    explicit SourceItemModel(QObject* parent);
    virtual ~SourceItemModel();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    void addItems(const QList<Item*>& items);


public slots:
    void playSound(const DBTypes::ItemType& type);
    void moveItem(const DBTypes::ItemType& type, const uint& count, const QString& pos_from, const QString& pos_to);
    void addCountInventory(const DBTypes::ItemType& type, const QString& pos);
    void subCountInventory(const DBTypes::ItemType& type, const QString& pos);
    void removeItem(const DBTypes::ItemType& type, const QString& pos);
    void clearInventory();

private:

    QList<Item*> m_items;
    Item* m_item;

};

#endif // SOURCEITEM_H
