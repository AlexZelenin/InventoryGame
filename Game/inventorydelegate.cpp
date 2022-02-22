#include "inventorydelegate.h"

#include <QAbstractItemView>
#include <QPainter>

#include <QDebug>

InventoryDelegate::InventoryDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

InventoryDelegate::~InventoryDelegate()
{

}

void InventoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(!index.isValid())
        return;

    QStyleOptionViewItem option_styled = option;
    QStyledItemDelegate::initStyleOption(&option_styled, index);
    QRect rect = option_styled.rect;

    painter->setPen(Qt::NoPen);

    if(option.state & QStyle::State_Selected){
        QColor color(Qt::white);
        color.setAlpha(255);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawRoundedRect(rect, 0, 0);
    } else if (option.state | QStyle::State_Enabled) {
        QColor color(Qt::white);
        color.setAlpha(255);
        QBrush brush(color);
        painter->setBrush(brush);
        painter->drawRoundedRect(rect, 0, 0);
    }

    QStyledItemDelegate::paint(painter, option, index);
}
