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
#include "ML_module/Stat.h"
#include <list>
#include <set>
#include <math.h>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */

#include <string>

#include <sstream>
#include <fstream>

#include "Training.hpp"



int main(int argc, const char * argv[]) {
    //Mat test = imread("/Users/michaelkihm/Documents/CV_projects/final HTCV/Training/12003.jpg");//dog.png" 6.2
    Mat test = imread("dog.png");//dog.png" 6.2
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
    Region_Growing test_r(&test);
    //test_ini.perform();
    
    
    test_r.push_back(&r  );
    test_r.push_back(&r2);
    cout <<  test_r.size() << endl;
    //test_r.merge_regions(0, 1);

    cout <<  "size after merge "<<test_r.size() << endl;
    cout <<test_r.r_size(0) << " mean: "<< test_r.r_mean(0)<< "\n"<<endl;
    
    set<int> rand_set{12,2,7,8,9,0,14,67,89,90,30,40};
    
    
    for(set<int>::iterator it = rand_set.begin(); it != rand_set.end(); it++)
    {
        if(*it < 50)
            cout << *it << endl;


    }
    cout << "\n--------------------------------" << endl;
    vector<int> v{0,1,2,3,4,5,6,7,8,9,10,11};
    int is = 3;
    bool b=false;
    for(int i = is+1; i != is; i++)
    {
        i == v.size() - 1? i = 0 : i=i;
        cout << i << endl;
        if(b==true)
            break;
        if(i==7)
            b=true;
    }
    cout << "\n--------------------------------" << endl;
    
 
    
    
    
    
    //GotoLine(input, 8);

    
   // int rows = get_row_or_col(input, 5);
    
    //string s;
    /*
    for (int i = 0; i <= 4; i++)
    {
        std::getline(input, s);
    }
    
    cout << s << endl;
    int word = s.find(" ");
    string strNew = s.substr(word+1,s.back());
    cols = stoi(strNew);
     */

    /*
    for( std::string line; getline( input, line ); )
    {
        cout << line << endl;
    }
    */
   cout << "\n--------------------------------" << endl;


    string path = "/Users/michaelkihm/Desktop/train_data/human/color/1108/286092.seg";
    string p2 = "/Users/michaelkihm/Desktop/BSDS300/images/train/286092.jpg";
    Mat test_train = imread(p2);
    //Training test123;
    
    Training coach(p2, path, &test_train);
    coach.train();
    
  
  
    
    //Region_Growing test_ini(&test);
    //test_ini.perform();
    //test_ini.display_contours();
    //cout <<" \nsize reg list: " << test_ini.size() << endl;
    return 0;
}





