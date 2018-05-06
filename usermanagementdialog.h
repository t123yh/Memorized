#ifndef USERMANAGEMENTDIALOG_H
#define USERMANAGEMENTDIALOG_H

#include <QDialog>

namespace Ui {
class UserManagementDialog;
}

class UserManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagementDialog(QWidget *parent = 0);
    ~UserManagementDialog();

private slots:
    void on_btn_Add_clicked();

private:
    Ui::UserManagementDialog *ui;
};

#endif // USERMANAGEMENTDIALOG_H
