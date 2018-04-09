#ifndef EXIFMANAGER_HPP
#define EXIFMANAGER_HPP

#include <QVector>
#include <QDate>
#include <QTime>

#include "exiv2/exiv2.hpp"
#include "logger.hpp"

struct DateTime {
  QDate date;
  QTime time;
};

class ExifManager {
public:
  ExifManager(Logger *logger);
  ~ExifManager();
  void sortByDateTime(QVector<QString> &list);
  bool copyMetadata(const QString &from, const QString &to);

private:
  Logger *p_logger;
  DateTime getDateTime(const QString &fullPath);
};

#endif // EXIFMANAGER_HPP
