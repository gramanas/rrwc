#ifndef MERKER_HPP
#define MARKER_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Marker {
  public:
    Marker(cv::Mat &source, cv::Mat &watermark);
};

#endif // MARKER_HPP
