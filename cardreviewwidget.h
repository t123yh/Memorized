#ifndef CARDREVIEWWIDGET_H
#define CARDREVIEWWIDGET_H

#include <QWidget>
#include <QJsonDocument>

class CardReviewWidget : public QWidget
{
    Q_OBJECT
public:
    virtual void loadData(const QJsonDocument& data) = 0;
    virtual double getPerformanceRating() = 0;

signals:
    void reviewed(double performanceRating);

public slots:

protected:
    explicit CardReviewWidget(QWidget *parent = nullptr);
};

#endif // CARDREVIEWWIDGET_H