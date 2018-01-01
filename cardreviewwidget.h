#ifndef CARDREVIEWWIDGET_H
#define CARDREVIEWWIDGET_H

#include <QJsonDocument>
#include <QString>
#include <QWidget>

class CardReviewWidget : public QWidget
{
  Q_OBJECT
public:
  virtual void loadData(const QString& description,
                        const QJsonDocument& data) = 0;
  virtual void setPerformanceRating(double performanceRating) = 0;
  virtual void resetPerformanceRating() = 0;
  virtual double getPerformanceRating() = 0;

signals:
  void reviewed();

public slots:

protected:
  explicit CardReviewWidget(QWidget* parent = nullptr);
};

#endif // CARDREVIEWWIDGET_H
