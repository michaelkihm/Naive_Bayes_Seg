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
#include "Region_Growing.hpp"
#include <list>
#include <set>
#include <math.h>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */


int main(int argc, const char * argv[]) {
    Mat test = imread("/Users/michaelkihm/Documents/CV_projects/final HTCV/Training/311068.jpg");//dog.png" 6.2
    Mat gray;
    cvtColor(test, gray, CV_RGB2GRAY);
    
   
    Point p(12,3), p1(12,4), p3(12,5), p4(12,6), p5(12,7);
    Point p21(13,3), p22(13,4), p23(13,5), p24(13,6), p25(13,7);
    
    
    //gray.at<uchar>(p) = 40;
    //gray.at<uchar>(p1) = 50;
    //gray.at<uchar>(p3) = 12;
    //gray.at<uchar>(p4) = 11;
    //gray.at<uchar>(p5) = 13;
    //imshow("gray", gray);
    //cvWaitKey();

    Region r, r2;
    Region::setSourceImages(&test, &gray);
  
   

    
    r.push_back(p); r.push_back(p1); r.push_back(p3); r.push_back(p4); r.push_back(p5);
    r2.push_back(p21); r2.push_back(p22); r2.push_back(p23); r2.push_back(p24); r2.push_back(p25);
    
    //std::cout << r.getSize() << std::endl;
    cout << "r1 mean: "<<r.getMean() << endl;
    cout << "r2 mean: "<<r2.getMean() << endl;
    
    r += r2;
    cout << "\n" << r.getStdDev() << endl;
    cout << "r1 mean: "<<r.getMean() << endl;
    cout << "r1 size: "<<r.getSize() << endl;
    //bool b1 = r == r;
    //cout <<"is equal?: "<< b1 << endl;
     
    cout << "---------------------- \n" << endl;
    Region_Growing test_r(&test), test_ini(&test);
    test_ini.perform();
    
    
    test_r.push_back(&r  );
    test_r.push_back(&r2);
    cout <<  test_r.size() << endl;
    //test_r.merge_regions(0, 1);

    cout <<  "size after merge "<<test_r.size() << endl;
    cout <<test_r.r_size(0) << " mean: "<< test_r.r_mean(0)<< endl;
    
    //test_ini.slic_wrapper();
    cout <<" \nsize reg list: " << test_ini.size() << endl;

    
    
    return 0;
}
