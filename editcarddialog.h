#ifndef EDITCARDDIALOG_H
#define EDITCARDDIALOG_H

#include "cardeditorwidget.h"
#include "utils.h"

#include <QDialog>
#include <QJsonDocument>
#include <QString>

namespace Ui {
class EditCardDialog;
}

class EditCardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCardDialog(QWidget *parent = 0);
    void setData(const QString &name, CardType type, const QJsonDocument &data);
    QString getCurrentName();
    CardType getCurrentType();
    QJsonDocument getCurrentData();
    ~EditCardDialog();

private slots:
    void on_cmb_Type_currentIndexChanged(int index);

    void on_buttonBox_helpRequested();

private:
    Ui::EditCardDialog *ui;
    CardEditorWidget *currentEditorWidget = NULL;
    QJsonDocument currentInputData;

    void placeEditorWidget();
};

#endif // EDITCARDDIALOG_H
