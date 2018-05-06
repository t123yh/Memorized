#include "mainwindow.h"
#include "cardsdialog.h"
#include "reviewdialog.h"
#include "reviewutils.h"
#include "ui_mainwindow.h"
#include "usermanagementdialog.h"
#include "utils.h"

#include <QString>
#include <QtSql>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  refreshUser();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::on_btn_Cards_clicked()
{
  CardsDialog* dialog = new CardsDialog(this);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->exec();
  refreshUser();
}

void
MainWindow::on_btn_Review_clicked()
{
  auto selectedList = ui->lw_Users->selectedItems();
  if (selectedList.count() == 0) {
    QMessageBox::warning(this,
                         "L:D_N:dialog_ID:warning",
                         tr("Please select a user."),
                         QMessageBox::Ok,
                         QMessageBox::Ok);
    return;
  }
  int userId = selectedList.first()->data(Qt::UserRole).toInt();
  ReviewDialog* dialog = new ReviewDialog(userId, this);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->exec();
  refreshUser();
}

void
MainWindow::refreshUser()
{
  ui->lw_Users->clear();
  QSqlQuery query;
  QString selectQuery("SELECT `Id`, `Name` FROM `users` ");
  query.prepare(selectQuery);
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  while (query.next()) {
    int userId = query.value(0).toInt();
    int overdueCount = getOverdueItemCount(GROUP_ALL, userId);
    QString userName = query.value(1).toString();
    if (overdueCount > 0) {
      userName += tr(" (%1)").arg(overdueCount);
    }
    QListWidgetItem* newItem = new QListWidgetItem(userName, ui->lw_Users);
    newItem->setData(Qt::UserRole, userId);
  }
}

void
MainWindow::on_btn_Users_clicked()
{
  UserManagementDialog* dialog = new UserManagementDialog(this);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->exec();
  refreshUser();
}
