#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btn_Cards_clicked();

    void on_btn_Review_clicked();

    void on_btn_Users_clicked();

private:
    Ui::MainWindow *ui;

    void refreshUser();
};

#endif // MAINWINDOW_H
