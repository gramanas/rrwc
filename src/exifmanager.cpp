#include <QDir>

#include "exifmanager.hpp"

ExifManager::ExifManager(const QString &dirPath)
    :m_dirPath(dirPath + QDir::separator()) {
}

DateTime ExifManager::getDateTime(const QString &fullPath) {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fullPath.toStdString());
    image->readMetadata();
    QString str;
    Exiv2::ExifData &exifData = image->exifData();
    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        const QString key = QString::fromStdString(i->key());
        if (key.compare(QString("Exif.Image.DateTime")) == 0) {
            str =  QString::fromStdString(i->value().toString());
            break;
        }
    }

    QStringList data = str.split(" ");
    QStringList date = data[0].split(":");
    QStringList time = data[1].split(":");
    DateTime dateTime;
    dateTime.date = QDate(date[0].toInt(), date[1].toInt(), date[2].toInt());
    dateTime.time = QTime(time[0].toInt(), time[1].toInt());
    return dateTime;
}

void ExifManager::sortByDateTime(QStringList &list) {
    std::sort(list.begin(), list.end(), [=](QString a, QString b) {
            DateTime dateTimeA = getDateTime(m_dirPath + a);
            DateTime dateTimeB = getDateTime(m_dirPath + b);

            if (dateTimeA.date < dateTimeB.date) {
                return true;
            } else if (dateTimeA.date > dateTimeB.date) {
                return false;
            } else {
                if (dateTimeA.time < dateTimeB.time) {
                    return true;
                } else {
                    return false;
                }
            }
        });
}
