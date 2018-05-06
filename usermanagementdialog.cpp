#include "usermanagementdialog.h"
#include "ui_usermanagementdialog.h"
#include "utils.h"

#include <QInputDialog>
#include <QtSql>

UserManagementDialog::UserManagementDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::UserManagementDialog)
{
  ui->setupUi(this);
}

UserManagementDialog::~UserManagementDialog()
{
  delete ui;
}

void
UserManagementDialog::on_btn_Add_clicked()
{
  qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);

  QSqlQuery query;
  query.prepare("INSERT INTO `users` (`Name`) "
                "VALUES (:name)");
  query.bindValue(":name", ui->tb_Username->text());
  if (!query.exec()) {
    Crash(query.lastError().text());
  }

  int userId = query.lastInsertId().toInt();
  query.prepare("SELECT `Id` FROM `cards`");

  if (!query.exec()) {
    Crash(query.lastError().text());
  }

  int days = ui->sb_AddDays->value();
  while (query.next()) {
    int id = query.value(0).toInt();
    QDateTime lastReviewed = QDateTime::currentDateTime().addDays(-1);
    int daysBetween = 1 + (qrand() % days);
    double difficulty = 0.3;

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO `user_cards` (`UserId`, `CardId`, "
                        "`Difficulty`, `LastReviewed`, `DaysBetweenReviews`)"
                        "VALUES    (:userId,  :cardId,  "
                        ":difficulty,  :lastReviewed,  :daysBetweenReviews)");

    insertQuery.bindValue(":userId", userId);
    insertQuery.bindValue(":cardId", id);
    insertQuery.bindValue(":difficulty", difficulty);
    insertQuery.bindValue(":lastReviewed", lastReviewed);
    insertQuery.bindValue(":daysBetweenReviews", daysBetween);

    if (!insertQuery.exec()) {
      Crash(insertQuery.lastError().text());
    }
  }
  this->close();
}
