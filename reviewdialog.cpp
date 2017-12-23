#include "reviewdialog.h"
#include "ui_reviewdialog.h"

ReviewDialog::ReviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReviewDialog)
{
    ui->setupUi(this);
}

ReviewDialog::~ReviewDialog()
{
    delete ui;
}
