#include "reviewdialog.h"
#include "plaincardreviewwidget.h"
#include "ui_reviewdialog.h"
#include "utils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QSqlError>
#include <QSqlQuery>

ReviewDialog::ReviewDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ReviewDialog)
{
  ui->setupUi(this);
  showCardGroups();
}

ReviewDialog::~ReviewDialog()
{
  delete ui;
}

void
ReviewDialog::showCardGroups()
{
  QSqlQuery query;
  if (!query.exec("SELECT `Id`, `Name` FROM `card_groups`")) {
    Crash(query.lastError().text());
  }

  ui->cb_CardGroup->addItem(tr("<all>"), QVariant(GROUP_ALL));
  while (query.next()) {
    int id = query.value(0).toInt();
    QString name = query.value(1).toString();
    ui->cb_CardGroup->addItem(name, QVariant(id));
  }
}

void
ReviewDialog::startReviewSession(int groupId)
{
  currentSessionCards.clear();
  currentSessionPerformance.clear();
  QSqlQuery query;
  QString selectQuery("SELECT `Id` FROM `cards` "),
    orderClause(" ORDER BY  (strftime('%s', 'now') - strftime('%s', "
                "`LastReviewed`)) / `DaysBetweenReviews` DESC");
  if (groupId != GROUP_ALL) {
    QString whereClause(" WHERE `GroupId` = :group_id ");
    query.prepare(selectQuery + whereClause + orderClause);
    query.bindValue(":group_id", groupId);
  } else {
    query.prepare(selectQuery + orderClause);
  }
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  while (query.next()) {
    int id = query.value(0).toInt();
    currentSessionCards.push_back(id);
  }

  if (currentSessionCards.count() > 0) {
    showCard(0);
  }
}

void
ReviewDialog::showCard(int cardIndex)
{
  if (cardIndex < 0 || cardIndex >= currentSessionCards.size()) {
    Crash("cardIndex out of bound.");
  }
  currentCardIndex = cardIndex;
  adjustNavigationButton();

  int cardId = currentSessionCards[currentCardIndex];
  QSqlQuery query;
  query.prepare("SELECT `cards`.`Name`, `cards`.`Data`, `cards`.`Type`, `card_groups`.`Name` "
                "FROM `cards` "
                "INNER JOIN card_groups on `cards`.`GroupId` = `card_groups`.`Id` "
                "WHERE `cards`.`Id` = :id");
  query.bindValue(":id", cardId);
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  if (!query.first()) {
    Crash("No such card!");
  }

  QString cardName = query.value(0).toString();
  QString cardGroupName = query.value(3).toString();
  QString cardDescription;
  if (getCurrentCardGroup() == GROUP_ALL)
  {
    cardDescription += QString("<small><i>%1</i></small> ").arg(cardGroupName);
  }
  cardDescription += cardName;

  QJsonDocument dataDoc =
    QJsonDocument::fromJson(query.value(1).toString().toUtf8());
  CardType type = (CardType)query.value(2).toInt();

  if (currentReviewWidget != NULL) {
    currentReviewWidget->deleteLater();
    currentReviewWidget = NULL;
  }

  switch (type) {
    case PLAIN_CARD:
      currentReviewWidget = new PlainCardReviewWidget(this);
      break;
    default:
      Crash(QString("Unknown card type %1.").arg(type));
      break;
  }

  if (currentReviewWidget != NULL) {
    currentReviewWidget->loadData(cardDescription, dataDoc);

    if (currentSessionPerformance.contains(cardId)) {
      currentReviewWidget->setPerformanceRating(
        currentSessionPerformance[cardId]);
    } else {
      currentReviewWidget->resetPerformanceRating();
    }

    ui->layout_Review->addWidget(currentReviewWidget);
  }
}

void
ReviewDialog::adjustNavigationButton()
{
  if (currentSessionCards.size() == 0) {
    ui->btn_Prev->setEnabled(false);
    ui->btn_Next->setEnabled(false);
  } else {
    ui->btn_Prev->setEnabled(currentCardIndex != 0);
    ui->btn_Next->setEnabled(currentCardIndex < currentSessionCards.size() - 1);
  }
}

int ReviewDialog::getCurrentCardGroup()
{
    return ui->cb_CardGroup->currentData().toInt();
}

void
ReviewDialog::on_cb_CardGroup_currentIndexChanged(int index)
{
  Q_UNUSED(index);
  startReviewSession(getCurrentCardGroup());
}

void
ReviewDialog::on_btn_Prev_clicked()
{
  showCard(currentCardIndex - 1);
}

void
ReviewDialog::on_btn_Next_clicked()
{
  showCard(currentCardIndex + 1);
}
