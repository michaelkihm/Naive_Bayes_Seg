//
//  Region_Growing.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 22.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Region_Growing.hpp"
#include <math.h>
#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */


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
}

/* ********************************************** */
/* create random numbers to select region         */
/* ********************************************** */
void Region_Growing::rand_num(float p, set<int>& rand_set)
{
    rand_set.clear();
    srand (time(NULL));
    float prob=0.15;
    int size = (int)region_list.size(), rand_n;
    //std::set<int> rand_set;
    while (rand_set.size() < ceil(prob*size))
    {
        rand_n = rand() % size;         // rand_n in the range 0 to size-1
        rand_set.insert(rand_n);
    }
   
}

/* ********************************************** */
/* initialization for the RG algorithm            */
/* ********************************************** */
void Region_Growing::init()
{
    float count=0;
    region_num_img = Mat::zeros(src_rgb_img->rows,src_rgb_img->cols, CV_32FC1);
    
    for(int r=0; r < src_rgb_img->rows; r++ ){
        for(int c=0; c < src_rgb_img->cols; c++ )
        {
            region_list.push_back(new Region);
            Point p = Point(c,r);
            region_list.back()->push_back(p);
            region_num_img.at<float>(r,c) = count;
            count++;
        }
    }
}

/* ******************************************************** */
/* wrapper from slic algorithm to Region Growing data       */
/* **********************************************************/
void Region_Growing::slic_wrapper()
{
    
    Mat *lab_image = new Mat(src_rgb_img->rows, src_rgb_img->cols, CV_8UC3);
    cvtColor(*src_rgb_img, *lab_image, CV_BGR2Lab);
    Mat image = src_rgb_img->clone();
    int w = image.cols, h = image.rows;
    int nr_superpixels = 4000;
    int nc = 40;
    
    double step = sqrt((w * h) / (double) nr_superpixels);
    
    Slic slic;
    slic.generate_superpixels(lab_image, step, nc);
    slic.create_connectivity(lab_image);
    
    /* Display the contours and show the result. */
    slic.display_contours(&image, CV_RGB(255,0,0));
    imshow("result", image);
    cvWaitKey(0);
    
}

