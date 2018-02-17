#include <algorithm>
#include <QDebug>

#include "engine/imagebuffer.hpp"

ImageBuffer::ImageBuffer(const QStringList &entryList, int ountputNumber)
    : m_entryList(entryList) {
    // reserve enough for all images
    // since they are pointers, memory consumption is
    // of no issue
    m_buffer.reserve(m_entryList.size());

    // init current
    m_currentForOutput.reserve(ountputNumber);
    for (auto it : m_currentForOutput) {
        it = 0;
    }

    m_blockSize = std::ceil(m_entryList.size() / 50.0); // why 50
    qDebug() << "Block size:" << m_blockSize;
    loadNextBlock();
    for (int i = 0; i <= m_blockSize; i++) {
        m_buffer[i] = load(m_bufferHead);
        m_bufferHead++;
    }
}

ImageBuffer::~ImageBuffer() {
    m_buffer.empty();
    m_currentForOutput.empty();
}

const cv::Mat * ImageBuffer::getNext(int outputIndex) {
    // get the text image of the caller's output index
    return m_buffer[m_currentForOutput[outputIndex]].get();

    // update the caller's current image
    m_currentForOutput[outputIndex]++;
    releaseOld();
}

void ImageBuffer::loadNextBlock() {
    if (m_bufferHead + m_blockSize <= m_entryList.size()) {
        for (int i = 0; i < m_blockSize; i++) {
            m_buffer[i] = load(m_bufferHead);
            m_bufferHead++;
        }
    }
    // the final block
    else {
        while (m_bufferHead <= m_entryList.size()) {
            m_buffer[m_bufferHead] = load(m_bufferHead);
            m_bufferHead++;
        }
    }
}

ImagePointer ImageBuffer::load(int i) {
    ImagePointer im(new cv::Mat);
    *im = cv::imread(m_entryList[i].toStdString());
    return std::move(im);
}

void ImageBuffer::releaseOld() {
    int min = *std::min_element(m_currentForOutput.constBegin(),
                                m_currentForOutput.constEnd());

    if (min > m_lastUsedImage) {
        // loop until min, but not with min cause it might still be used by a thread
        for (int i = 0; i < min; i++) {
            if (m_buffer.value(i)) {
                m_buffer.value(i).reset(nullptr);
            }
        }
        m_lastUsedImage = min;
    }
}
