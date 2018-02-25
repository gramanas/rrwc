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
    bool enable(bool flag) {
        m_enabled = flag;
        return flag;
    }

    bool isEnabled() {
        return m_enabled;
    }

    void loadSource(cv::Mat *source) {
        p_source = source;
    }


  protected:
    cv::Mat *p_source;

  private:
    bool m_enabled = false;
};

#endif // WORKER_HPP
