#include "resizer.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

Resizer::Resizer(cv::Mat &source, int length, int height)
    : m_source(source),
      m_length(length),
      m_height(height) {
}

bool Resizer::isRotated() {
    if (m_source.rows > m_source.cols)
        return true;
    return false;
}

void Resizer::exec(cv::Mat &destination) {
    cv::Size size;
    isRotated() ? size = cv::Size(m_height, m_length) : size = cv::Size(m_length, m_height);
    cv::resize(m_source, destination, size);
}

Resizer::~Resizer() {
}
