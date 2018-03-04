#ifndef MARKER_HPP
#define MARKER_HPP

#include <QString>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "engine/worker.hpp"

class Marker : public Worker {
  public:
    Marker();
    ~Marker();

    void loadData(QString watermarkPath, int opacity);
    bool exec(cv::Mat &destination);

  private:
    // code from: https://jepsonsblog.blogspot.gr/2012/10/overlay-transparent-image-in-opencv.html
    void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                      cv::Mat &output, cv::Point2i location);

    float m_opacity;
    cv::Mat m_finalWatermark;
    cv::Mat m_watermark;
};

#endif // MARKER_HPP
