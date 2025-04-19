// FrameProcessing.cpp
#include "FrameProcessing.hpp"


std::vector<uint32_t> processFrame(
    const cv::Mat& frame, int width, int height, int rectFixedSpace,
    int nLedsTop, int nLedsBottom, int nLedsLeft, int nLedsRight, LedStrip& strip
) {
    cv::Rect topRect(0, 0, width, rectFixedSpace);
    cv::Rect bottomRect(0, height - rectFixedSpace, width, rectFixedSpace);
    cv::Rect leftRect(0, 0, rectFixedSpace, height);
    cv::Rect rightRect(width - rectFixedSpace, 0, rectFixedSpace, height);

    int kernel_size = 50;
    cv::Mat topBlur, bottomBlur, leftBlur, rightBlur;
    cv::blur(frame(topRect), topBlur, cv::Size(kernel_size, kernel_size));
    cv::blur(frame(bottomRect), bottomBlur, cv::Size(kernel_size, kernel_size));
    cv::blur(frame(leftRect), leftBlur, cv::Size(kernel_size, kernel_size));
    cv::blur(frame(rightRect), rightBlur, cv::Size(kernel_size, kernel_size));

    std::vector<uint32_t> pixels;

    // Top LEDs
    for (int i = 0; i < nLedsTop; ++i) {
        cv::Vec3b color = topBlur.at<cv::Vec3b>(rectFixedSpace / 2, (width / nLedsTop) * i + (width / (2 * nLedsTop)));
        pixels.push_back(strip.defineColor(color[2], color[1], color[0]));
    }
    // Reverse top LEDs
    std::reverse(pixels.begin(), pixels.begin() + nLedsTop);

    // Left LEDs
    for (int i = 0; i < nLedsLeft; ++i) {
        cv::Vec3b color = leftBlur.at<cv::Vec3b>((height - 1) - (height / nLedsLeft) * i - (height / (2 * nLedsLeft)), rectFixedSpace / 2);
        pixels.push_back(strip.defineColor(color[2], color[1], color[0]));
    }
    // Reverse left LEDs
    std::reverse(pixels.begin() + nLedsTop, pixels.begin() + nLedsTop + nLedsLeft);

    // Bottom LEDs
    for (int i = 0; i < nLedsBottom; ++i) {
        cv::Vec3b color = bottomBlur.at<cv::Vec3b>(rectFixedSpace / 2, (width / nLedsBottom) * i + (width / (2 * nLedsBottom)));
        pixels.push_back(strip.defineColor(color[2], color[1], color[0]));
    }

    // Right LEDs
    for (int i = 0; i < nLedsRight; ++i) {
        cv::Vec3b color = rightBlur.at<cv::Vec3b>((height / nLedsRight) * i + (height / (2 * nLedsRight)), rectFixedSpace / 2);
        pixels.push_back(strip.defineColor(color[2], color[1], color[0]));
    }
    // Invert right LEDs
    std::reverse(pixels.end() - nLedsRight, pixels.end());

    return pixels;
}
