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

/* ****************************************** */
/* computes the arrangement of two regions to */
/* each other                                 */
/* ****************************************** */
double Region::compArr(/*Region* */Region& r, Mat *region_num_image)
{
    compute_boundary(region_num_image);
    //r->compute_boundary(region_num_image);
    r.compute_boundary(region_num_image);
    //float adj_reg_nr = r->getRegionNr(region_num_image);
    float adj_reg_nr = r.getRegionNr(region_num_image);
    int adj_pixel = 0;
    
    for(std::list<Point>::iterator it = Reg_boundary.begin(); it != Reg_boundary.end(); ++it)
    {
        if(check_neighborhood(*it, adj_reg_nr, region_num_image))
            adj_pixel++;
    }
    double result = adj_pixel/( (float)get_boundary_size()  + /*(float)r->get_boundary_size()*/(float)r.get_boundary_size());
    if(result > 1 || result <= 0)
        cout << "strange arrangement: " << result << endl;
    return result;
    
}


/* ************************************ */
/* checks if two regions are adjacent   */
/* ************************************ */
bool Region::is_adjacent(/*Region **/ Region& r, Mat* region_num_img)
{
    //float v = r->getRegionNr(region_num_img);
    float v = r.getRegionNr(region_num_img);
    for(list<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it)
        if( check_neighborhood(*it,v,region_num_img) )
            return true;
    return false;
}

/* ********************************************************** */
/* checks if one pixel of the 8 neighborhood has NOT value v  */
/* ********************************************************** */
bool Region::check_neighborhood_NOT(Point p, float v, Mat* region_num_img)
{
    int rows = region_num_img->rows, cols = region_num_img->cols;
    
    //4 neighborhood
    if(is_pixel_valid(p.y -1, p.x, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x) != v )
            return true;
    
    if(is_pixel_valid(p.y, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y, p.x -1) != v )
            return true;
    
    if(is_pixel_valid(p.y+1, p.x, rows, cols))
        if ( region_num_img->at<float>(p.y+1, p.x) != v )
            return true;
    
    if(is_pixel_valid(p.y, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y, p.x +1) != v )
            return true;
    
    
    //4 diagonal pixels
    if(is_pixel_valid(p.y -1, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x -1) != v )
            return true;
    
    if(is_pixel_valid(p.y -1, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x +1) != v )
            return true;
    
    if(is_pixel_valid(p.y +1, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y +1, p.x -1) != v )
            return true;
    
    if(is_pixel_valid(p.y +1, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y +1, p.x +1) != v )
            return true;
    
    
    return false;

}

/* ********************************************************** */
/* checks if one pixel of the 8 neighborhood has value v  */
/* ********************************************************** */
bool Region::check_neighborhood(Point p, float v, Mat* region_num_img)
{
    int rows = region_num_img->rows, cols = region_num_img->cols;
    
    //4 neighborhood
    if(is_pixel_valid(p.y -1, p.x, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x) == v )
            return true;
    
    if(is_pixel_valid(p.y, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y, p.x -1) == v )
            return true;
    
    if(is_pixel_valid(p.y+1, p.x, rows, cols))
        if ( region_num_img->at<float>(p.y+1, p.x) == v )
            return true;
    
    if(is_pixel_valid(p.y, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y, p.x +1) == v )
            return true;
    
    
    //4 diagonal pixels
    if(is_pixel_valid(p.y -1, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x -1) == v )
            return true;
    
    if(is_pixel_valid(p.y -1, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y -1, p.x +1) == v )
            return true;
    
    if(is_pixel_valid(p.y +1, p.x -1, rows, cols))
        if ( region_num_img->at<float>(p.y +1, p.x -1) == v )
            return true;
    
    if(is_pixel_valid(p.y +1, p.x +1, rows, cols))
        if ( region_num_img->at<float>(p.y +1, p.x +1) == v )
            return true;
    
    
    return false;
    
}


/* ****************************************************** */
/* checks if coordinate is in image plane                 */
/* ****************************************************** */
bool Region::is_pixel_valid(int r, int c, int rows, int cols)
{
    
    if (r >= 0 && r < rows && c >= 0 && c < cols)
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


/* ****************************************************** */
/* fills the boundary vector                              */
/* ****************************************************** */
void Region::compute_boundary(Mat* region_num_image)
{
    float v = getRegionNr(region_num_image);
    for(std::list<Point>::iterator it = Reg_vector.begin(); it != Reg_vector.end(); ++it)
    {
        if(check_neighborhood_NOT(*it, v, region_num_image))
            push_back_boundary(*it);
        
        
    }

}
