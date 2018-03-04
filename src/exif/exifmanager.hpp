#ifndef EXIFMANAGER_HPP
#define EXIFMANAGER_HPP

#include <QVector>
#include <QDate>
#include <QTime>

#include "exiv2/exiv2.hpp"

struct DateTime {
    QDate date;
    QTime time;
};

class ExifManager {
  public:
    ExifManager();
    ~ExifManager();
    void sortByDateTime(QVector<QString> &list);
    bool copyMetadata(const QString &from, const QString &to);

  private:
    DateTime getDateTime(const QString &fullPath);
};

#endif // EXIFMANAGER_HPP
