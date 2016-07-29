#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include "CL/cl.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_inputBtn_encrypt_clicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this,"Select a file to encrypt", "input", "All Files (*.*)");
    ui->inputEncryptPath->setText(file1Name);
}

void MainWindow::on_outputBtn_encrypt_clicked()
{
    QFileDialog *pFile = new QFileDialog (this, "Select a destination to save output file", "output", ".aes");
    pFile->setAcceptMode(QFileDialog::AcceptSave);
    pFile->setWindowModality(Qt::WindowModal);
    if(pFile->exec())
    {
        ui->outputEncryptPath->setText(pFile->selectedFiles()[0]+".aes");
    }
}

void MainWindow::on_inputBtn_decrypt_clicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this,"Select a file to decrypt", "input", "All Files (*.*)");
    ui->inputDecryptPath->setText(file1Name);
}

void MainWindow::on_outputBtn_decrypt_clicked()
{
    QFileDialog *pFile = new QFileDialog (this, "Select a destination to save output file", "output", ".txt");
    pFile->setAcceptMode(QFileDialog::AcceptSave);
    pFile->setWindowModality(Qt::WindowModal);
    if(pFile->exec())
    {
        ui->outputDecryptPath->setText(pFile->selectedFiles()[0]+".txt");
    }
}

void MainWindow::on_encryptBtn_clicked()
{

}
