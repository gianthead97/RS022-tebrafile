#ifndef SEARCH_H
#define SEARCH_H

#include "listFiles.h"

#include <QFtp>
#include <QSharedPointer>
#include <QTreeWidget>
#include <QHeaderView>
#include <QHash>
#include <iostream>

class Search : public QObject
{
    Q_OBJECT
public:
    Search(QSharedPointer<ListFiles> treeWidget,
           const QSharedPointer<QRegularExpression> filename,
           QString path, const QSharedPointer<QFtp>& client);
    ~Search(){

    }
    void search();
    void stopSearch();
    void addToList(const QUrlInfo& file);

private slots:
    void folderFinished(bool error);

signals:
    void searchFinished();

private:
    QSharedPointer<ListFiles> _treeWidget;
    QSharedPointer<QRegularExpression> _filename;
    QString _path;
    QSharedPointer<QFtp> _client;
    QVector<QString> _folders;
    bool _stop = false;
};


#endif // SEARCH_H
