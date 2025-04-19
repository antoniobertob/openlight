// main.cpp
#include <csignal>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "LedStrip.hpp"
#include "FrameProcessing.hpp"
#include "Test.hpp"

#define N_LEDS_TOP 64
#define N_LEDS_BOTTOM 64
#define N_LEDS_LEFT 35
#define N_LEDS_RIGHT 34

#define TOTAL_LEDS (N_LEDS_TOP + N_LEDS_BOTTOM + N_LEDS_LEFT + N_LEDS_RIGHT)


LedStrip* globalStrip = nullptr;

void handleSignal(int signal) {
    if (signal == SIGINT && globalStrip != nullptr) {
        std::cout << "\n[INFO] Received signal SIGINT. Turining LEDs off..." << std::endl;
        globalStrip->turnOffLeds();
    }
    exit(0);
}

int main() {
    LedStrip strip(TOTAL_LEDS);

    globalStrip = &strip;
    signal(SIGINT, handleSignal);

    test(strip, TOTAL_LEDS);


    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "Error: Can't find the video source" << std::endl;
        return -1;
    }

    cap.set(cv::CAP_PROP_FPS, 24);

    cv::Mat frame;
    cap.read(frame);

    if (frame.empty()) {
        std::cerr << "Error: Can't get the initial frame." << std::endl;
        return -1;
    }

    int height = frame.rows;
    int width = frame.cols;
    int rectFixedSpace = static_cast<int>(height * 0.15);

    try {
        while (true) {
            cap.read(frame);

            if (frame.empty()) { // Can't get the frame
                std::cerr << "[INFO] Missing video signal. Turn off LEDs..." << std::endl;
                strip.turnOffLeds();
                usleep(500000);      // Wait half second before retry
                continue;
            }

            std::vector<uint32_t> ledStripe = processFrame(
                frame, width, height, rectFixedSpace,
                N_LEDS_TOP, N_LEDS_BOTTOM, N_LEDS_LEFT, N_LEDS_RIGHT, strip
            );

            strip.turnOnLeds(ledStripe);

            if (cv::waitKey(1) == 'q') {
                strip.turnOffLeds();
                break;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
