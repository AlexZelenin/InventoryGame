#include "menuform.h"
#include "ui_menuform.h"


MenuForm::MenuForm(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::MenuForm)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    this->setStyleSheet("QDialog {"
        "background-color: #32CD32;"
    "}");

    ui->pbNewGame->setStyleSheet("QPushButton {"
        "background-color: #E0FFFF;"
    "}");

    ui->pbQuit->setStyleSheet("QPushButton {"
        "background-color: #E0FFFF;"
    "}");

    connect(ui->pbQuit, &QPushButton::clicked, this, &MenuForm::quitGame);

    connect(ui->pbNewGame, SIGNAL(clicked()), this, SIGNAL(newGame()));
    connect(ui->pbNewGame, &QPushButton::clicked, this, &MenuForm::close);
}

MenuForm::~MenuForm()
{
    delete ui;
}

void MenuForm::quitGame()
{
    qApp->quit();
}

void MenuForm::showEvent(QShowEvent* e)
{
    m_animation = new QPropertyAnimation(this, "geometry");

    m_animation->setDuration(500);

    QRect start(pos().x(), 500, width(), height());
    QRect end(pos().x(), pos().y(), width(), height());

    m_animation->setStartValue(start);
    m_animation->setEndValue(end);

    m_animation->start(QAbstractAnimation::DeleteWhenStopped);
    QDialog::showEvent(e);
}
