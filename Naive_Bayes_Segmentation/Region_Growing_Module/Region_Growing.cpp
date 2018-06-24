//
//  Region_Growing.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 22.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Region_Growing.hpp"


/* ************************************ */
/* constructor                          */
/* ************************************ */
Region_Growing::Region_Growing(Mat *src):src_rgb_img(src)
{
    cvtColor(*src_rgb_img, src_gray_img, CV_RGB2GRAY);
    Region::setSourceImages(src_rgb_img, &src_gray_img);  
}


/* ************************************ */
/* merges two regions                   */
/* ************************************ */
void Region_Growing::merge_regions(int r1_ind, int r2_ind)
{
    *region_list[r1_ind] += *region_list[r2_ind];
    delete_region(r2_ind);
    //delete r2;
    
}


/* ********************************************** */
/* initialization for the RG algorithm            */
/* ********************************************** */
void Region_Growing::init()
{
    int n_regions = src_rgb_img->cols * src_rgb_img->rows;
    int n_rows = src_rgb_img->rows;
    region_num_img = Mat::zeros(src_rgb_img->rows,src_rgb_img->cols, CV_8UC1);
    //region_list.push_back(new Region);
}
