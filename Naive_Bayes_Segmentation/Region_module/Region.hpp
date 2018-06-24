//
//  Region.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 21.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef Region_hpp
#define Region_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <list>


using namespace std;
using namespace cv;



class Region
{
private:
    static Mat *src_img_rgb;
    static Mat *src_img_gray;
    list<Point> Reg_vector; //vector containing the pixels of the region
    list<Point> Reg_boundary; //vector containing the boundary pixels of the region
    Region *adjacentRegion;
    //TODO stuff for Mahanalobis distance
    double StdDev; //standard deviation as texture descriptor
    double Mean;  //gray scale mean for color descriptor
    unsigned long Size; //region size
    
public:
    Region() { }
    ~Region() { cout << "called region deconstructor" << endl; }
   // void dispImg() { imshow("test", *(src_img_rgb)); cvWaitKey();}
    static void setSourceImages(Mat *rgb, Mat *gray);
    
    void computeSize() { Size = Reg_vector.size(); }
    
    void push_back(Point pix) { Reg_vector.push_back(pix); }
    void push_back_boundary(Point pix) { Reg_boundary.push_back(pix); }
   
    unsigned long getSize()  { return Reg_vector.size(); }
    double getMean();  //method for mean gray color
    double getStdDev();//method for standard deviaton (used as texture descr)
    
    friend class Region_Growing;
    
    //overloaded operators
    Region &operator+=( Region& r)
    {
        Reg_vector.splice(Reg_vector.begin(), r.Reg_vector); //splices r in front of Reg_vector
        Reg_boundary.splice(Reg_boundary.begin(), r.Reg_boundary); //splices r in front of Reg_vector
        
        return *this;
        
    }
    

    
};

//gloabal overloaded operators
inline bool operator==( Region& r1,  Region& r2){
    return r1.getSize() == r2.getSize() && r1.getMean() == r2.getMean() && r1.getStdDev() == r2.getStdDev();
}


#endif /* Region_hpp */
