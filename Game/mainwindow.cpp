#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dbmanager.h"

#include "item.h"

#include "sourceitemmodel.h"
#include "inventory.h"
#include "inventorydelegate.h"

#include <QDebug>

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setDisabled(true);

    dbManager()->init();

    setStyleSheet("QMainWindow {"
        "background-color: #fff;"
    "}");

    m_menuForm = new MenuForm(ui->centralwidget);

    connect(ui->pbMainMenu, &QPushButton::clicked, this, &MainWindow::openMenu);

    connect(dbManager(), &DBManager::error, this, [this](const QString& message){
        QMessageBox::critical(this, "Error database", message,
                              QMessageBox::Cancel, QMessageBox::Cancel);
    });

    SourceItemModel *model = new SourceItemModel(this);

    QList<Item*> listItems;
    listItems.append(new Item(DBTypes::Apple, this));
    listItems.append(new Item(DBTypes::Cherry, this));
    listItems.append(new Item(DBTypes::Kiwi, this));
    listItems.append(new Item(DBTypes::Strawberry, this));

    for (int i = 0; i < listItems.count(); i++) {
        listItems.at(i)->loadData();
    }

    model->addItems(listItems);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->setStyleSheet("QTableView::item {"
        "border: none;"
        "background-color : none;"
    "}");

    InventoryDelegate* delegateView = new InventoryDelegate(ui->tableView);
    ui->tableView->setItemDelegate(delegateView);

    InventoryDelegate* delegateWidget = new InventoryDelegate(ui->tableWidget);
    ui->tableWidget->setItemDelegate(delegateWidget);

    connect(ui->tableWidget, &Inventory::playSound, model, &SourceItemModel::playSound);
    connect(ui->tableWidget, &Inventory::moveItem, model, &SourceItemModel::moveItem);
    connect(ui->tableWidget, &Inventory::addCountInventory, model, &SourceItemModel::addCountInventory);
    connect(ui->tableWidget, &Inventory::subCountInventory, model, &SourceItemModel::subCountInventory);
    connect(ui->tableWidget, &Inventory::removeItem, model, &SourceItemModel::removeItem);
    connect(ui->tableWidget, &Inventory::clearInventory, model, &SourceItemModel::clearInventory);

    connect(m_menuForm, &MenuForm::newGame, this, [this](){
        ui->tableWidget->clear();
    });

    ui->tableWidget->setStyleSheet("QTableWidget::item {"
        "border: 1px solid grey;"
        "background-color : none;"
    "}");
}

MainWindow::~MainWindow()
{
    dbManager()->clearTableInventory();
    dbManager()->close();
    delete ui;
}

void MainWindow::openMenu()
{
    setDisabled(true);
    const int xWindow = geometry().x() + 2;
    const int yWindow = y() + (geometry().y() - frameGeometry().y());
    const int wWindow = width() - 4;
    const int hWindow = 75;

    m_menuForm->setEnabled(true);
    m_menuForm->setGeometry(xWindow, yWindow, wWindow, hWindow);
    m_menuForm->exec();
    setDisabled(false);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMetaObject::invokeMethod(this, "openMenu", Qt::QueuedConnection);
    QMainWindow::showEvent(event);
}
