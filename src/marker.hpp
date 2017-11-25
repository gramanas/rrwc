#ifndef MARKER_HPP
#define MARKER_HPP

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Marker {
  public:
    Marker(cv::Mat &source, cv::Mat &watermark, int opacity);
    ~Marker();

    void exec(cv::Mat &destination);

  private:
    bool isRotated();

    // code from: https://jepsonsblog.blogspot.gr/2012/10/overlay-transparent-image-in-opencv.html
    void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                      cv::Mat &output, cv::Point2i location);
    float m_opacity;
    cv::Mat m_source;
    cv::Mat m_watermark;
};

#endif // MARKER_HPP
