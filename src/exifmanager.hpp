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
    void sortByDateTime(QStringList &list);

  private:
    DateTime getDateTime(const QString &fullPath);
    bool lessThan(const QString &item, const QString &meti);
    const QString m_dirPath;
};

#endif // EXIFMANAGER_HPP
