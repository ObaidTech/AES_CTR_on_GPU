#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_actionExit_triggered();

    void on_inputBtn_encrypt_clicked();

    void on_outputBtn_encrypt_clicked();

    void on_inputBtn_decrypt_clicked();

    void on_outputBtn_decrypt_clicked();

    void on_encryptBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
