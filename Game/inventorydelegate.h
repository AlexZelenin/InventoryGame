#ifndef INVENTORYDELEGATE_H
#define INVENTORYDELEGATE_H


#include <QStyledItemDelegate>

#include <QObject>

/*
 * Класс InventoryDelegate
 *
 * Делегат для виджета инвентаря
*/

class InventoryDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit InventoryDelegate(QObject *parent = nullptr);
    virtual ~InventoryDelegate();

    /*
     * Переопределенный метод paint
     * Меняет цвет выделенной ячейки на белый
    */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const override;
};

#endif // INVENTORYDELEGATE_H
