//
//  Training.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.


#include "Training.hpp"
#include <set>


/* *************************************************************** */
/* read ground truth .seg file and initializes the                 */
/*  ground truth region vector                                     */
/* *************************************************************** */
void Training::init_gt()
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
    gt_reg_num_image = new Mat(rows, cols, CV_32FC1);
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
    
    create_gt_edge_image();
    //-----------------test stuff--------------------//
    Mat* out_img = new Mat;
    *out_img=src_rgb_img.clone();
    
    
    
    //compute column differences
    for(int r=0; r < src_rgb_img.rows; r++){
        for(int c = 0; c < src_rgb_img.cols -1; c++){
            Point current(c,r);
            Point neigh(c+1,r);
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
            {
                out_img->at<cv::Vec3b>(current)[0] = 0;
                out_img->at<cv::Vec3b>(current)[1] = 0;
                out_img->at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    //compute row differences
    for(int c=0; c < src_rgb_img.cols; c++){
        for(int r = 0; r < src_rgb_img.rows -1; r++){
            Point current(c,r);
            Point neigh(c,r+1);
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
            {
                out_img->at<cv::Vec3b>(current)[0] = 0;
                out_img->at<cv::Vec3b>(current)[1] = 0;
                out_img->at<cv::Vec3b>(current)[2] = 255;
            }
        }
    }
    
 
    imwrite("gt_img.jpg", *out_img);
   
    delete out_img;
    if(gt_region_vector.size() != segments)
        cout << "Error during init ground truth" <<endl;
    
}

/* *************************************************************** */
/* draws an edge image of the ground truth image                   */
/* *************************************************************** */
void Training::create_gt_edge_image()
{
    gt_edge_image = new Mat;
    *gt_edge_image = Mat::zeros(src_gray_img.size(), CV_8UC1);
    
    //compute column differences
    for(int r=0; r < src_rgb_img.rows; r++){
        for(int c = 0; c < src_rgb_img.cols -1; c++){
            Point current(c,r);
            Point neigh(c+1,r);
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
                gt_edge_image->at<uchar>(current) = 255;
        }
    }
    //compute row differences
    for(int c=0; c < src_rgb_img.cols; c++){
        for(int r = 0; r < src_rgb_img.rows -1; r++){
            Point current(c,r);
            Point neigh(c,r+1);
            if(gt_reg_num_image->at<float>(current) != gt_reg_num_image->at<float>(neigh))
                gt_edge_image->at<uchar>(current) = 255;
        }
    }
    
}



/* *************************************************************** */
/* perform trainig                                                 */
/* adapted version of perform algorithm of Region_Growing class    */
/* *************************************************************** */
void Training::train()
{
    
    unordered_set<int> rand_set;
    float p = 0.05;
    unsigned long temp, sat_count = 0;
    int bmerged;
    double diff_stddev, diff_mean, arr;
    int diff_size;
    ofstream outfile;
    outfile.open("test_data.txt", std::ios_base::app);
  
    int merged_per_rand_set = 10;
    //int c=0;
    
    //------------------------------------//
    //call slic wrapper
    slic_wrapper();
    //init();
    
    //------------------------------------//
    //init ground truth data
    init_gt();
    
    //------------------------------------//
    //start training
    string s = "Start training image ";// + ;
    cout << s << endl;
    while(sat_count < 10 && region_list.size() >= segments+1)
    {
        if (region_list.size() < 500)       //p control to be put in an extra function
            p = 0.25;
        else if(region_list.size() <= 100)
            p = 1.0;
        
        rand_num(p, rand_set);
  
       
        bmerged = 0;
        temp = region_list.size();
        //cout << region_list.size() << endl;
        for(unordered_set<int>::iterator it = rand_set.begin(); it != rand_set.end(); ++it)
        {
            if(*it < region_list.size() && bmerged <= merged_per_rand_set)
            {
                for(int i = *it+1; i != *it; i++)
                {
                    i >= region_list.size() - 1? i = 0 : i=i; //circular iteration for loop
                    if(*it == i) break; //assure that region does not check itself
                    if(region_list[*it]->is_adjacent(*region_list[i], &region_num_img))
                    {
                        
                        diff_stddev  = abs( region_list[*it]->getStdDev() - region_list[i]->getStdDev() );
                        diff_mean    = abs( region_list[*it]->getMean() - region_list[i]->getMean() );
                        diff_size    = abs(region_list[*it]->getSize() - region_list[i]->getSize() );
                        arr          = region_list[*it]->compArr(*region_list[i], &region_num_img);
                        
                        
                        if( bcan_be_merged(*it, i) ) //to implement
                        {
                            outfile<<diff_stddev<<" "<<diff_mean<<" "<<diff_size<<" "<<arr<<" "<< 1<< endl;//save result for trainings data
                            merge_regions(*it,i);
                            update_reg_num_image(*it);
                            bmerged++; //used to only merge one region pair of each rand vector
                            break;
                        }
                        else
                        {
                            outfile<<diff_stddev<<" "<<diff_mean<<" "<<diff_size<<" "<<arr<<" "<< 0 << endl;//save result for trainings data
                        }
                    }
                }
            }
        }
        if( temp == region_list.size() )
            sat_count++;
    }
   
    for(int i=0; i < region_list.size(); i++)
        update_reg_num_image(i);
    //save_contours(0);
    //delete new pointer images
    delete gt_reg_num_image;
    delete gt_edge_image;
    
    
    cout << "region_list size "<<region_list.size() <<" segments: "<<segments<< endl;
    cout << "Finished training" <<endl;
    display_contours();
    writeCSV("trained_image",0,region_num_img);
}


/* ********************************************* */
/*  checks if two adjacent regions can be merged */
/* ********************************************* */
bool Training::bcan_be_merged(int r1, int r2)
{
    return find_majority_element(r1) == find_majority_element(r2);// && boundary_test(r1,r2);// && find_majority_element(r1)!=false && find_majority_element(r2)!=false ? true : false;
    //return find_majority_element(r1) && find_majority_element(r2) ? true : false;
}


/* ********************************************************************************* */
/*  Moore’s Voting Algorithm to get a candidate for majority element.                */
/* ********************************************************************************* */
int Training::find_majority_element(int r_index)
{
    //fill vector with ground truth region number image
    list<Point> *reg_list = region_list[r_index]->getRegList();
    vector<float> v;// check_vec;
    update_reg_num_image(r_index);
    
   // for(auto it = reg_list->begin(); it != reg_list->end(); ++it)
     //   check_vec.push_back(region_num_img.at<float>(*it));
    
   // for(int i=0; i < check_vec.size()-1; i++)
     //   if(check_vec[i] != check_vec[i+1])
       //     cout << "different numbers inside one region!!"<<endl;
    

   
     for(list<Point>::iterator it = reg_list->begin(); it != reg_list->end(); ++it)
         v.push_back(gt_reg_num_image->at<float>(*it));
   /*
    for(int i=0; i < v.size()-1; i++)
        if(v[i] != v[i+1])
            return false;
    return v[0];
 */

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
    return v[maj_index];
}

bool Training::boundary_test(int r1, int r2)
{
    update_reg_num_image(r1);
    update_reg_num_image(r2);
    
    region_list[r1]->compute_boundary(&region_num_img);
    region_list[r2]->compute_boundary(&region_num_img);
    list<Point> *bound_list_r1 = region_list[r1]->getBoundList();
    list<Point> *bound_list_r2 = region_list[r2]->getBoundList();
    
    
    Region r;
    list<Point> *reg_l1   = region_list[r1]->getRegList();
    for(auto it = reg_l1->begin();  it != reg_l1->end(); ++it)
        r.push_back(*it);
    list<Point> *reg_l2   = region_list[r1]->getRegList();
    for(auto it = reg_l2->begin();  it != reg_l2->end(); ++it)
        r.push_back(*it);
    
    
    list<Point> *reg_list   = r.getRegList();
    //int cnt = 0;
    for(auto it = reg_list->begin(); it != reg_list->end(); ++it)
    {
        if(gt_edge_image->at<uchar>(*it) == 255 && !is_in_list(*it, *bound_list_r1) && !is_in_list(*it, *bound_list_r2))
            return false;
    }
    return true;
    

}

bool Training::is_in_list(Point p, list<Point> &l)
{
    for(auto it = l.begin(); it != l.end(); ++it)
        if(p == *it)
            return true;
    return false;
        
}

