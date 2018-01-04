#include "cardsdialog.h"
#include "datetimedisplaydelegate.h"
#include "editcarddialog.h"
#include "ui_cardsdialog.h"
#include "utils.h"

#include <QDateTime>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QtSql>

CardsDialog::CardsDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::CardsDialog)
{
  ui->setupUi(this);
  initializeModels();
  initCardGroupsGroupbox();
}

CardsDialog::~CardsDialog()
{
  delete ui;
}

void
CardsDialog::initCardGroupsGroupbox()
{
  ui->cmb_CardGroups->setModel(cardGroupsModel);
  ui->cmb_CardGroups->setModelColumn(cardGroupsModel->fieldIndex("Name"));
}

void
CardsDialog::initializeModels()
{
  cardsModel = new QSqlRelationalTableModel(this);
  cardsModel->setTable("cards");
  int groupIdIndex = cardsModel->fieldIndex("GroupId");
  cardsModel->setRelation(groupIdIndex,
                          QSqlRelation("card_groups", "Id", "Name"));
  cardsModel->select();

  cardGroupsModel = new QSqlTableModel(this);
  cardGroupsModel->setTable("card_groups");
  cardGroupsModel->setSort(cardGroupsModel->fieldIndex("Name"),
                           Qt::AscendingOrder);
  cardGroupsModel->select();

  ui->tbl_Cards->setModel(cardsModel);
  // ui->tbl_Cards->hideColumn(cardsModel->fieldIndex("GroupId")); // You cannot
  // use this one here
  ui->tbl_Cards->hideColumn(groupIdIndex);
  ui->tbl_Cards->hideColumn(cardsModel->fieldIndex("DaysBetweenReviews"));
  ui->tbl_Cards->hideColumn(cardsModel->fieldIndex("Id"));
  ui->tbl_Cards->hideColumn(cardsModel->fieldIndex("Data"));
  ui->tbl_Cards->setItemDelegateForColumn(cardsModel->fieldIndex("created"),
                                          new DateTimeDisplayDelegate);
  ui->tbl_Cards->setItemDelegateForColumn(
    cardsModel->fieldIndex("lastReviewed"), new DateTimeDisplayDelegate);
}

int
CardsDialog::getCurrentCardId()
{
  return cardsModel
    ->index(ui->tbl_Cards->selectionModel()->currentIndex().row(),
            cardsModel->fieldIndex("Id"))
    .data(0)
    .toInt();
}

int
CardsDialog::getCurrentGroupId()
{
  return cardGroupsModel
    ->index(ui->cmb_CardGroups->currentIndex(),
            cardGroupsModel->fieldIndex("Id"))
    .data(0)
    .toInt();
}

void
CardsDialog::showCards()
{
  cardsModel->setFilter(QString("GroupId = '%1'").arg(getCurrentGroupId()));
  cardsModel->select();
}

void
CardsDialog::on_cmb_CardGroups_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  showCards();
}

void
CardsDialog::on_btn_AddCard_clicked()
{
  EditCardDialog dialog(this);
  // dialog->setAttribute(Qt::WA_DeleteOnClose);
  int dialogResult = dialog.exec();

  if (dialogResult == QDialog::Accepted) {
    /*
    QSqlRecord record = cardsModel->record();
    record.setValue("Name", dialog.getCurrentName());
    record.setValue("Type", dialog.getCurrentType());
    record.setValue("Data", dialog.getCurrentData());
    record.setValue("LastReviewed", getCurrentDateTime());
    record.setValue("Difficulty", 0.3);
    record.setValue("DaysBetweenReview", 1);
    qDebug() << "CurrentGroupId" << getCurrentGroupId();
    record.setValue("GroupId", getCurrentGroupId());

    if (!cardsModel->insertRecord(0, record))
    {
        Crash(cardsModel->lastError().text());
    }
       qDebug() << cardsModel->editStrategy();
       // cardsModel->submitAll();
        qDebug() << cardsModel->query().lastQuery();
        */
    // I cannot get QSqlTableModel::insertRecord to work, so let's use raw query
    // instead.
    QSqlQuery query;
    query.prepare(
      "INSERT INTO `cards` (`Name`, `Type`, `Data`, `LastReviewed`, "
      "`Difficulty`, `DaysBetweenReviews`, `GroupId`, `Created`) "
      "VALUES (:name, :type, :data, :lastreviewed, "
      ":difficulty, :daysbetweenreviews, :groupid, :created)");
    query.bindValue(":name", dialog.getCurrentName());
    query.bindValue(":type", dialog.getCurrentType());
    auto cardData =
      QString(dialog.getCurrentData().toJson(QJsonDocument::Compact));
    query.bindValue(":data", cardData);
    query.bindValue(":lastreviewed", getCurrentDateTime());
    query.bindValue(":created", getCurrentDateTime());
    query.bindValue(":difficulty", 0.3);
    query.bindValue(":daysbetweenreviews", 1);
    query.bindValue(":groupid", getCurrentGroupId());
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
    showCards();
  }
}

void
CardsDialog::on_btn_RemoveCard_clicked()
{
  QMessageBox::StandardButton reply = QMessageBox::question(
    this,
    "L:D_N:dialog_ID:removeconfirm",
    QString(tr("Are you sure you want to remove this card?")),
    QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    QSqlQuery query;
    query.prepare("DELETE FROM `cards` WHERE `Id` = :id");
    query.bindValue(":id", getCurrentCardId());
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
    showCards();
  }
}

void
CardsDialog::on_btn_EditCard_clicked()
{
  int rowId = ui->tbl_Cards->selectionModel()->currentIndex().row();
  auto record = cardsModel->record(rowId);
  auto name = record.field("name").value().toString();
  auto data =
    QJsonDocument::fromJson(record.field("data").value().toString().toUtf8());
  auto type = record.field("type").value().toInt();
  EditCardDialog dialog(this);
  dialog.setData(name, (CardType)type, data);
  int dialogResult = dialog.exec();

  if (dialogResult == QDialog::Accepted) {
    QSqlQuery query;
    query.prepare("UPDATE `cards` "
                  "SET `Name` = :name, `Data` = :data, `Type` = :type "
                  "WHERE `Id` = :id ");
    query.bindValue(":name", dialog.getCurrentName());
    query.bindValue(":type", dialog.getCurrentType());
    query.bindValue(":data", QString(dialog.getCurrentData().toJson()));
    query.bindValue(":id", record.field("id").value().toInt());
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
    showCards();
  }
}

void
CardsDialog::on_btn_AddGroup_clicked()
{
  bool ok;
  QString text = QInputDialog::getText(this,
                                       "L:D_N:dialog_ID:groupname_RKB:true",
                                       tr("Group name:"),
                                       QLineEdit::Normal,
                                       "",
                                       &ok);
  if (ok) {
    QSqlQuery query;
    query.prepare("INSERT INTO `card_groups` (`Name`) "
                  "VALUES (:name)");
    query.bindValue(":name", text);
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
    int newId = query.lastInsertId().toInt();
    cardGroupsModel->select();
    auto list = cardGroupsModel->match(
      cardGroupsModel->index(0, cardGroupsModel->fieldIndex("Id")),
      0,
      QVariant(newId));
    if (list.cbegin() == list.cend()) {
      Crash("Insertion failed.");
    }
    auto newIndex = list.cbegin();
    ui->cmb_CardGroups->setCurrentIndex(newIndex->row());
  }
}

void
CardsDialog::on_btn_RemoveGroup_clicked()
{
  int currentId = getCurrentGroupId();

  QSqlQuery query;
  query.prepare("SELECT COUNT(*) FROM `cards` WHERE `GroupId` = :id");
  query.bindValue(":id", currentId);
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  query.next();
  int count = query.value(0).toInt();
  if (count != 0) {
    QMessageBox::StandardButton reply = QMessageBox::question(
      this,
      "L:D_N:dialog_ID:deleteconfirm",
      QString(tr("This group has %1 cards. Are you sure you want to delete the "
                 "whole card group?"))
        .arg(count),
      QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
      return;
    }
  }

  /*
  query.prepare("DELETE FROM `cards` WHERE `GroupId` = :id");
  query.bindValue(":id", getCurrentGroupId());
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  */
  query.prepare("DELETE FROM `card_groups` WHERE `Id` = :id");
  query.bindValue(":id", getCurrentGroupId());
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  cardGroupsModel->select();
  if (cardGroupsModel->rowCount() != 0) {
    // TODO
    ui->cmb_CardGroups->setCurrentIndex(0);
  }
}
