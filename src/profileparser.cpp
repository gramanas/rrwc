#include <QTextStream>
#include <QDebug>

#include "profileparser.hpp"
#include "globals.hpp"

ProfileParser::ProfileParser(const QVector<Output *> &outputs) {
    m_outputs.reserve(outputs.size());
    Output *p_output;
    for (const auto& output : outputs) {
        p_output = new Output;
        p_output->folder = output->folder;
        p_output->resize = output->resize;
        p_output->rename = output->rename;
        p_output->watermark = output->watermark;
        p_output->length = output->length;
        p_output->height = output->height;
        p_output->renameText = output->renameText;
        p_output->watermarkText = output->watermarkText;
        p_output->threads = output->threads;
        p_output->opacity = output->opacity;
        p_output->counter.start = output->counter.start;
        p_output->counter.step = output->counter.step;
        p_output->counter.digits = output->counter.digits;
        p_output->stripMetadata = output->stripMetadata;
        p_output->index = output->index;
        m_outputs << p_output;
    }
}

ProfileParser::ProfileParser() {
}

void ProfileParser::writeToFile(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

    QTextStream out(&file);
    for (const auto *output : m_outputs) {
        out << "Output {\n";
        out << OTP_FOLDER << "= \"" << output->folder << "\"" << "\n";
        out << OTP_RESIZE << "=" << (output->resize ? "true" : "false") << "\n";
        out << OTP_LENGTH << "=" << output->length << "\n";
        out << OTP_HEIGHT << "=" << output->height << "\n";
        out << OTP_RENAME << "=" << (output->rename ? "true" : "false") << "\n";
        out << OTP_RENAME_TEXT << "= \"" << output->renameText << "\"" << "\n";
        out << OTP_COUNTER_START << "=" << output->counter.start << "\n";
        out << OTP_COUNTER_STEP << "=" << output->counter.step << "\n";
        out << OTP_COUNTER_DIGITS << "=" << output->counter.digits << "\n";
        out << OTP_WATERMARK << "=" << (output->watermark ? "true" : "false") << "\n";
        out << OTP_WATERMARK_TEXT << "= \"" << output->watermarkText << "\"" << "\n";
        out << OTP_OPACITY << "=" << output->opacity << "\n";
        out << OTP_STRIP_METADATA << "=" << (output->stripMetadata ? "true" : "false") << "\n";
        out << OTP_THREADS << "=" << output->threads << "\n";
        out << "}\n";
    }
    file.close();
}

void ProfileParser::copyOutputsToVector(QVector<Output *> &vector) {
    Output *p_output;
    for (const auto& output : m_outputs) {
        p_output = new Output;
        p_output->folder = output->folder;
        p_output->resize = output->resize;
        p_output->rename = output->rename;
        p_output->watermark = output->watermark;
        p_output->length = output->length;
        p_output->height = output->height;
        p_output->renameText = output->renameText;
        p_output->watermarkText = output->watermarkText;
        p_output->threads = output->threads;
        p_output->opacity = output->opacity;
        p_output->counter.start = output->counter.start;
        p_output->counter.step = output->counter.step;
        p_output->counter.digits = output->counter.digits;
        p_output->stripMetadata = output->stripMetadata;
        p_output->index = output->index;
        vector << p_output;
    }
}

QStringList ProfileParser::stripWhitespaces(QStringList list) {
    QStringList ret;
    for (auto it : list) {
        ret << it.simplified();
    }
    return ret;
}

void ProfileParser::parse(int &output, QStringList &data) {
    output = data[1].toInt();
}

void ProfileParser::parse(QString &output, QStringList &data) {
    output = data[1].remove("\"");
}

void ProfileParser::parse(bool &output, QStringList &data) {
    data[1] == "true" ? output=true : output=false;
}

bool ProfileParser::readFromFile(const QString &filename, QVector<Output *> &vector) {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
        return false;
    }

    int index = 0;
    QTextStream in(&file);

    Output *p_output = nullptr;
    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.startsWith('#')) {
            continue;
        } else if (line.contains('{')) {
            if (p_output != nullptr) {
                qDebug() << "Error in profile" << filename;
                qDebug() << "Outputs must be enclosed in \"{}\"";
                return false;
            }
            p_output = new Output;
            p_output->index = index++;
        } else if (p_output != nullptr) {
            QStringList data = stripWhitespaces(line.split('='));
            if (line.contains('}')) {
                m_outputs << p_output;
                // reset
                p_output = nullptr;
            } else if (line.isEmpty()) {
                continue;
            } else if (data[0] == OTP_FOLDER) {
                parse(p_output->folder, data);
            } else if (data[0] == OTP_RESIZE) {
                parse(p_output->resize, data);
            } else if (data[0] == OTP_LENGTH) {
                parse(p_output->length, data);
            } else if (data[0] == OTP_HEIGHT) {
                parse(p_output->height, data);
            } else if (data[0] == OTP_RENAME) {
                parse(p_output->rename, data);
            } else if (data[0] == OTP_RENAME_TEXT) {
                parse(p_output->renameText, data);
            } else if (data[0] == OTP_COUNTER_START) {
                parse(p_output->counter.start, data);
            } else if (data[0] == OTP_COUNTER_STEP) {
                parse(p_output->counter.step, data);
            } else if (data[0] == OTP_COUNTER_DIGITS) {
                parse(p_output->counter.digits, data);
            } else if (data[0] == OTP_WATERMARK) {
                parse(p_output->watermark, data);
            } else if (data[0] == OTP_WATERMARK_TEXT) {
                parse(p_output->watermarkText, data);
            } else if (data[0] == OTP_OPACITY) {
                parse(p_output->opacity, data);
            } else if (data[0] == OTP_THREADS) {
                parse(p_output->threads, data);
            } else if (data[0] == OTP_STRIP_METADATA) {
                parse(p_output->stripMetadata, data);
            } else {
                qDebug() << "Error in profile" << filename;
                qDebug() << "Line" << line;
                return false;
            }
        }
    }
    file.close();
    copyOutputsToVector(vector);
    return true;
}

ProfileParser::~ProfileParser(){
    for (int i = 0; i < m_outputs.size(); i++) {
        delete m_outputs[i];
    }
    m_outputs.clear();
}
