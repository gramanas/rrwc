#ifndef EXIFMANAGER_HPP
#define EXIFMANAGER_HPP

#include <QStringList>
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
    void sortByDateTime(QStringList &list);
    bool copyMetadata(const QString &from, const QString &to);
    DateTime getDateTime(const QString &fullPath);

  private:
};

#endif // EXIFMANAGER_HPP
