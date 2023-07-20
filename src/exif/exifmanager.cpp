#include <QDir>
#include <QDebug>
#include <memory>

#include "exif/exifmanager.hpp"
#include "globals.hpp"

ExifManager::ExifManager(Logger *logger)
  : p_logger(logger) {
}

DateTime ExifManager::getDateTime(const QString &fullPath) {
  DateTime dateTime;
  std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(fullPath.toStdString());
  if (image == 0) {
    p_logger->err("Exif can't be determined for image: " + fullPath + " it might be corrupted.");
    dateTime.date = QDate(0, 0, 0);
    dateTime.time = QTime(-1, -1, -1);
    return dateTime;
  }
  image->readMetadata();
    QString str = "";
  Exiv2::ExifData &exifData = image->exifData();
  Exiv2::ExifData::const_iterator end = exifData.end();
  for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
    const QString key = QString::fromStdString(i->key());
    if (key.compare(QString("Exif.Photo.DateTimeOriginal")) == 0) {
      str =  QString::fromStdString(i->value().toString());
      break;
    }
  }
  if (str == "") {
    p_logger->err("Exif date not found for: " + fullPath + " it might be corrupted.");
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

void ExifManager::sortByDateTime(QVector<QString> &list) {
  QHash<QString, DateTime> hash;
  hash.reserve(list.count());
  p_logger->setTotal(list.count());
  for (auto const& key : list) {
    // reading the exif takes time,
    // this is what we show as progress while sorting
    // The actual sorting will never take a lot of time
    // compared to this, so it's redundant
    p_logger->incrementItemsDone();
    DateTime dt = getDateTime(key);
    if (dt.date == QDate(0, 0, 0))
      continue;
    hash.insert(key, dt);
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

  // reset progress
  p_logger->setItemsDone(0);
}

void ExifManager::updateComment(const QString &path, const QString &str) {
  std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(path.toStdString());
  image->readMetadata();
  Exiv2::ExifData data = image->exifData();
  data["Exif.Photo.UserComment"] = str.toStdString();
  image->setExifData(data);
  image->writeMetadata();
}

void ExifManager::copyMetadata(const QString &from, const QString &to) {
  // open file
  std::unique_ptr<Exiv2::Image> imageFrom = Exiv2::ImageFactory::open(from.toStdString());
  imageFrom->readMetadata();

  Exiv2::ExifData &dataToCopy = imageFrom->exifData();
  if (dataToCopy.empty()) {
    p_logger->err(WAR_IMAGE.arg(from.split(QDir::separator()).back()) + WAR_EXIF_NOT_FOUD);
    return;
  }

  std::unique_ptr<Exiv2::Image> imageTo = Exiv2::ImageFactory::open(to.toStdString());
  imageTo->readMetadata();

  // imwrite saves the image rotated, with the orientation to 0
  // if we copy the old orientation, the new image will turn out to be
  // rotated, so we set it to the new one.
  // if the image was copied without OpenCV libraries, we don't change anything
  dataToCopy["Exif.Image.Orientation"] = imageTo->exifData()["Exif.Image.Orientation"];

  imageTo->setExifData(dataToCopy);
  imageTo->writeMetadata();
}

void ExifManager::stripMetadata(const QString &path) {
  std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(path.toStdString());
  Exiv2::ExifData emptyData;

  image->setExifData(emptyData);
  image->writeMetadata();
}

ExifManager::~ExifManager() {
}

