#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "menuform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openMenu();

protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::MainWindow *ui;

    MenuForm *m_menuForm;
    QPropertyAnimation *m_animation;
};
#endif // MAINWINDOW_H
