#include "reviewdialog.h"
#include "plaincardreviewwidget.h"
#include "reviewutils.h"
#include "ui_reviewdialog.h"
#include "utils.h"

#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <algorithm>

const double CorrectThreshold = 0.6;

ReviewDialog::ReviewDialog(int userId, QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ReviewDialog)
  , userId(userId)
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
  if (!currentSessionPerformance.empty()) {
    commitReviews();
  }
  currentSessionCards.clear();
  currentSessionPerformance.clear();
  QSqlQuery query;
  QString selectQuery(
    "SELECT `user_cards`.`Id` FROM `user_cards` "
    "INNER JOIN `cards` ON `user_cards`.`CardId` = `cards`.`Id` "),
    orderClause(
      " ORDER BY  ("
      "cast(cast(julianday('now') + 0.5 as integer) - "
      "cast(julianday(`user_cards`.`LastReviewed`) + 0.5 as integer) as real)"
      ") / cast(`user_cards`.`DaysBetweenReviews` as real) DESC ");

  QString whereClause(" WHERE `user_cards`.`UserId` = :user_id ");
  if (groupId != GROUP_ALL) {
    whereClause += "AND `cards`.`GroupId` = :group_id ";
  }
  query.prepare(selectQuery + whereClause + orderClause);

  query.bindValue(":user_id", this->userId);
  if (groupId != GROUP_ALL) {
    query.bindValue(":group_id", groupId);
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
ReviewDialog::commitReviews()
{
  for (auto iter = currentSessionPerformance.cbegin();
       iter != currentSessionPerformance.cend();
       iter++) {
    int userCardId = iter.key();
    double performanceRating = std::get<0>(iter.value());
    QDateTime reviewTime = std::get<1>(iter.value());
    QSqlQuery query;
    query.prepare("SELECT `LastReviewed`, `DaysBetweenReviews`, `Difficulty` "
                  "FROM `user_cards` "
                  "WHERE `Id` = :id");
    query.bindValue(":id", userCardId);
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
    if (!query.first()) {
      Crash("Card not found!");
    }
    QDateTime lastReviewed = convertToDateTime(query.value(0).toString());
    int expectedDaysBetweenReviews = query.value(1).toInt();
    double difficulty = query.value(2).toDouble();

    // Credit: algorithm from here
    // http://www.blueraja.com/blog/477/a-better-spaced-repetition-learning-algorithm-sm2

    bool correct = performanceRating >= CorrectThreshold;
    int daysFromLastReview = lastReviewed.daysTo(QDateTime::currentDateTime());
    double percentOverdue = correct
                              ? std::min(2.0,
                                         (double)daysFromLastReview /
                                           (double)expectedDaysBetweenReviews)
                              : 1;

    difficulty +=
      percentOverdue * (1.0 / 17.0) * (6.0 - 9.0 * performanceRating);
    if (difficulty < 0)
      difficulty = 0;
    if (difficulty > 1)
      difficulty = 1;
    double difficultyWeight = 3 - 1.7 * difficulty;

    expectedDaysBetweenReviews *=
      correct ? 1 + (difficultyWeight - 1) * percentOverdue
              : 1 / difficultyWeight / difficultyWeight;
    if (expectedDaysBetweenReviews == 0) {
      expectedDaysBetweenReviews = 1;
    }

    query.prepare(
      "INSERT INTO `review_history` "
      "(`UserCardId`, `DateTime`, `PerformanceRating`, "
      "`DifficultyAfter`, `DaysBetweenReviewsAfter`) "
      "VALUES (:id, :currentTime, :rating, :difficulty, :daysBetween)");
    query.bindValue(":id", userCardId);
    query.bindValue(":currentTime", toSqlTime(reviewTime));
    query.bindValue(":rating", performanceRating);
    query.bindValue(":difficulty", difficulty);
    query.bindValue(":daysBetween", expectedDaysBetweenReviews);
    if (!query.exec()) {
      Crash(query.lastError().text());
    }

    query.prepare("UPDATE `user_cards` SET `LastReviewed` = :currentTime, "
                  "`DaysBetweenReviews` = :daysBetween, "
                  "`Difficulty` = :difficulty "
                  "WHERE `Id` = :id");
    query.bindValue(":id", userCardId);
    query.bindValue(":currentTime", toSqlTime(reviewTime));
    query.bindValue(":difficulty", difficulty);
    query.bindValue(":daysBetween", expectedDaysBetweenReviews);
    if (!query.exec()) {
      Crash(query.lastError().text());
    }
  }
  currentSessionPerformance.clear();
}

void
ReviewDialog::showCard(int cardIndex)
{
  if (cardIndex < 0 || cardIndex >= currentSessionCards.size()) {
    Crash("cardIndex out of bound.");
  }
  currentCardIndex = cardIndex;
  adjustNavigationButton();

  int userCardId = currentSessionCards[currentCardIndex];
  QSqlQuery query;
  query.prepare(
    "SELECT `cards`.`Name`, `cards`.`Data`, `cards`.`Type`, "
    "`card_groups`.`Name`, `user_cards`.`LastReviewed`, "
    "`user_cards`.`DaysBetweenReviews` "
    "FROM `cards` "
    "INNER JOIN card_groups on `cards`.`GroupId` = `card_groups`.`Id` "
    "INNER JOIN user_cards on `cards`.`Id` = `user_cards`.`CardId` "
    "WHERE `user_cards`.`Id` = :id");
  query.bindValue(":id", userCardId);
  if (!query.exec()) {
    Crash(query.lastError().text());
  }
  if (!query.first()) {
    Crash("No such card!");
  }

  int overdueCount = getOverdueItemCount(getCurrentCardGroup(), userId);
  // Hide overdue card count if current card is not overdue
  QString cardCountTemplate =
    currentCardIndex < overdueCount ? "%1 / %3 / %2" : "%1 / %2";
  ui->lb_CardCount->setText(cardCountTemplate.arg(currentCardIndex + 1)
                              .arg(currentSessionCards.count())
                              .arg(overdueCount));

  QString cardName = query.value(0).toString();
  QString cardGroupName = query.value(3).toString();
  QString cardDescription;
  if (getCurrentCardGroup() == GROUP_ALL) {
    cardDescription += QString("<small><i>%1</i></small> ").arg(cardGroupName);
  }
  cardDescription += cardName;

  QDateTime lastReviewed = convertToDateTime(query.value(4).toString());
  int daysBetweenReviews = query.value(5).toInt();
  QString moreInfoText(tr("%1 ").arg(getFancyDateTimeString(lastReviewed)));
  if (lastReviewed.daysTo(QDateTime::currentDateTime()) >= daysBetweenReviews) {
    moreInfoText += tr(" <b>Overdue</b>");
  } else {
    moreInfoText += tr(" Not overdue");
  }
  ui->lb_MoreInfo->setText(moreInfoText);

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

    if (currentSessionPerformance.contains(userCardId)) {
      currentReviewWidget->setPerformanceRating(
        std::get<0>(currentSessionPerformance[userCardId]));
    } else {
      currentReviewWidget->resetPerformanceRating();
    }

    connect(currentReviewWidget,
            &CardReviewWidget::reviewed,
            this,
            &ReviewDialog::on_reviewed);
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

int
ReviewDialog::getCurrentCardGroup()
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

void
ReviewDialog::on_reviewed()
{
  double rating = currentReviewWidget->getPerformanceRating();
  int userCardId = currentSessionCards[currentCardIndex];
  if (rating < 0) {
    if (currentSessionPerformance.contains(userCardId)) {
      currentSessionPerformance.remove(userCardId);
    }
    return;
  }
  currentSessionPerformance[userCardId] =
    std::make_tuple(rating, QDateTime::currentDateTime());
}

void
ReviewDialog::on_btn_Exit_clicked()
{
  this->accept();
}

void
ReviewDialog::on_ReviewDialog_accepted()
{
  commitReviews();
}

void
ReviewDialog::on_ReviewDialog_rejected()
{
  commitReviews();
}

void
ReviewDialog::on_btn_Reset_clicked()
{
  if (currentReviewWidget != NULL) {
    currentReviewWidget->resetPerformanceRating();
  }
}

void
ReviewDialog::on_btn_Clear_clicked()
{
  QMessageBox::StandardButton reply =
    QMessageBox::question(this,
                          "L:D_N:dialog_ID:clearconfirm",
                          QString(tr("Clear %1 reviews you've just done?"))
                            .arg(currentSessionPerformance.count()),
                          QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    currentSessionPerformance.clear();
    if (currentSessionCards.count() > 0) {
      showCard(0);
    }
  }
}
