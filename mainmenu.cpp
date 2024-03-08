#include "mainmenu.h"
#include <QWidget>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu(){
    delete ui;
}

