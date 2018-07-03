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
#include <set>
#include "../Region_module/Region.hpp"
#include "slic.h"


class Region_Growing
{
private:
    //data
    Mat *src_rgb_img;
    Mat src_gray_img;
    Mat region_num_img; //image in which pixel have the value of the region they belong to
    Mat out_img; //image to show segmentation result
    vector<Region*> region_list; //vector of pointer to regions
    //list<Region*> region_list;
 
    
    //methods
    //void push_back(Region *r) { region_list.push_back(r); }
    //void deleteRegion(int index) { region_list.erase(region_list.begin() + index); }
    //void merge(int r1_ind, int r2_ind);
    //void initialize();
public:
    Region_Growing(Mat *src);
    Region_Growing() = delete;
    unsigned long  size() const { return region_list.size(); }
    /*void disp() {imshow("test rgb", *src_rgb_img);cvWaitKey();
        imshow("test gray", src_gray_img);
        cvWaitKey();}*/
    unsigned long r_size(int i) const { return region_list[i]->getSize(); }
    double r_mean(int i) const { return region_list[i]->getMean(); }
    double r_stddev(int i) const { return region_list[i]->getStdDev(); }
    
    void push_back(Region *r) { region_list.push_back(r); } //for testing in public
    void delete_region(int index) { delete region_list[index]; region_list.erase(region_list.begin() + index); }//for testing in public
    void merge_regions(int r1_ind, int r2_ind);//for testing in public
    void init();//for testing in public
    void draw_output();//for testing in public
    void rand_num(float p, set<int>& rand_set);//for testing in public
    void update_reg_num_image(int i); //for testing in public
    
    void slic_wrapper();
    void perform();
    void display_contours();
    void save_contours(int c);
    
    
};



#endif /* Region_Growing_hpp */
