#ifndef ENTRYLIST_HPP
#define ENTRYLIST_HPP

#include <QTime>
#include <memory>
#include <memory>
#include <QVector>
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
    const QVector<QString> *get() const {
        return &m_entryList;
    }

    void run() override;

  signals:
    void statusChanged(QString status);

  private:
    void fillEntryList();
    void applyFilters();

    QVector<QString> m_entryList;
    QDir m_dir;
    QString m_sort;
    QTime m_time;
};

#endif // ENTRYLIST_HPP
