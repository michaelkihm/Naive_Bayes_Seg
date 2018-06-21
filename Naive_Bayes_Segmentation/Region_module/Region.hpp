//
//  Region.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 21.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef Region_hpp
#define Region_hpp

#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;



class Region
{
private:
    static Mat *src_img_rgb;
    static Mat *src_img_gray;
    vector<Point> Reg_vector; //vector containing the pixels of the region
    vector<Point> Reg_boundary; //vector containing the boundary pixels of the region
    //TODO stuff for Mahanalobis distance
    double StdDev; //standard deviation as texture descriptor
    double Mean;  //gray scale mean for color descriptor
    unsigned long Size; //region size
    
public:
    //Region(Mat *rgb, Mat *gray):src_img_rgb(rgb),src_img_gray(gray) { }
    Region() { }
    //void dispImg() { imshow("test", *(image)); cvWaitKey();}
    static void setSourceImages(Mat *rgb, Mat *gray);
    //methods for region size
    unsigned long getSize() { return Reg_vector.size(); }
    void computeSize() { Size = Reg_vector.size(); }
    
    void push_back(Point pix) { Reg_vector.push_back(pix); }
    void push_back_boundary(Point pix) { Reg_boundary.push_back(pix); }
   
    double getMean();  //method for mean gray color
    double getStdDev();//method for standard deviaton (used as texture descr)
    
    
    
};
#include <stdio.h>

#endif /* Region_hpp */
