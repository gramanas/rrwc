#include "output/engine/resizer.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

Resizer::Resizer() {
}

void Resizer::loadData(int length, int height) {
    m_length = length;
    m_height = height;
 }

bool Resizer::exec(cv::Mat &destination) {
    cv::Size size;
    isRotated() ? size = cv::Size(m_height, m_length) : size = cv::Size(m_length, m_height);

    cv::resize(*p_source, destination, size);
    if (destination.empty()) {
        return false;
    }
    return true;
}

Resizer::~Resizer() {
}
