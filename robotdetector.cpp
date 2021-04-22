#include "robotdetector.h"

RobotDetector::RobotDetector()
{
    isHSVFrameAvailable = false;
}
void RobotDetector::startDetecting(){
    using namespace cv;
    stoped = false;
    int blueLowH = 90, blueHighH = 140, blueLowS = 50, blueHighS = 255, blueLowV = 50, blueHighV = 255;
    int redLowH = 150, redHighH = 180, redLowS = 50, redHighS = 255, redLowV = 50, redHighV = 255;
    while (!stoped) {
        if (isHSVFrameAvailable) {
                cv::Mat blueMask , redMask;
                cv::inRange(hsvFrame, cv::Scalar(blueLowH, blueLowS, blueLowV), cv::Scalar(blueHighH, blueHighS, blueHighV), blueMask); //Threshold the image
                cv::inRange(hsvFrame, cv::Scalar(redLowH, redLowS, redLowV), cv::Scalar(redHighH, redHighS, redHighV), redMask);
                std::vector<std::vector<cv::Point> > blueContours, redContours;
                std::vector<cv::Vec4i> blueHierarchy, redHierarchy;
                cv::findContours(blueMask, blueContours, blueHierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
                cv::findContours(redMask, redContours, redHierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
                size_t largestBlueContourIndex = 0;
                size_t largestRedContourIndex = 0;
                double blueMaxArea = 0;
                for (size_t i = 0; i < blueContours.size(); i++) {
                    double area = cv::contourArea(blueContours[i]);
                    if (area > blueMaxArea) {
                        largestBlueContourIndex = i;
                        blueMaxArea = area;
                    }
                }
                double redMaxArea = 0;
                for (size_t i = 0; i < redContours.size(); i++) {
                    double area = cv::contourArea(redContours[i]);
                    if (area > redMaxArea) {
                        largestRedContourIndex = i;
                        redMaxArea = area;
                    }
                }
                if (redMaxArea >= 20 && blueMaxArea >= 20) {
                    cv::Rect blueRect = cv::boundingRect(blueContours[largestBlueContourIndex]);
                    cv::Rect redRect = cv::boundingRect(redContours[largestRedContourIndex]);
                    int xRobot = std::min(blueRect.x, redRect.x), yRobot = std::min(blueRect.y, redRect.y);
                    int widthRobot = std::max(redRect.x + redRect.width, blueRect.x + blueRect.width) - xRobot;
                    int heightRobot = std::max(redRect.y + redRect.height, blueRect.y + blueRect.height) - yRobot;
                    cv::Rect robot(xRobot, yRobot, widthRobot, heightRobot);
                    emit detectingBox(robot);
                }
            }
    }
}

void RobotDetector::stopDetecting(){
    stoped = true;
}

void RobotDetector::getHSVFrame(cv::Mat newHSVFrame){
    hsvFrame = newHSVFrame;
    isHSVFrameAvailable = true;
}
