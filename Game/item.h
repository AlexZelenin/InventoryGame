#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QHash>
#include <QString>
#include <QMediaPlayer>

#include "dbmanager.h"


/*
 * Класс Item
 *
 * Описывает предмет
*/
class Item : public QObject
{
    Q_OBJECT
public:

    enum ImageSize {
        Width = 90,
        Height = 90
    };

    explicit Item(const DBTypes::ItemType type, QObject *parent = nullptr);
    virtual ~Item();

    bool loadData();

    DBTypes::ItemType itemType() const;
    QString itemName() const;

    void playSound();
    QImage getImage() const ;

    void moveItem(const uint count, const QString& from_pos, const QString& to_pos);
    void addCountInventory(const QString& pos);
    void subCountInventory(const QString& pos);
    void removeItem(const QString& pos);

signals:


private:
    QMediaPlayer *m_sound;
    DBTypes::ItemType m_type;
    QString m_name;
    DBTypes::ITEM_OBJECT m_object;

};

#endif // ITEM_H
