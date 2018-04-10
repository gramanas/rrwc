#include <QDebug>

#include "globals.hpp"
#include "output/outputmanager.hpp"
#include "profile/profileparser.hpp"
#include "exif/exifmanager.hpp"


#include "ui_outputtab.h"

OutputManager::OutputManager(Logger *logger)
  : p_logger(logger),
    m_entryList(logger) {
  connect(&m_entryList, SIGNAL(finished()),
          this, SLOT(slotEntryListFull()));
}

void OutputManager::saveProfile(const QString &filename) {
  ProfileParser profileParser(m_outputs, p_logger);
  profileParser.writeToFile(filename);
}

bool OutputManager::loadProfile(const QString &filename) {
  ProfileParser profileParser(p_logger);
  m_outputs.clear();
  return profileParser.readFromFile(filename, m_outputs);
}

void OutputManager::print() const {
  qDebug() << "Outputs:";
  for (const auto& output : m_outputs) {
    qDebug() << "----Output----";
    qDebug() << "index=" << output->index;
    qDebug() << "folder=" << output->folder;
    qDebug() << "resize=" << output->resize;
    qDebug() << "rename=" << output->rename;
    qDebug() << "watermark=" << output->watermark;
    qDebug() << "length=" << output->length;
    qDebug() << "height=" << output->height;
    qDebug() << "rename=" << output->renameText;
    qDebug() << "watermark=" << output->watermarkText;
    qDebug() << "opacity=" << output->opacity;
    qDebug() << "start=" << output->counter.start;
    qDebug() << "step=" << output->counter.step;
    qDebug() << "digits=" << output->counter.digits;
    qDebug() << "stripMetadata=" << output->stripMetadata;
    qDebug() << "\n";
  }
}

void OutputManager::fillEntryList(const QString &inputPath, const QString &sort) {
  m_entryList.setDir(inputPath, sort);
  m_entryList.start();
}

void OutputManager::startOutputs(const int &threadNumber) {
  p_threadManager = new ThreadManager(m_entryList, m_outputs, threadNumber, p_logger);
  // set the total number of items, so progess bar can work
  p_logger->setTotal(m_entryList.get()->count());
  connect(p_threadManager, SIGNAL(done()), this, SLOT(onDone()));
  p_threadManager->startThreads();
}

void OutputManager::slotEntryListFull() {
  emit entryListFull();
}

void OutputManager::onDone() {
  emit done();
}

void OutputManager::clean() {
  for (int i = 0; i < m_outputs.size(); i++) {
    delete m_outputs[i];
  }
  m_outputs.clear();
  m_entryList.clear();
  delete p_threadManager;
}

void OutputManager::generateOutputsFromTabs(QVector<OutputTab *> outputTabs) {
  m_outputs.clear();
  Output *p_output;
  int i = 0;
  for (const auto& tab : outputTabs) {
    p_output = new Output;
    p_output->folder = tab->getUi()->inputOutputFolder->text();
    p_output->resize = tab->getUi()->resize->isChecked();
    p_output->rename = tab->getUi()->rename->isChecked();
    p_output->watermark = tab->getUi()->watermark->isChecked();
    p_output->length = tab->getUi()->inputLength->value();
    p_output->height = tab->getUi()->inputHeight->value();
    p_output->renameText = tab->getUi()->inputRename->text();
    p_output->watermarkText = tab->getUi()->inputWatermark->text();
    p_output->opacity = tab->getUi()->inputOpacity->value();
    p_output->counter.start = tab->getUi()->inputCounterStart->value();
    p_output->counter.step = tab->getUi()->inputCounterStep->value();
    p_output->counter.digits = tab->getUi()->inputCounterDigits->value();
    p_output->stripMetadata = tab->getUi()->stripMetadata->isChecked();
    p_output->comment = tab->getUi()->comment->isChecked();
    p_output->commentText = tab->getUi()->inputComment->text();
    p_output->index = i++;
    m_outputs.append(p_output);
  }
}

OutputManager::~OutputManager() {
  clean();
}
