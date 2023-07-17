#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../PolyQtTable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PolyQtTableWidget* wid = new PolyQtTableWidget();

    ui->gridLayoutPoly->addWidget(wid);

    wid->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

