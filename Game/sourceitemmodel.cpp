#include "sourceitemmodel.h"

#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMimeData>

#include "dbmanager.h"

SourceItemModel::SourceItemModel(QObject* parent) :
    QAbstractTableModel(parent),
    m_item(nullptr)
{

}

SourceItemModel::~SourceItemModel()
{

}

int SourceItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return CountRows;
}

int SourceItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return CountColumns;
}

QVariant SourceItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Item* item = nullptr;
    int i;

    if (index.row() == 0) {
        i = index.column();
    } else if (index.row() == 1) {
        if (3 + index.column() <= m_items.count())
            i = 3 + index.column();
        else
            return QVariant();
    } else if (index.row() == 2) {
        if (6 + index.column() <= m_items.count())
            i = 6 + index.column();
        else
            return QVariant();
    } else {
        return QVariant();
    }

    if (i >= m_items.count())
        return QVariant();

    item = m_items.value(i);

    if (!item)
        return QVariant();

    switch (role)
    {
    case Qt::DecorationRole:
        return QVariant(item->getImage());
    case Qt::UserRole:
        return QVariant(item->itemName());
    case ItemTypeRole:
        return QVariant(item->itemType());
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignBottom | Qt::AlignRight);

    default:
        return QVariant();
    }

    return QVariant();
}

void SourceItemModel::addItems(const QList<Item*>& items)
{
    m_items = items;
}

Qt::ItemFlags SourceItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return defaultFlags | Qt::ItemIsDropEnabled;
}

QHash<int, QByteArray> SourceItemModel::roleNames() const
{
    QHash<int, QByteArray> roles = {
        {SourceRole, "SourceRole"},
        {InventoryRole, "InventoryRole"},
        {ItemTypeRole, "ItemTypeRole"}
    };
    return roles;
}

QMimeData *SourceItemModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;

    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::UserRole).toString();
            mimeData->setText(text);
            QImage image = data(index, Qt::DecorationRole).value<QImage>();
            mimeData->setImageData(QVariant(image));
            DBTypes::ItemType type = data(index, ItemTypeRole).value<DBTypes::ItemType>();
            int alignment = data(index, Qt::TextAlignmentRole).toInt();

            stream << 1 << type << alignment;
        }
    }

    mimeData->setData(mimeTypes().value(0), encodedData);

    return mimeData;
}

void SourceItemModel::playSound(const DBTypes::ItemType &type)
{
    m_item = m_items.value(type);

    if (!m_item) return;

    m_item->playSound();
}

void SourceItemModel::moveItem(const DBTypes::ItemType & type, const uint& count, const QString & pos_from, const QString &pos_to)
{
    m_item = m_items.value(type);

    if (!m_item) return;

    m_item->moveItem(count, pos_from, pos_to);
}

void SourceItemModel::addCountInventory(const DBTypes::ItemType &type, const QString & pos)
{
    m_item = m_items.value(type);

    if (!m_item) return;

    m_item->addCountInventory(pos);
}

void SourceItemModel::subCountInventory(const DBTypes::ItemType &type, const QString &pos)
{
    m_item = m_items.value(type);

    if (!m_item) return;

    m_item->subCountInventory(pos);
}

void SourceItemModel::removeItem(const DBTypes::ItemType &type, const QString &pos)
{
    m_item = m_items.value(type);

    if (!m_item) return;

    m_item->removeItem(pos);
}

void SourceItemModel::clearInventory()
{
    dbManager()->clearTableInventory();
}
