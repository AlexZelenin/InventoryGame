include(../Common.pri)

QT += gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inventorydelegate.cpp \
    item.cpp \
    inventory.cpp \
    main.cpp \
    mainwindow.cpp \
    menuform.cpp \
    sourceitemmodel.cpp

HEADERS += \
    inventorydelegate.h \
    item.h \
    inventory.h \
    mainwindow.h \
    menuform.h \
    sourceitemmodel.h

FORMS += \
    mainwindow.ui \
    menuform.ui

win32:RC_ICONS = $${PWD}/rss/icons/icon.ico

RESOURCES = rss/resources.qrc

TARGET = Inventory

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Inventory/ -lDBInventory
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Inventory/ -lDBInventory
else:unix: LIBS += -L$$PWD/../../Inventory/ -lDBInventory

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
