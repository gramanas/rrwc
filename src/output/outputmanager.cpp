#include <QDebug>

#include "globals.hpp"
#include "output/outputmanager.hpp"
#include "profile/profileparser.hpp"
#include "exif/exifmanager.hpp"

#include "ui_outputtab.h"

OutputManager::OutputManager() {
    connect(&m_entryList, SIGNAL(statusChanged(QString)),
            this, SLOT(onStatusChanged(QString)));
    connect(&m_entryList, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(&m_entryList, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    connect(&m_entryList, SIGNAL(finished()),
            this, SLOT(slotEntryListFull()));
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
        p_output->threads = tab->getUi()->inputThreads->value();
        p_output->opacity = tab->getUi()->inputOpacity->value();
        p_output->counter.start = tab->getUi()->inputCounterStart->value();
        p_output->counter.step = tab->getUi()->inputCounterStep->value();
        p_output->counter.digits = tab->getUi()->inputCounterDigits->value();
        p_output->stripMetadata = tab->getUi()->stripExifData->isChecked();
        p_output->index = i++;
        m_outputs.append(p_output);
    }

    // update the remaining outputs
    m_outputsRemaining = outputTabs.size();
}

void OutputManager::saveProfile(const QString &filename) {
    ProfileParser profileParser(m_outputs);
    profileParser.writeToFile(filename);
}

bool OutputManager::loadProfile(const QString &filename) {
    ProfileParser profileParser;
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
        qDebug() << "threads=" << output->threads;
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

void OutputManager::startOutput(int output) {
    // Lazy initialization
    if (m_engines.isEmpty()) {
        m_engines.reserve(m_outputs.size());
        m_outputProgress.reserve(m_outputs.size());
    }

    // One engine manager for each output
    m_engines.insert(output, new EngineManager(m_outputs[output], *m_entryList.get(), output));
    m_outputProgress.insert(output, 0);

    // connect the signals
    connect(m_engines[output], SIGNAL(progressChanged(int, int)),
            this, SLOT(onProgressChanged(int, int)));
    connect(m_engines[output], SIGNAL(done()),
            this, SLOT(onDone()));
    connect(m_engines[output], SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));

    m_engines[output]->startThreads();
}

void OutputManager::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}

void OutputManager::onProgressChanged(int output, int progress) {
    m_outputProgress[output] = progress;

    int sum = 0;
    for (int i = 0; i < m_outputProgress.size(); i++) {
        sum += m_outputProgress[i];
    }
    // emit the average
    emit progressChanged(int(float(sum)/float(m_outputProgress.size())));
}

void OutputManager::onProgressChanged(int progress) {
    emit progressChanged(progress);
}

void OutputManager::onStatusChanged(QString status) {
    emit statusChanged(status);
}

void OutputManager::slotEntryListFull() {
    emit entryListFull();
}

void OutputManager::onDone() {
    m_outputsRemaining--;
    if (m_outputsRemaining == 0) {
        emit done();
    }
}

void OutputManager::clean() {
    for (int i = 0; i < m_outputs.size(); i++) {
        delete m_outputs[i];
        delete m_engines[i];
    }
    m_outputs.clear();
    m_engines.clear();
    m_outputProgress.clear();
    m_entryList.clear();
}

OutputManager::~OutputManager() {
    clean();
}
