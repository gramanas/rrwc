#ifndef ENTRYLIST_HPP
#define ENTRYLIST_HPP

#include <QTime>
#include <QDir>
#include <QDirIterator>
#include <QThread>

#include "globals.hpp"

class EntryList : public QThread {
    Q_OBJECT
  public:
    EntryList();
    ~EntryList();

    void setDir(const QString &path,
                const QString &sort);
    void clear();
    const QStringList* get() {
        return &m_entryList;
    }

    void run() override;

  signals:
    void progressChanged(int progress);
    void statusChanged(QString status);
    void writeLog(QString log, QString str);

  private:
    void fillEntryList();
    void applyFilters();

    QStringList m_entryList;
    QDir m_dir;
    QString m_sort;
    QTime m_time;
};

#endif // ENTRYLIST_HPP
