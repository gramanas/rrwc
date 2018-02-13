#include <QDir>
#include <QDebug>

#include "exif/exifmanager.hpp"

ExifManager::ExifManager() {
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
    }
    else {
        QStringList data = str.split(" ");
        QStringList date = data[0].split(":");
        QStringList time = data[1].split(":");
        dateTime.date = QDate(date[0].toInt(), date[1].toInt(), date[2].toInt());
        dateTime.time = QTime(time[0].toInt(), time[1].toInt(), time[2].toInt());
    }
    return dateTime;
}

void ExifManager::sortByDateTime(QStringList &list) {
    QHash<QString, DateTime> hash;
    hash.reserve(list.count());
    for (auto const& key : list) {
        hash.insert(key, getDateTime(key));
    }

    std::sort(list.begin(), list.end(), [=](QString a, QString b) {
            DateTime dateTimeA = hash.value(a);
            DateTime dateTimeB = hash.value(b);

            // compare names if no exif data is avaliable;
            if (!dateTimeA.date.isValid()
                || !dateTimeB.date.isValid()) {
                return a < b ? true : false;
            }
            // most of the time the date in a set of photographs is the same while
            // the time is what determines the sorting, so we check for that first
            // to (hopefully) reduce the total amount of if statements
            if (dateTimeA.date == dateTimeB.date) {
                return dateTimeA.time < dateTimeB.time ? true : false;
            }
            // finally compare by date
            return dateTimeA.date < dateTimeB.date ? true : false;
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

