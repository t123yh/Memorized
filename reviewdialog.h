#ifndef REVIEWDIALOG_H
#define REVIEWDIALOG_H

#include "cardreviewwidget.h"

#include <QDialog>
#include <QHash>
#include <QSqlTableModel>
#include <QVector>

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ReviewDialog(QWidget* parent = 0);
  ~ReviewDialog();

private slots:
  void on_cb_CardGroup_currentIndexChanged(int index);

  void on_btn_Prev_clicked();

  void on_btn_Next_clicked();

  void on_reviewed();

  void on_btn_Reset_clicked();

  void on_btn_Exit_clicked();

  void on_ReviewDialog_accepted();

  void on_ReviewDialog_rejected();

  void on_btn_Clear_clicked();

private:
  Ui::ReviewDialog* ui;
  CardReviewWidget* currentReviewWidget = NULL;

  void showCardGroups();
  void startReviewSession(int groupId);
  void commitReviews();

  void showCard(int cardIndex);
  void adjustNavigationButton();

  int getCurrentCardGroup();

  QHash<int, double> currentSessionPerformance;
  QVector<int> currentSessionCards;
  int currentCardIndex;
};

#endif // REVIEWDIALOG_H
