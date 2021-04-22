#ifndef PATHCAPTURE_H
#define PATHCAPTURE_H

#include <QObject>
#include <QMainWindow>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class PathCapture : public QObject
{
    Q_OBJECT
public:
    PathCapture();

private:
    cv::Point path[100000];
    cv::Rect robotBox;
    int pathSize ;
    bool stoped;
    void determinePath();

public slots:
    void recieveBox(cv::Rect newRobotBox);
    void showResult(cv::Mat newFrame);

signals:
    void resultDisplay(QPixmap pixmap);
};

#endif // PATHCAPTURE_H
