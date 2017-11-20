#ifndef RESIZER_HPP
#define RESIZER_HPP

#include <opencv2/core/core.hpp>

class Resizer {
  public:
    Resizer(cv::Mat &source, int length, int height);
    ~Resizer();

    void exec(cv::Mat &destination);

  private:
    bool isRotated();
    cv::Mat m_source;
    int m_length;
    int m_height;

};

#endif
