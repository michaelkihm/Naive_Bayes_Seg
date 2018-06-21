//
//  main.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 20.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "Region_module/Region.hpp"

int main(int argc, const char * argv[]) {
    Mat test = imread("BOLOGNA4.jpg");
    Mat gray;
    cvtColor(test, gray, CV_RGB2GRAY);
    
    
    
   
    Point p(12,3), p1(12,4), p3(12,5), p4(12,6), p5(12,7);
    
    gray.at<uchar>(p) = 40;
    gray.at<uchar>(p1) = 50;
    gray.at<uchar>(p3) = 12;
    gray.at<uchar>(p4) = 11;
    gray.at<uchar>(p5) = 13;
    //imshow("gray", gray);
    //cvWaitKey();
    Region::setSourceImages(&test, &gray);
    Region r;
    
  
    r.push_back(p); r.push_back(p1); r.push_back(p3); r.push_back(p4); r.push_back(p5);
    
    std::cout << r.getSize() << std::endl;
    cout << r.getMean() << endl;
    cout << "\n" << r.getStdDev() << endl;
    return 0;
}
