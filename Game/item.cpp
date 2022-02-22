#include "item.h"


Item::Item(const DBTypes::ItemType type, QObject *parent) :
    QObject(parent),
    m_type(type)
{
    m_sound = new QMediaPlayer(this);
}

Item::~Item()
{

}

bool Item::loadData()
{
    m_object = dbManager()->getItem(m_type);
    m_name = m_object.item_name;

    if (m_name.isEmpty()) return false;
    return true;
}

DBTypes::ItemType Item::itemType() const
{
    return m_type;
}

QString Item::itemName() const
{
    return m_name;
}

void Item::playSound()
{
    m_sound->setMedia(QUrl(m_object.item_sound));
    m_sound->setVolume(100);
    m_sound->play();
}

QImage Item::getImage() const
{
    QString path = m_object.item_image;
    QImage image(path);
    image = image.scaled(QSize(ImageSize::Width, ImageSize::Height), Qt::KeepAspectRatio);
    return image;
}

void Item::moveItem(const uint count, const QString &from_pos, const QString &to_pos)
{
    dbManager()->moveItemInventory(m_type, m_name, count, from_pos, to_pos);
}

void Item::addCountInventory(const QString &pos)
{
    dbManager()->addCountInventoryItem(m_type, m_name, 1, pos);
}

void Item::subCountInventory(const QString &pos)
{
    dbManager()->subCountInventoryItem(m_name, pos);
}

void Item::removeItem(const QString &pos)
{
    dbManager()->removeItem(m_name, pos);
}
