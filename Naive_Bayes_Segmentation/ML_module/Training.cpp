//
//  Training.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Training.hpp"
#include <set>


/* *************************************************************** */
/* read ground truth .seg file and initializes the                 */
/*  ground truth region vector                                     */
/* *************************************************************** */
void Training::init()
{
   
    //read .seg file and initialize region_num_img
    fstream f( seg_file_path);
    string input;
    getline(f,input);getline(f,input);getline(f,input);getline(f,input); //ignore first lines
    
    
    int cols,rows,row,col1,col2;
    
    f>>input;
    f>>cols;
    f>>input;
    f>>rows;
    f>>input;
    f>>segments;
    f.ignore();
    getline(f,input);getline(f,input);getline(f,input);getline(f,input); //ignore next 4 lines
    gt_reg_num_image = new Mat(rows, cols, CV_32F);
    float region;
    while(!f.eof()){
        f>>region;
        f>>row;
        f>>col1;
        f>>col2;
        for(int i=col1;i<=col2;i++)
            gt_reg_num_image->at<float>(row,i) = region;
    }
    f.close();

    //------------------------------------//
    //init region vector
    for(int i=0; i < segments; i++)
        gt_region_vector.push_back(unique_ptr<Region>(new Region));
    
    
    for (int r = 0; r < gt_reg_num_image->rows; r++) {
        for (int c = 0; c < gt_reg_num_image->cols; c++) {
            Point p = Point(c,r);
            float ind = gt_reg_num_image->at<float>(r,c);
            gt_region_vector[ind]->push_back(p);
        }
    }
    
    //-----------------test stuff--------------------//
    Mat out_img = src_rgb_img->clone();
    
    
    /*
    //compute column differences
    for(int r=0; r < src_rgb_img->rows; r++){
        for(int c = 0; c < src_rgb_img->cols -1; c++){
            Point current(c,r);
            Point neigh(c+1,r);
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
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
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
            {
                out_img.at<cv::Vec3b>(current)[0] = 0;
                out_img.at<cv::Vec3b>(current)[1] = 0;
                out_img.at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    */
 
        
    

    
    for(int i=0; i < gt_region_vector.size(); i++)
    {
        gt_region_vector[i]->compute_boundary(gt_reg_num_image);
        for(auto it = gt_region_vector[i]->Reg_boundary.begin(); it != gt_region_vector[i]->Reg_boundary.end(); ++it)
        {
            out_img.at<cv::Vec3b>(*it)[0] = 0;
            out_img.at<cv::Vec3b>(*it)[1] = 0;
            out_img.at<cv::Vec3b>(*it)[2] = 255;
        }
    }
    imwrite("gt_img.jpg", out_img);
   
    
    if(gt_region_vector.size() != segments)
        cout << "Error during init ground truth" <<endl;
    
}

/* *************************************************************** */
/* perform trainig                                                 */
/* adapted version of perform algorithm of Region_Growing class    */
/* *************************************************************** */
void Training::train()
{
    
    unordered_set<int> rand_set;
    float p = 0.5;
    unsigned long temp, sat_count = 0;
    int bmerged;
    double diff_stddev, diff_mean, arr;
    int diff_size;
    ofstream outfile;
    outfile.open("test.txt", std::ios_base::app);
    
    int c=0;
    //------------------------------------//
    //call slic wrapper
    slic_wrapper();
    
    //------------------------------------//
    //init ground truth data
    init();
    
    //------------------------------------//
    //start training
    cout << segments << endl;
    while(/*sat_count != 3000*/region_list.size() >= segments)
    {
        if(region_list.size() > 560)
            rand_num(p, rand_set);
        else
            rand_num(1, rand_set);
        bmerged = 0;
        temp = region_list.size();
        cout << region_list.size() << endl;
        for(unordered_set<int>::iterator it = rand_set.begin(); it != rand_set.end(); ++it)
        {
            if(*it < region_list.size() && bmerged < 50)
            {
                for(int i = *it+1; i != *it; i++)
                {
                    i >= region_list.size() - 1? i = 0 : i=i; //circular iteration for loop
                    if(*it == i) break; //assure that region does not check itself
                    if(region_list[*it]->is_adjacent(*region_list[i], &region_num_img))
                    {
                        
                        diff_stddev  = abs( region_list[*it]->getStdDev() - region_list[i]->getStdDev() );
                        diff_mean    = abs( region_list[*it]->getMean() - region_list[i]->getMean() );
                        diff_size    = ( region_list[*it]->getSize() - region_list[i]->getSize() );
                        arr          = region_list[*it]->compArr(*region_list[i], &region_num_img);
                        
                        
                        if( bcan_be_merged(*it, i) ) //to implement
                        {
                            outfile<<diff_stddev<<" "<<diff_mean<<" "<<abs(diff_size)<<" "<<arr<<" "<< 1 << endl;//save result for trainings data
                            merge_regions(*it,i);
                            update_reg_num_image(*it);
                            bmerged++; //used to only merge one region pair of each rand vector
                            break;
                        }
                        else
                        {
                            outfile<<diff_stddev<<" "<<diff_mean<<" "<<abs(diff_size)<<" "<<arr<<" "<< 0 << endl;//save result for trainings data
                        }
                    }
                }
            }
        }
        if(c==5)
        {
            save_contours(sat_count+=1);
            writeCSV("gt_img", sat_count,*gt_reg_num_image);
            writeCSV("reg_img" , sat_count, region_num_img);
            c=0;
        }
        c+=1;
    }
   
    display_contours();
    save_contours(0);
    delete gt_reg_num_image;
    cout << "region_list size "<<region_list.size() <<" segments: "<<segments<< endl;
    cout << "Finished training" <<endl;
}


/* ********************************************* */
/*  checks if two adjacent regions can be merged */
/* ********************************************* */
bool Training::bcan_be_merged(int r1, int r2)
{
    return find_majority_element(r1) == find_majority_element(r2) && find_majority_element(r1)!=false && find_majority_element(r2)!=false ? true : false;
    //return find_majority_element(r1) && find_majority_element(r2) ? true : false;
}


/* ********************************************************************************* */
/*  Moore’s Voting Algorithm to get a candidate for majority element.                */
/* ********************************************************************************* */
int Training::find_majority_element(int r_index)
{
    //fill vector with ground truth region number image
    list<Point> *reg_list = region_list[r_index]->getRegList();
    vector<float> v, check_vec;
    update_reg_num_image(r_index);
    
    for(auto it = reg_list->begin(); it != reg_list->end(); ++it)
        check_vec.push_back(region_num_img.at<float>(*it));
    
    for(int i=0; i < check_vec.size()-1; i++)
        if(check_vec[i] != check_vec[i+1])
            cout << "different numbers inside one region!!"<<endl;
    

    
     for(list<Point>::iterator it = reg_list->begin(); it != reg_list->end(); ++it)
         v.push_back(gt_reg_num_image->at<float>(*it));
    
    for(int i=0; i < v.size()-1; i++)
        if(v[i] != v[i+1])
            return false;
    return v[0];

/*
    //find majority element of vector v
    int maj_index = 0, count = 1;
    for (int i = 1; i < v.size(); i++)
    {
        if (v[maj_index] == v[i])
            count++;
        else
            count--;
        if (count == 0)
        {
            maj_index = i;
            count = 1;
        }
    }
    
    int cnt =0;
    for (int i = 1; i < v.size(); i++)
        if(v[i]==v[maj_index])
            cnt++;
    if(region_list.size() > 800)
    {
        if(cnt >= v.size()*0.98)
            return v[maj_index];
        else
            return false;
    }
    else if(region_list.size() < 800 && region_list.size() > 600)
    {
        if(cnt >= v.size()*0.8)
            return v[maj_index];
        else
            return false;
    }
    else
    {
        if(cnt >= v.size()*0.3)
            return v[maj_index];
        else
            return false;
    }
    */
   
}


