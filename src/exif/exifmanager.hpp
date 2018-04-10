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
  void copyMetadata(const QString &from, const QString &to);
  void stripMetadata(const QString &path);

private:
  DateTime getDateTime(const QString &fullPath);

  Logger *p_logger;
};

#endif // EXIFMANAGER_HPP
