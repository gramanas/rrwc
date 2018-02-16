#include "engine/imagebuffer.hpp"
#include <algorithm>

ImageBuffer::ImageBuffer(QStringList entrylist) {
    m_buffer.reserve(entrylist.size());
    // init current
}

ImageBuffer::~ImageBuffer() {
}

const cv::Mat * ImageBuffer::getNext(int outputIndex) {
    // get the text image of the caller's output index
    return m_buffer[m_currentForOutput[outputIndex]].get();

    // update the caller's current image
    m_currentForOutput[outputIndex]++;
    releaseOld();
}

void ImageBuffer::releaseOld() {
    int min = *std::min_element(m_currentForOutput.constBegin(),
                                m_currentForOutput.constEnd());

    // loop until min, but not with min cause it might still be used by a thread
    for (int i = 0; i < min; i++) {
        if (m_buffer.value(i)) {
            m_buffer.value(i).reset(nullptr);
        }
    }
}
