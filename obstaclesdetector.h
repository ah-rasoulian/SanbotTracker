#ifndef OBSTACLESDETECTOR_H
#define OBSTACLESDETECTOR_H

#include <QObject>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class ObstaclesDetector : public QObject
{
    Q_OBJECT
public:
    ObstaclesDetector();

private:
    bool stoped;
    bool isHSVFrameAvailable;
    cv::Mat hsvFrame;

signals:
    void detectingObstacles(cv::Rect* obstalceBoxes, int numberOfObstacles);

public slots:
    void startDetecting();
    void stopDetecting();
    void getHSVFrame(cv::Mat newHSVFrame);
};

#endif // OBSTACLESDETECTOR_H
