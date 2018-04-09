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
#include "logger.hpp"


class EntryList : public QThread {
  Q_OBJECT
public:
  EntryList(Logger *logger);
  ~EntryList();

  void setDir(const QString &path,
              const QString &sort);
  void clear();
  const QVector<QString> *get() const {
    return &m_entryList;
  }

  void run() override;

private:
  void fillEntryList();
  void applyFilters();

  Logger *p_logger;
  QVector<QString> m_entryList;
  QDir m_dir;
  QString m_sort;
  QTime m_time;
};

#endif // ENTRYLIST_HPP
