#include "inventory.h"

#include "sourceitemmodel.h"
#include "dbmanager.h"

#include <QHeaderView>
#include <QDropEvent>
#include <QMimeData>

#include <QDebug>

Inventory::Inventory(QWidget* parent) :
    QTableWidget(parent)
{
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);

    viewport()->setMouseTracking(true);
    setColumnCount(SourceItemModel::CountColumns);
    setRowCount(SourceItemModel::CountRows);

    setFrameStyle(0);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Inventory::~Inventory()
{

}

Qt::DropActions Inventory::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

void Inventory::dropEvent(QDropEvent *event)
{
    const QMimeData *data = event->mimeData();
    const QModelIndex index = indexAt(event->pos());
    const QString& item_name = data->text();
    const QImage& item_image = data->imageData().value<QImage>();

    const QString& target = index.data(Qt::UserRole).toString();

    if (item_name == target || target.isEmpty()) {
        QTableWidget::dropEvent(event);

        QStringList types = mimeTypes();
        QByteArray d = data->data(types.at(0));

        QDataStream stream(&d, QIODevice::ReadOnly);

        DBTypes::ItemType id = DBTypes::None;
        int alignment = -1;
        QString count;

        while (!stream.atEnd()) {
            stream >> count >> id >> alignment;
        }

        const QString& pos_to = QString("%1,%2").arg(QString::number(index.row()), QString::number(index.column()));

        if (event->dropAction() == Qt::MoveAction) {

            if (dropIndicatorPosition() == QAbstractItemView::OnItem) {

                emit moveItem(id, count.toUInt(), property("pos_from").toString(), pos_to);

                model()->setData(index, dbManager()->getCountInventoryItem(item_name, pos_to), Qt::DisplayRole);
                model()->setData(index, item_name, Qt::UserRole);
                model()->setData(index, item_image, Qt::DecorationRole);
                model()->setData(index, alignment, Qt::TextAlignmentRole);
                model()->setData(index, id, SourceItemModel::ItemTypeRole);
            }
        } else if (Qt::CopyAction) {
            emit addCountInventory(id, pos_to);

            model()->setData(index, dbManager()->getCountInventoryItem(item_name, pos_to), Qt::DisplayRole);
            model()->setData(index, item_name, Qt::UserRole);
            model()->setData(index, item_image, Qt::DecorationRole);
            model()->setData(index, alignment, Qt::TextAlignmentRole);
            model()->setData(index, id, SourceItemModel::ItemTypeRole);
        }
    }

    repaint();
}

void Inventory::dragMoveEvent(QDragMoveEvent *event)
{
    Q_UNUSED(event);
    // QTableWidget::dragMoveEvent(event);
}

void Inventory::dragEnterEvent(QDragEnterEvent *event)
{
    QTableWidget::dragEnterEvent(event);
    if (event->mimeData()->text().isEmpty()) event->ignore();
}

bool Inventory::dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action)
{
    if (action == Qt::IgnoreAction)
        return true;

    return QTableWidget::dropMimeData(row, column, data, action);
}

void Inventory::clear()
{
    emit clearInventory();
    QTableWidget::clear();
}

void Inventory::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);

    const QModelIndex& index = currentIndex();
    const QString& pos = QString("%1,%2").arg(QString::number(index.row()), QString::number(index.column()));

    const DBTypes::ItemType& type = index.data(SourceItemModel::ItemTypeRole).value<DBTypes::ItemType>();

    if (event->button() == Qt::RightButton) {
        if (index.data(Qt::DisplayRole).toUInt() > 1) {

            emit playSound(type);
            emit subCountInventory(type, pos);

            model()->setData(index, dbManager()->getCountInventoryItem(index.data(Qt::UserRole).toString(), pos), Qt::DisplayRole);

        } else if (index.data(Qt::DisplayRole).toUInt() > 0) {

            emit playSound(type);
            emit removeItem(type, pos);

            model()->setData(index, QVariant(), Qt::DisplayRole);
            model()->setData(index, QVariant(), Qt::UserRole);
            model()->setData(index, QVariant(), Qt::DecorationRole);
            model()->setData(index, QVariant(), Qt::TextAlignmentRole);
            model()->setData(index, QVariant(), SourceItemModel::ItemTypeRole);
        }
    } else if (event->button() == Qt::LeftButton) {
        setProperty("pos_from", pos);
    }
}

QMimeData *Inventory::mimeData(const QList<QTableWidgetItem *> items) const
{
    QMimeData *mimeData = new QMimeData;

    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach(const QTableWidgetItem* item, items) {

        if (!item) return mimeData;

        QString text =item->data(Qt::UserRole).toString();
        mimeData->setText(text);
        QImage image = item->data(Qt::DecorationRole).value<QImage>();
        mimeData->setImageData(QVariant(image));
        DBTypes::ItemType type = item->data(SourceItemModel::ItemTypeRole).value<DBTypes::ItemType>();
        int alignment = item->data(Qt::TextAlignmentRole).toInt();

        stream << item->data(Qt::DisplayRole).toString() << type << alignment;
    }

    mimeData->setData(mimeTypes().value(0), encodedData);

    return mimeData;
}
