TEMPLATE = subdirs

QT += core

CONFIG += ordered

SUBDIRS += \
    DBInventory \
    Game
	
Game.depends = DBInventory


OTHER_FILES += \
    Common.pri
