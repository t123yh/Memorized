#ifndef CARDSDIALOG_H
#define CARDSDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class CardsDialog;
}

class CardsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardsDialog(QWidget *parent = 0);
    ~CardsDialog();

private slots:
    void on_cmb_CardGroups_currentIndexChanged(int index);

    void on_btn_AddCard_clicked();

    void on_btn_RemoveCard_clicked();

    void on_btn_EditCard_clicked();

    void on_btn_AddGroup_clicked();

    void on_btn_RemoveGroup_clicked();

private:
    Ui::CardsDialog *ui;

    void initCardGroupsGroupbox();
    void showCards();
    void initializeModels();

    QSqlRelationalTableModel *cardsModel;
    QSqlTableModel *cardGroupsModel;

    int getCurrentCardId();
    int getCurrentGroupId();
};

#endif // CARDSDIALOG_H
