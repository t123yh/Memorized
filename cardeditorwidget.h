#ifndef CARDEDITORWIDGET_H
#define CARDEDITORWIDGET_H

#include <QWidget>
#include <QJsonObject>

class CardEditorWidget : public QWidget
{
    Q_OBJECT
public:
    virtual void loadData(const QJsonDocument &data) = 0;
    virtual QJsonDocument getData() = 0;

signals:
    void valueChanged();

public slots:

protected:
    explicit CardEditorWidget(QWidget *parent = 0);
};

#endif // CARDEDITORWIDGET_H
