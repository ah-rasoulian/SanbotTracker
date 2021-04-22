#include "pathcapture.h"

PathCapture::PathCapture()
{
    pathSize = 0;
    stoped = false;
}

void PathCapture::determinePath() {
    using namespace cv;
    cv::Point p(static_cast<int>(robotBox.x) + static_cast<int>(robotBox.width / 2), static_cast<int>(robotBox.y) + static_cast<int>(robotBox.height / 2));
    path[pathSize] = p;
    pathSize++;
}

void PathCapture::recieveBox(cv::Rect newRobotBox){
    robotBox = newRobotBox;
    determinePath();
}

void PathCapture::showResult(cv::Mat frame){
    using namespace cv;
    cv::Mat lastFrame = frame.clone();
    // displaying
    for (int i = 1; i < pathSize - 1; i++) {
                cv::line(lastFrame, path[i], path[i + 1], cv::Scalar(0, 0, 255), 2);
    }
    pathSize = 0; 
    emit resultDisplay( QPixmap::fromImage(
                QImage(static_cast<uchar*>(lastFrame.data), lastFrame.cols, lastFrame.rows, static_cast<int>(lastFrame.step), QImage::Format_RGB888)
                .rgbSwapped()
                    )
            );
}

