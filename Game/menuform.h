#ifndef MENUFORM_H
#define MENUFORM_H

#include <QDialog>
#include <QPropertyAnimation>

namespace Ui {
class MenuForm;
}

class MenuForm : public QDialog
{
    Q_OBJECT

public:
    explicit MenuForm(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~MenuForm();

protected:
    void showEvent(QShowEvent* e) override;

public slots:
    void quitGame();

signals:
    void newGame();

private:
    Ui::MenuForm *ui;

    QPropertyAnimation *m_animation;
};

#endif // MENUFORM_H
