#ifndef WORKER_HPP
#define WORKER_HPP

 /////////////////////////////////////
 // Common ansestor for the workers //
 /////////////////////////////////////

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Worker {
  public:
    void activate() {
        m_active = true;
    }

    bool isOn() {
        return m_active;
    }

    void loadSource(cv::Mat *source) {
        p_source = source;
    }


  protected:
    // TODO add exif!
    bool isRotated() {
        if (p_source->rows > p_source->cols) {
            return true;
        }
        return false;
    }

    cv::Mat *p_source;

  private:
    bool m_active = false;
};

#endif // WORKER_HPP
