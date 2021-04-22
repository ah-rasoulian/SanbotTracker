#include "obstaclesdetector.h"

ObstaclesDetector::ObstaclesDetector()
{
    isHSVFrameAvailable = false;
}

void ObstaclesDetector::startDetecting(){
    using namespace cv;
    stoped = false;
    int greenLowH = 50, greenHighH = 80, greenLowS = 50, greenHighS = 255, greenLowV = 50, greenHighV = 255;
    cv::Rect tempObstacles[20];
    while (!stoped) {
        if (isHSVFrameAvailable) {
            cv::Mat greenMask;
            int tempNumberOfObstacles = 0;
            cv::inRange(hsvFrame, cv::Scalar(greenLowH, greenLowS, greenLowV), cv::Scalar(greenHighH, greenHighS, greenHighV), greenMask);
            std::vector<std::vector<cv::Point> > greenContours;
            std::vector<cv::Vec4i> greenHierarchy;
            cv::findContours(greenMask, greenContours, greenHierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
            for (size_t i = 0; i < greenContours.size(); i++) {
                double area = cv::contourArea(greenContours[i]);
                if (area > 200) {
                    cv::Rect obsRect = cv::boundingRect(greenContours[i]);
                    tempObstacles[tempNumberOfObstacles] = obsRect;
                    tempNumberOfObstacles ++;
                }
            }
            emit detectingObstacles(tempObstacles, tempNumberOfObstacles);
        }
    }
}

void ObstaclesDetector::stopDetecting(){
    stoped = true;
}

void ObstaclesDetector::getHSVFrame(cv::Mat newHSVFrame){
    hsvFrame = newHSVFrame;
    isHSVFrameAvailable = true;
}
