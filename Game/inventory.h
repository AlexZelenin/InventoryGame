#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>
#include <QTableWidget>

#include "item.h"

/**
 * Класс Inventory
 * Родительский класс QTableWidget
 *
 * Описывает таблицу инвентаря и действия с предметами в нем
*/

class Inventory : public QTableWidget
{
    Q_OBJECT
public:
    explicit Inventory(QWidget* parent = nullptr);
    virtual ~Inventory();

    /*
     * Переопределенный метод
     * Определяет поддержку типов действий для сброса в принимающую ячейку
    */
    Qt::DropActions supportedDropActions() const override;

    /*
     * Переопределенный метод
     * Принимает событие сброса в ячейку
     * В нутри определяет действие CopyAction или MoveAction
     * Проверяет на разность типов
    */
    void dropEvent(QDropEvent* event) override;

    /*
     * Переопределенный метод
     * Принимает событие перемещения захваченой ячейки
    */
    void dragMoveEvent(QDragMoveEvent* event) override;

    /*
     * Переопределенный метод
     * Принимает событие входа указателя мыши в новую ячейку при действии QDrag
    */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /*
     * Переопределенный метод
     * Принимает событие зажатых клавиш
     * Проверяет какя клавиша зажата. Если RightButton то уменьшает количество предмета в инвентаре
     * Если LeftButton то опрделеяет начальную позицию при перемещении
    */
    void mousePressEvent(QMouseEvent* event) override;

    /*
     * Переопределенный метод
     * Кодирует переносимые данные в MIME тип
     * заполняет необходимые данные для переноса
    */
    virtual QMimeData *mimeData(const QList<QTableWidgetItem*> items) const override;

    /*
    */
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action) override;

    /*
     * Метод очистки виджета Inventory
     * И очищает таблицу инвентаря в БД
    */
    void clear();


signals:

    /*
     * Сигнал playSound
     * отправляет событие для воспроизведения звука
     * Вызывается в mousePressEvent если нажата правая кнопка мыши и если
     * в ячейке есть предмет
     *
     * Принимает тип предмета
    */
    void playSound(const DBTypes::ItemType& type);

    /*
     * Сигнал moveItem
     * отправялет событие перемещения предмета в нутри инвентаря
     *
     * Прнимает (тип предмета, количество, позиция от куда, позиция куда)
    */
    void moveItem(const DBTypes::ItemType&, const uint&, const QString&, const QString&);

    /*
     * Сигнал addCountInventory
     *
     * Добавляет предмет в инвентарь
     * Если предмет добавляется в ячейку где уже есть предмет этого типа то увеличивает его количество на 1
     *
     * Принимает (тип предмета, позицию)
    */
    void addCountInventory(const DBTypes::ItemType&, const QString&);

    /*
     * Сигнал subCountInventory
     * уменьшаяет количество предмета в инвентаре на 1
     *
     * Принимает (тип предмета, позицию)
    */
    void subCountInventory(const DBTypes::ItemType&, const QString&);

    /*
     * Сигнал removeItem
     * Если количество предмета достигло 0 то удаляет предмет из виджета и из таблици БД
     *
     * Принимает (тип предмета, позицию)
    */
    void removeItem(const DBTypes::ItemType&, const QString&);

    /*
     * Сигнал clearInventory
     * Вызывается для удаления всех записей в таблице БД инвентаря
    */
    void clearInventory();

private:


};

#endif // INVENTORY_H
