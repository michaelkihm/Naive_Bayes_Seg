//
//  Region.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 21.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Region.hpp"
#include <cmath>
/* ************************************ */
/* compute mean value of region pixel   */
/* ************************************ */
double Region::getMean()
{
    float sum = 0;
    for(std::vector<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it)
        sum += src_img_gray->at<uchar>(it->y,it->x);
    return sum/Reg_vector.size();
}

/* ************************************ */
/* compute mean value of region pixel   */
/* ************************************ */
double Region::getStdDev()
{
    
    double var = 0;                              //define variance
    double mean = getMean();        //call mean value function
    int histogram[256];
    float numPixels = getSize();              //define histogram

    //set histogram entries to zero
    for (int i=0; i<256; i++)
        histogram[i] = 0;

    //calculate histogram of the region
    for(std::vector<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it){
        histogram[src_img_gray->at<uchar>(it->y,it->x)] +=1;
    }

    //calculate variance
    for ( int i =0; i<256; i++)
        if(histogram[i] != 0)
            var += pow((i - mean),2)* double(histogram[i]/numPixels);
    
    
    
    return sqrt(var);
    
}

