#include <QDir>
#include <QDebug>

#include "exifmanager.hpp"

ExifManager::ExifManager(const QString &dirPath)
    :m_dirPath(dirPath + QDir::separator()) {
}

ExifManager::ExifManager()
    :m_dirPath("") {
}

DateTime ExifManager::getDateTime(const QString &fullPath) {
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fullPath.toStdString());
    image->readMetadata();
    QString str = "";
    Exiv2::ExifData &exifData = image->exifData();
    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        const QString key = QString::fromStdString(i->key());
        if (key.compare(QString("Exif.Image.DateTime")) == 0) {
            str =  QString::fromStdString(i->value().toString());
            break;
        }
    }
    DateTime dateTime;
    if (str == "") {
        // set invalid date and time
        dateTime.date = QDate(-1, -1, -1);
        dateTime.time = QTime(-1, -1, -1);
    } else {
        QStringList data = str.split(" ");
        QStringList date = data[0].split(":");
        QStringList time = data[1].split(":");
        dateTime.date = QDate(date[0].toInt(), date[1].toInt(), date[2].toInt());
        dateTime.time = QTime(time[0].toInt(), time[1].toInt(), time[2].toInt());
    }
    return dateTime;
}

void ExifManager::sortByDateTime(QStringList &list) {
    std::sort(list.begin(), list.end(), [=](QString a, QString b) {
            DateTime dateTimeA = getDateTime(m_dirPath + a);
            DateTime dateTimeB = getDateTime(m_dirPath + b);

            if (!dateTimeA.date.isValid() || !dateTimeB.date.isValid()) {
                // compare names if no exif data is avaliable;
                if (a < b) {
                    return true;
                } else {
                    return false;
                }
            }
            if (dateTimeA.date < dateTimeB.date) {
                return true;
            } else if (dateTimeA.date > dateTimeB.date) {
                return false;
            } else {
                if (dateTimeA.time < dateTimeB.time) {
                    return true;
                }  else {
                    return false;
                }
            }
        });
}

bool ExifManager::copyMetadata(const QString &from, const QString &to) {
    Exiv2::Image::AutoPtr imageFrom = Exiv2::ImageFactory::open(from.toStdString());
    imageFrom->readMetadata();
    Exiv2::ExifData &exifData = imageFrom->exifData();
    if (exifData.empty()) {
        return false;
    }
    Exiv2::Image::AutoPtr imageTo = Exiv2::ImageFactory::open(to.toStdString());
    imageTo->setExifData(exifData);
    imageTo->writeMetadata();
    return true;
}

ExifManager::~ExifManager() {
}

