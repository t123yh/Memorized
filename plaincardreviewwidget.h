#ifndef PLAINCARDREVIEWWIDGET_H
#define PLAINCARDREVIEWWIDGET_H

#include "cardreviewwidget.h"

#include <QWidget>
#include <QJsonDocument>
#include <QString>

namespace Ui {
class PlainCardReviewWidget;
}

class PlainCardReviewWidget : public CardReviewWidget
{
    Q_OBJECT

public:
    explicit PlainCardReviewWidget(QWidget *parent = 0);
    ~PlainCardReviewWidget();
    void loadData(const QJsonDocument &data) override;
    double getPerformanceRating() override;
    void showContent();

private:
    Ui::PlainCardReviewWidget *ui;
    QString currentFront;
    QString currentBack;
    QString currentNotes;
    bool backShown;
};

#endif // PLAINCARDREVIEWWIDGET_H
