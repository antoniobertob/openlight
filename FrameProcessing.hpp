// FrameProcessing.hpp
#ifndef FRAMEPROCESSING_HPP
#define FRAMEPROCESSING_HPP

#include <vector>
#include <opencv2/opencv.hpp>
#include "LedStrip.hpp"


std::vector<uint32_t> processFrame(
    const cv::Mat& frame, int width, int height, int rectFixedSpace,
    int nLedsTop, int nLedsBottom, int nLedsLeft, int nLedsRight, LedStrip& strip
);

#endif // FRAMEPROCESSING_HPP
