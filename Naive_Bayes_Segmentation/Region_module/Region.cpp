//
//  Region.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 21.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Region.hpp"

/* ****************************************************** */
/* set source images for all regions in the region list   */
/* ****************************************************** */
void Region::setSourceImages(Mat *rgb, Mat *gray)
{
    src_img_rgb = rgb;
    src_img_gray = gray;
}

Mat* Region::src_img_rgb;
Mat* Region::src_img_gray;

/* ************************************ */
/* compute mean value of region pixel   */
/* ************************************ */
double Region::getMean() 
{
    float sum = 0;
    for(std::list<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it)
        sum += src_img_gray->at<uchar>(it->y,it->x);
    return sum/Reg_vector.size();
}

/* ************************************ */
/* compute mean value of region pixel   */
/* ************************************ */
double Region::getStdDev() 
{
    if(Reg_vector.size() > 1)
    {
        double var = 0;                              //define variance
        double mean = getMean();        //call mean value function
        int histogram[256];
        float numPixels = getSize();              //define histogram

        //set histogram entries to zero
        for (int i=0; i<256; i++)
            histogram[i] = 0;

        //calculate histogram of the region
        for(std::list<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it){
            histogram[src_img_gray->at<uchar>(it->y,it->x)] +=1;
        }

        //calculate variance
        for ( int i =0; i<256; i++)
            if(histogram[i] != 0)
                var += pow((i - mean),2)* double(histogram[i]/numPixels);
        
        return sqrt(var);
    }
    else
        return 0;
    
}


/* ************************************ */
/* checks if two regions are adjacent   */
/* ************************************ */
bool Region::is_adjacent(Region *r, Mat* region_num_img)
{
    float v = r->getRegionNr(region_num_img);
    for(list<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it)
        if( check_neighborhood(*it,v,region_num_img) )
            return true;
    return false;
}

/* ****************************************************** */
/* checks if one pixel of the 8 neighborhood has value v  */
/* ****************************************************** */
bool Region::check_neighborhood(Point p, float v, Mat* region_num_img)
{
    Point pixel_above(p.y -1, p.x);
    Point pixel_left(p.y, p.x -1);
    Point pixel_below(p.y+1, p.x);
    Point pixel_right(p.y, p.x +1);
    
    Point pixel_above_left(p.y -0, p.x -0);
    Point pixel_above_right(p.y -0, p.x +1);
    Point pixel_below_left(p.y +1, p.x -1);
    Point pixel_below_right(p.y +1, p.x +1);
    
    int rows = region_num_img->rows, cols = region_num_img->cols;
    
    if (region_num_img->at<float>(pixel_above) == v && is_pixel_valid(pixel_above, rows, cols))
        return true;
    
    else if(region_num_img->at<float>(pixel_left) == v && is_pixel_valid(pixel_left, rows, cols))
        return true;

    else if (region_num_img->at<float>(pixel_below) == v && is_pixel_valid(pixel_below, rows, cols))
       return true;
    
    else if (region_num_img->at<float>(pixel_right) == v && is_pixel_valid(pixel_right, rows, cols))
        return true;
    
    else if (region_num_img->at<float>(pixel_above_left) == v && is_pixel_valid(pixel_above_left, rows, cols))
        return true;
    
    else if (region_num_img->at<float>(pixel_above_right) == v && is_pixel_valid(pixel_above_right, rows, cols))
        return true;
    
    else if (region_num_img->at<float>(pixel_below_left) == v && is_pixel_valid(pixel_below_left, rows, cols))
        return true;
    
    else if (region_num_img->at<float>(pixel_below_right) == v && is_pixel_valid(pixel_below_right, rows, cols))
        return true;
    else
        return false;
}

/* ****************************************************** */
/* checks if coordinate is in image plane                 */
/* ****************************************************** */
bool Region::is_pixel_valid(Point p, int rows, int cols)
{
    
    if (p.y >= 0 && p.y < rows && p.x >= 0 && p.x < cols)
        return true;
    else
        return false;

}

/* ****************************************************** */
/* returns region number of a region                      */
/* ****************************************************** */
float Region::getRegionNr(Mat *region_num_img)
{
    Point p = Reg_vector.front();
    return region_num_img->at<float>(p);
}
