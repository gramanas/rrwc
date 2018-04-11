#include <QDir>
#include <QFile>
#include "exif/exifmanager.hpp"
#include "engine/outputengine.hpp"
#include <QDebug>

OutputEngine::OutputEngine(const QVector<Output *> &outputs,
                           const int &current,
                           Logger *logger)
  : m_current(current-1),
    p_logger(logger) {
  m_gears.reserve(outputs.size());

  for (const auto &it : outputs) {
    int i = it->index;
    m_gears[i] = new Gears;
    if (m_gears[i]->resizer.enable(it->resize)) {
      m_gears[i]->resizer.loadData(it->length, it->height);
    }
    if (m_gears[i]->renamer.enable(it->rename)) {
      m_gears[i]->renamer.loadData(it->renameText, it->counter);
    }
    if (m_gears[i]->marker.enable(it->watermark)) {
      m_gears[i]->marker.loadData(it->watermarkText, it->opacity);
    }
    m_gears[i]->copyFlag = !it->resize && !it->watermark ? true : false;
    m_gears[i]->stripMetadata = it->stripMetadata;
    m_gears[i]->p_output = it;
  }
    }

void OutputEngine::setCurrentOutput(int index) {
  m_gear = m_gears[index];
}

OutputEngine::~OutputEngine() {
  for (auto it : m_gears) {
    delete it;
  }
  m_gears.clear();
}

bool OutputEngine::loadImage(const QString &path) {
  m_current++;
  m_sourceInfo = QFileInfo(path);
  if (!m_sourceInfo.exists()) {
    // LOG
    return false;
  }
  m_source = cv::imread(path.toStdString());
  if (m_source.empty()) {
    // LOG
    return false;
  }
  return true;
}

bool OutputEngine::exec() {
  m_source.copyTo(m_out);
  if (m_gear->renamer.isEnabled()) {
    m_gear->renamer.loadNext(m_sourceInfo.baseName(), m_current);
    m_gear->renamer.exec(m_newName);
  }
  else {
    m_newName = m_sourceInfo.baseName();
  }
  if (m_gear->resizer.isEnabled()) {
    m_gear->resizer.loadSource(&m_out);
    if (!m_gear->resizer.exec(m_out)) {
      // LOG
    }
  }

  if (m_gear->marker.isEnabled()) {
    m_gear->marker.loadSource(&m_out);
    if (!m_gear->marker.exec(m_out)) {
      p_logger->err(ERR_OTP.arg(m_gear->p_output->index + 1)
                    + " "
                    + ERR_IMAGE.arg(m_sourceInfo.baseName())
                    + ERR_WATERMARK_FIT);
    }
  }

  return true;
}

void OutputEngine::handleExif(const QString &path) {
  ExifManager exifs = ExifManager(p_logger);
  if (m_gear->stripMetadata) {
    if (m_gear->copyFlag) {
      exifs.stripMetadata(path);
    }
    // if copyFlag is not set the image gets saved by
    // cv::imwrite() which does not preserve exif info
  }
  else {
    // if we want to keep metadata, but don't have copyFlag
    // enabled, we have to copy the metadata from source to
    // file
    if (!m_gear->copyFlag) {
      exifs.copyMetadata(m_sourceInfo.absoluteFilePath(), path);
    }
  }
  if (m_gear->p_output->comment) {
    exifs.updateComment(path, m_gear->p_output->commentText);
  }
}

bool OutputEngine::write() {
  QString fullName = m_gear->p_output->folder
    + QDir::separator()
    + m_newName
    + "." + m_sourceInfo.suffix();
  if (m_gear->copyFlag) {
    QFile::copy(m_sourceInfo.absoluteFilePath(), fullName);
  }
  else {
    if (!cv::imwrite(fullName.toStdString(), m_out)) {
      p_logger->err(ERR_FAIL.arg(m_gear->p_output->index + 1).arg(m_sourceInfo.baseName()));
      return false;
    }
  }
  handleExif(fullName);
  return true;
}
