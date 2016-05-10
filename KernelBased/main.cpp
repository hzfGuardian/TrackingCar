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


int D = 50, T = 20, w1 = 0.3, w2 = 1 - w1;

Mat getHistImg(const MatND& hist)
{
    double maxVal=0;
    double minVal=0;
    
    //找到直方图中的最大值和最小值
    minMaxLoc(hist,&minVal,&maxVal,0,0);
    int histSize=hist.rows;
    Mat histImg(histSize,histSize,CV_8U,Scalar(255));
    // 设置最大峰值为图像高度的90%
    int hpt=static_cast<int>(0.9*histSize);
    
    for(int h=0;h<histSize;h++)
    {
        float binVal=hist.at<float>(h);
        int intensity=static_cast<int>(binVal*hpt/maxVal);
        line(histImg,Point(h,histSize),Point(h,histSize-intensity),Scalar::all(0));
    }
    
    return histImg;
}

int main(int argc, const char * argv[])
{
    Mat F, CB, DM;
    
    VideoCapture video_capture("move.mpeg");
    
    //首先采用视频序列第 1 帧图像 F0 作为初始背景
    video_capture >> F;
    
    //cvtColor(F, CB, CV_BGR2GRAY);
    F.copyTo(CB);
    
    Mat Q = Mat::ones(video_capture.get(CV_CAP_PROP_FRAME_HEIGHT), video_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_16UC1);
    
    Mat BT(video_capture.get(CV_CAP_PROP_FRAME_HEIGHT), video_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC1);
    
    
    while (true) {
        
        if (!video_capture.read(F))
            break;
        
        //cvtColor(F, F, CV_BGR2GRAY);
        
        imshow("F", F);
        
        imshow("image", CB);
        
        
        //BT and Q
        Mat tmp = abs(F - CB);
        cvtColor(tmp, tmp, CV_BGR2GRAY);
        
        threshold(tmp, BT, D, 255, THRESH_BINARY);
        
        for (int i = 0; i < BT.rows; ++i) {
            for (int j = 0; j < BT.cols; ++j) {
                
                int r = F.at<Vec3b>(i, j)[0] - CB.at<Vec3b>(i, j)[0];
                int g = F.at<Vec3b>(i, j)[1] - CB.at<Vec3b>(i, j)[1];
                int b = F.at<Vec3b>(i, j)[2] - CB.at<Vec3b>(i, j)[2];
                
                if (r * r + g * g + b * b > D * D)
                    Q.at<uint16_t>(i, j)++;
                
            }
        }
        
        //CB
        for (int i = 0; i < CB.rows; ++i) {
            for (int j = 0; j < CB.cols; ++j) {
                
                if (BT.at<uint8_t>(i, j) == 255) {
                    if (Q.at<uint16_t>(i, j) > T)
                        CB.at<Vec3b>(i, j) = F.at<Vec3b>(i, j);
                }
                else
                    CB.at<Vec3b>(i, j) = w1 * CB.at<Vec3b>(i, j) + w2 * F.at<Vec3b>(i, j);
                
            }
        }
        
        //calculate Hist
        const int channels[1]={0};
        const int histSize[1]={256};
        float hranges[2]={0,255};
        const float* ranges[1]={hranges};
        MatND hist;
        Mat tmpgray;
        cvtColor(F, tmpgray, CV_BGR2GRAY);
        calcHist(&tmpgray, 1, channels, Mat(), hist, 1, histSize, ranges);
        
        imshow("Hist", getHistImg(hist));
        
        
        //binary
        DM = abs(F - CB);
        
        Mat BDK;
        cvtColor(DM, DM, CV_BGR2GRAY);
        
        int thre = 60;
        threshold(DM, BDK, thre, 255, THRESH_BINARY);
        
        imshow("Object", BDK);
        
        waitKey();
    }
    
    
    return 0;
}







