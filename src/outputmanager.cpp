#include <QDebug>

#include "outputmanager.hpp"
#include "ui_outputtab.h"

OutputManager::OutputManager() {
}

void OutputManager::generateOutputsFromTabs(QVector<OutputTab *> outputTabs) {
    m_outputs.clear();
    Output *p_output;
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

        m_outputs.append(p_output);
    }
}

void OutputManager::print() const {
    qDebug() << "Outputs:";
    for (const auto& output : m_outputs) {
        qDebug() << "----Output----";
        qDebug() << "folder: " << output->folder;
        qDebug() << "resize: " << output->resize;
        qDebug() << "rename: " << output->rename;
        qDebug() << "watermark: " << output->watermark;
        qDebug() << "length: " << output->length;
        qDebug() << "height: " << output->height;
        qDebug() << "rename: " << output->renameText;
        qDebug() << "watermark: " << output->watermarkText;
        qDebug() << "threads: " << output->threads;
        qDebug() << "opacity: " << output->opacity;
        qDebug() << "start: " << output->counter.start;
        qDebug() << "step: " << output->counter.step;
        qDebug() << "digits: " << output->counter.digits;
        qDebug() << "\n";
    }
}

OutputManager::~OutputManager() {
    for (auto output : m_outputs) {
        delete output;
    }
    m_outputs.clear();
}
