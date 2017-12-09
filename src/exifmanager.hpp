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
    ExifManager(const QString &dirPath);
    ExifManager();
    ~ExifManager();
    void sortByDateTime(QStringList &list);
    void copyMetadata(const QString &from, const QString &to);

  private:
    DateTime getDateTime(const QString &fullPath);
    const QString m_dirPath;
};

#endif // EXIFMANAGER_HPP
