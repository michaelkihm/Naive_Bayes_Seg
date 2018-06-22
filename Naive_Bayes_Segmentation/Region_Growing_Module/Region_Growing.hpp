//
//  Region_Growing.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 22.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef Region_Growing_hpp
#define Region_Growing_hpp

#include <stdio.h>
#include "../Region_module/Region.hpp"


class Region_Growing
{
private:
    //data
    Mat *src_rgb_img;
    Mat src_gray_img;
    Mat out_img; //image to show segmentation result
    //vector<Region*> region_list; //vector of pointer to regions
    list<Region*> region_list;
    
    //methods
    void addRegion(Region *r) { region_list.push_back(r); }
    void deleteRegion(Region *r) { region_list.remove(r); }
    void merge(Region *r1, Region *r2);
public:
    Region_Growing(Mat *src);
    /*void disp() {imshow("test rgb", *src_rgb_img);cvWaitKey();
        imshow("test gray", src_gray_img);
        cvWaitKey();}*/
    
};



#endif /* Region_Growing_hpp */
