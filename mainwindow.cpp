#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cardsdialog.h"
#include "reviewdialog.h"

#include <QString>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_Cards_clicked()
{
    CardsDialog *dialog = new CardsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

void MainWindow::on_btn_Review_clicked()
{

    ReviewDialog *dialog = new ReviewDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
