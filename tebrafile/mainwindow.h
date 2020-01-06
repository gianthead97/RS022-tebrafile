#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serverconnection.h"

#include <iostream>

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFtp>
#include <QHash>
#include <QTreeWidget>
#include <QDir>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getFileList();


public slots:
    void addToList(const QUrlInfo& file);
    void afterLogin(int state);
    void initTreeWidget();
    void restartTreeWidget();
    void listDone(bool error);
    void listFiles(const QString& fileName);
    void cdToFolder(QTreeWidgetItem *widgetItem, int column);
    void leaveFolder();

    void downloadProgressBarSlot(qint64 done, qint64 total);

    void progressBarSlot(qint64 done, qint64 total);
    void uploadFinishHandler(int id, bool error);

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();

    void on_openButton_clicked();

    void on_uploadButton_clicked();
    void on_downloadButton_clicked();

    void on_treeWidget_clicked();

    void on_header_cliked(int logicalIndex);

private:
    Ui::MainWindow *ui;

    ServerConnection* serverConn;
    QFile *file;

    QString username = "default1";
    QString password = "DEFAULT1";


    QTreeWidget *fileList;
    QHash<QString, bool> isDir;
    QString currentPath;
    QString uploadFileName;
    QString downloadFilename;

    QHeaderView *headerView;

    bool logged = false;

};
#endif // MAINWINDOW_H
