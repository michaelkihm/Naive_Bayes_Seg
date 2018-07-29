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
#include <string>


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
    if(r1_ind > region_list.size() || r2_ind > region_list.size())
        cout << "bad access merge regions func" << endl;
    else
    {
        *region_list[r1_ind] += *region_list[r2_ind];
        delete_region(r2_ind);
    }

}

/* ********************************************** */
/* create random numbers to select region         */
/* ********************************************** */
void Region_Growing::rand_num(float p, unordered_set<int>& rand_set)
{
    rand_set.clear();
    srand (time(NULL));
    int size = (int)region_list.size(), rand_n;
    //std::set<int> rand_set;
    while (rand_set.size() < ceil(p*size))
    {
        rand_n = rand() % size;         // rand_n in the range 0 to size-1
        rand_set.insert(rand_n);
    }
   
}


/* ************************************************************************ */
/* initialization for the RG algorithm when not using superpixels           */
/* ************************************************************************ */
void Region_Growing::init()
{
    float count=0;
    region_num_img = Mat::zeros(src_rgb_img->rows,src_rgb_img->cols, CV_32FC1);
    
    for(int r=0; r < src_rgb_img->rows; r++ ){
        for(int c=0; c < src_rgb_img->cols; c++ )
        {
            region_list.push_back(unique_ptr<Region>(new Region));
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
    int w = src_rgb_img->cols, h = src_rgb_img->rows;
    int nr_superpixels = 1800;
    int nc = 80;
    int slic_buffer = 100;
    double step = sqrt((w * h) / (double) nr_superpixels);

    Slic slic;
    slic.generate_superpixels(lab_image, step, nc);
    
    //Mat* img = new Mat;
    //*img = src_rgb_img->clone();
    //slic.display_contours(img,  CV_RGB(255,0,0));
    //imwrite("slic_out.jpg", *img);
    
    for(int i=0; i<nr_superpixels+slic_buffer; i++)
        region_list.push_back(unique_ptr<Region>(new Region));
       
    
    
    for (int x = 0; x < src_rgb_img->cols; x++) {
        for (int y = 0; y < src_rgb_img->rows; y++) {
            Point p = Point(x,y);
            region_list[ slic.getCluster(x, y) ]->push_back(p);    
        }
    }
    
    //number of superpixel is not excatly known before SLIC execution. Therefore the variable
    //slic_buffer is used. Here the empty and not needed regions are deleted
    while(r_size(region_list.size() - 1) == 0)
    {
        region_list.erase(region_list.end()-1);
    }
    
    //initialize region_num_img
    region_num_img = Mat::zeros(src_rgb_img->rows,src_rgb_img->cols, CV_32FC1);
    for (int c = 0; c < region_num_img.cols; c++)
        for (int r = 0; r < region_num_img.rows; r++)
            region_num_img.at<float>(r,c) = (float)slic.getCluster(c, r);
    
    //delete img;
    delete lab_image;
    cout << "Finished generating SLIC superpixel" << endl;
}

/* *********************************************** */
/* updates the number with the region numbers      */
/* *********************************************** */
void Region_Growing::update_reg_num_image(int i)
{
    if(i < region_list.size())
    {
    float num = region_list[i]->getRegionNr(&region_num_img);
     for(auto it = region_list[i]->Reg_vector.begin(); it != region_list[i]->Reg_vector.end(); ++it)
         region_num_img.at<float>(*it) = num;
    }
    else
        update_reg_num_image(region_list.size()-1);
}


/* ****************************************** */
/* executes the region growing algorithm      */
/* ****************************************** */
void Region_Growing::perform()
{
    slic_wrapper();
    unordered_set<int> rand_set;
    float p = 0.3;//, max_regions = 1;
    unsigned long temp, sat_count = 0;
    double diff_stddev, diff_mean, arr;
    int diff_size;
 
    
    cout << "Start region growing " <<endl;
    
    while(sat_count != 300 /*&& region_list.size() > max_regions*/)
    {
        rand_num(p, rand_set);
        temp = region_list.size();
        for(unordered_set<int>::iterator it = rand_set.begin(); it != rand_set.end(); ++it)
        {
            if(*it < region_list.size() /*-1*/ )
            {
                for(int i = *it+1; i != *it; i++)
                {
                    i >= region_list.size() - 1? i = 0 : i=i; //circular iteration for loop
                    if(*it == i) break; //assure that region does not check itself
                    if(region_list[*it]->is_adjacent(*region_list[i], &region_num_img))
                    {
                      
                        diff_stddev  = abs(region_list[*it]->getStdDev() - region_list[i]->getStdDev());
                        diff_mean    = abs(region_list[*it]->getMean() - region_list[i]->getMean());
                        diff_size    = abs(region_list[*it]->getSize() - region_list[i]->getSize());
                        arr          = region_list[*it]->compArr(*region_list[i], &region_num_img);
                        bool can_be_merged=true;
                        if(can_be_merged) //to implement: respond of ML module
                        {
                            merge_regions(*it,i);
                            update_reg_num_image(*it);
                            break;
                        }
                    }
                }
            }
        }
        if( temp == region_list.size() )
            sat_count++;
    }
    cout << "Finished region growing" <<endl;
}


/* *********************************************** */
/* draws region boundary in the output image       */
/* *********************************************** */
void Region_Growing::display_contours()
{
    out_img = src_rgb_img->clone();
    /*
    for(int i=0; i < region_list.size(); i++)
    {
        region_list[i]->compute_boundary(&region_num_img);
        for(auto it = region_list[i]->Reg_boundary.begin(); it != region_list[i]->Reg_boundary.end(); ++it)
        {
            out_img.at<cv::Vec3b>(*it)[0] = 0;
            out_img.at<cv::Vec3b>(*it)[1] = 0;
            out_img.at<cv::Vec3b>(*it)[2] = 255;
        }
    }
    */
    
    
    //compute column differences
    for(int r=0; r < src_rgb_img->rows; r++){
        for(int c = 0; c < src_rgb_img->cols -1; c++){
            Point current(c,r);
            Point neigh(c+1,r);
            if(region_num_img.at<float>(current) != region_num_img.at<float>(neigh))
            {
                out_img.at<cv::Vec3b>(current)[0] = 0;
                out_img.at<cv::Vec3b>(current)[1] = 0;
                out_img.at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    //compute row differences
    for(int c=0; c < src_rgb_img->cols; c++){
        for(int r = 0; r < src_rgb_img->rows -1; r++){
            Point current(c,r);
            Point neigh(c,r+1);
            if(region_num_img.at<float>(current) != region_num_img.at<float>(neigh))
            {
                out_img.at<cv::Vec3b>(current)[0] = 0;
                out_img.at<cv::Vec3b>(current)[1] = 0;
                out_img.at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    
    imshow("out", out_img);
    cvWaitKey();
}


/* *********************************************** */
/* for testing                                     */
/* *********************************************** */
void Region_Growing::save_contours(int c)
{
    out_img = src_rgb_img->clone();
    string s = "image_"+std::to_string(c)+".jpg";
    //compute column differences
    for(int r=0; r < src_rgb_img->rows; r++){
        for(int c = 0; c < src_rgb_img->cols -1; c++){
            Point current(c,r);
            Point neigh(c+1,r);
            if(region_num_img.at<float>(current) != region_num_img.at<float>(neigh))
            {
                out_img.at<cv::Vec3b>(current)[0] = 0;
                out_img.at<cv::Vec3b>(current)[1] = 0;
                out_img.at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    //compute row differences
    for(int c=0; c < src_rgb_img->cols; c++){
        for(int r = 0; r < src_rgb_img->rows -1; r++){
            Point current(c,r);
            Point neigh(c,r+1);
            if(region_num_img.at<float>(current) != region_num_img.at<float>(neigh))
            {
                out_img.at<cv::Vec3b>(current)[0] = 0;
                out_img.at<cv::Vec3b>(current)[1] = 0;
                out_img.at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    
    imwrite(s, out_img);
}

/* *********************************************************** */
/* exports an image as csv file                                */
/* *********************************************************** */
void Region_Growing::writeCSV(string filename, int n,cv::Mat m)
{
    string file = filename +"_"+to_string(n)+".csv";
    ofstream outputFile(file);
    outputFile << format(m, cv::Formatter::FMT_CSV) << endl;
    outputFile.close();
}
