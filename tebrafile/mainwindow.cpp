#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
}


void MainWindow::ftpDone(bool error)
{
    if (error) {
        std::cerr << "Error: " << qPrintable(ftpClient->errorString()) << std::endl;
        ui->loginMsg->setText(ftpClient->errorString());
        ui->loginMsg->setStyleSheet("QLabel {color : red}");
        ftpClient->disconnect();
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addToList(const QUrlInfo& file)
{
    fileList.push_back(file.name());
    qDebug() << file.name();
}

void MainWindow::on_connectButton_clicked()
{

    ftpAdrress = ui->serverNameField->text();
    connectToServer();
}

void MainWindow::login(InputDialog* diag)
{
    QStringList credentials = InputDialog::getStrings(diag);
    username = credentials.at(0);
    password = credentials.at(1);
    ftpClient->login(username, password);
    QObject::connect(ftpClient, &QFtp::stateChanged, this, &MainWindow::afterLogin);
}

void MainWindow::connectToServer()
{
        ftpClient = new QFtp(this);
        url = QUrl(ftpAdrress);
        if (url.isValid()) {
            ftpClient->connectToHost(url.host(), url.port(21));
            QObject::connect(ftpClient, &QFtp::done, this, &MainWindow::ftpDone);
        } else
            qDebug() << "greska ";

       QObject::connect(ftpClient, &QFtp::stateChanged, this, &MainWindow::showLoginDialog);
}

void MainWindow::on_disconnectButton_clicked()
{
    ftpClient->close();
    ftpClient->abort();
    ui->loginMsg->setText("You are disconnected");
}

void MainWindow::showLoginDialog(int state)
{
    if (state == QFtp::Connected and ftpClient->error() == QFtp::NoError) {
        InputDialog* diag = new InputDialog(this,
                                            QString("username"),
                                            QString("password"));
        QObject::connect(diag, &InputDialog::credentialsCaptured, this, &MainWindow::login);
        diag->exec();
    } else if (state == QFtp::Unconnected and ftpClient->currentCommand() != QFtp::Close){
        std::cerr << "ELSE ERROR: " << ftpClient->error() << std::endl;
        QMessageBox errBox;
        errBox.setWindowTitle("Connection error");
        errBox.setText(ftpClient->errorString());
        errBox.setIcon(QMessageBox::Critical);
        errBox.exec();
        ftpClient->close();
        ftpClient->disconnect();
    }

}

void MainWindow::afterLogin(int state)
{
    if (state == QFtp::LoggedIn and ftpClient->currentCommand() == QFtp::Login) {
        QObject::connect(ftpClient, &QFtp::listInfo, this, &MainWindow::addToList);
        ftpClient->list("./");
        ui->loginMsg->clear();
    }
}
