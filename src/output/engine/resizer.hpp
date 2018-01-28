#ifndef RESIZER_HPP
#define RESIZER_HPP

#include <opencv2/core/core.hpp>

#include "output/engine/worker.hpp"

class Resizer : public Worker {
  public:
    Resizer();
    ~Resizer();

    void loadData(int length, int height);
    bool exec(cv::Mat &destination);

  private:
    // if there are more rows than collumns we have a portrait
    bool isPortrait();

    int m_length;
    int m_height;
};

#endif
