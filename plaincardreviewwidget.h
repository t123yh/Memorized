#ifndef PLAINCARDREVIEWWIDGET_H
#define PLAINCARDREVIEWWIDGET_H

#include "cardreviewwidget.h"

#include <QJsonDocument>
#include <QString>
#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class PlainCardReviewWidget;
}

class PlainCardReviewWidget : public CardReviewWidget
{
  Q_OBJECT

public:
  explicit PlainCardReviewWidget(QWidget* parent = 0);
  ~PlainCardReviewWidget();
  void loadData(const QString& description, const QJsonDocument& data) override;
  double getPerformanceRating() override;
  void setPerformanceRating(double performanceRating) override;
  void resetPerformanceRating() override;
  void showContent();

private slots:
  void on_btn_ShowBackSide_clicked();

private:
  Ui::PlainCardReviewWidget* ui;
  QString currentFront;
  QString currentBack;
  QString currentNotes;
  bool backShown;
  QButtonGroup ratingButtonGroup;
};

#endif // PLAINCARDREVIEWWIDGET_H
