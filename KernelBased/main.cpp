//
//  main.cpp
//  KernelBased
//
//  Created by hzfmacbook on 5/9/16.
//  Copyright © 2016 hzfmacbook. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int D = 10, T = 20, w1 = 0.5, w2 = 0.5;


int main(int argc, const char * argv[])
{
    Mat F, CB;
    
    VideoCapture video_capture("ellipse_slow.mov");
    
    //首先采用视频序列第 1 帧图像 F0 作为初始背景
    video_capture >> F;
    
    cvtColor(F, CB, CV_BGR2GRAY);
    
    Mat Q = Mat::ones(video_capture.get(CV_CAP_PROP_FRAME_HEIGHT), video_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_16UC1);
    
    Mat BT(video_capture.get(CV_CAP_PROP_FRAME_HEIGHT), video_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC1);
    
    
    VideoWriter writer("tracking_result.mov",
                       video_capture.get(CV_CAP_PROP_FOURCC),
                       video_capture.get(CV_CAP_PROP_FPS),
                       Size(video_capture.get(CV_CAP_PROP_FRAME_WIDTH),
                            video_capture.get(CV_CAP_PROP_FRAME_HEIGHT))
                       );
    
    //cout << video_capture.get(CV_CAP_PROP_FPS) << endl;
    
    while (true) {
        
        if (!video_capture.read(F))
            break;
        
        cvtColor(F, F, CV_BGR2GRAY);
        
        //BT and Q
        for (int i = 0; i < BT.rows; ++i) {
            for (int j = 0; j < BT.cols; ++j) {
                if (fabs(F.at<uint8_t>(i, j) - CB.at<uint8_t>(i, j)) > D) {
                    BT.at<uint8_t>(i, j) = 255;
                    ++Q.at<uint16_t>(i, j);
                }
                else
                    BT.at<uint8_t>(i, j) = 0;
            }
        }
        
        //CB
        for (int i = 0; i < CB.rows; ++i) {
            for (int j = 0; j < CB.cols; ++j) {
                if (BT.at<uint8_t>(i, j) == 255) {
                    if (Q.at<uint16_t>(i, j) > T)
                        CB.at<uint8_t>(i, j) = F.at<uint8_t>(i, j);
                }
                else
                    CB.at<uint8_t>(i, j) = w1 * CB.at<uint8_t>(i, j) + w2 * F.at<uint8_t>(i, j);
            }
        }
        
        //imshow("image", CB);
        writer << CB;
        
        //waitKey(1000 / video_capture.get(CV_CAP_PROP_FPS));
        
    }

    
    return 0;
}
