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

int main(int argc, const char * argv[])
{
    Mat orig, current;
    
    VideoCapture video_capture("move.mpeg");
    
    //首先采用视频序列第 1 帧图像 F0 作为初始背景
    video_capture >> orig;
    
    imshow("image", orig);
    
    while (true) {
        
        if (!video_capture.read(current))
            break;
        
        
    }
    

    cout << video_capture.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    
    waitKey();
    
    return 0;
}
