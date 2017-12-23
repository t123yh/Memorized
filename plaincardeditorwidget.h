#ifndef PLAINCARDEDITORWIDGET_H
#define PLAINCARDEDITORWIDGET_H

#include "cardeditorwidget.h"
#include <QWidget>

namespace Ui {
class PlainCardEditorWidget;
}

class PlainCardEditorWidget : public CardEditorWidget
{
    Q_OBJECT

public:
    explicit PlainCardEditorWidget(QWidget *parent = 0);
    ~PlainCardEditorWidget();

    void loadData(const QJsonDocument &data) override;
    QJsonDocument getData() override;

private:
    Ui::PlainCardEditorWidget *ui;
};

#endif // PLAINCARDEDITORWIDGET_H
