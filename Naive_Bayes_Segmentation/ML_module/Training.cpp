//
//  Training.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Training.hpp"


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
    
    
    int cols,rows,row,col1,col2, segments;
    
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
        gt_region_vector.push_back(new Region);
    
    
    for (int r = 0; r < gt_reg_num_image->rows; r++) {
        for (int c = 0; c < gt_reg_num_image->cols; c++) {
            Point p = Point(c,r);
            float ind = gt_reg_num_image->at<float>(r,c);
            gt_region_vector[ind]->push_back(p);
        }
    }
    
}

/* *************************************************************** */
/* perform trainig                                                 */
/* adapted version of perform algorithm of Region_Growing class    */
/* *************************************************************** */
void Training::train()
{
    
    set<int> rand_set;
    float p = 0.3;
    unsigned long temp, sat_count = 0;
    bool bmerged = false;
    
    //------------------------------------//
    //call slic wrapper
    slic_wrapper();
    
    //------------------------------------//
    //init method
    init();
    
    //------------------------------------//
    //start training
    while(sat_count != 300 /*&& region_list.size() > max_regions*/)
    {
        rand_num(p, rand_set);
        temp = region_list.size();
        for(set<int>::iterator it = rand_set.begin(); it != rand_set.end(); ++it)
        {
            if(*it < region_list.size() /*-1*/ )
            {
                for(int i = *it+1; i != *it; i++)
                {
                    i >= region_list.size() - 1? i = 0 : i=i; //circular iteration for loop
                    if(*it == i) break; //assure that region does not check itself
                    if(region_list[*it]->is_adjacent(region_list[i], &region_num_img))
                    {
                        
                        double diff_stddev  = abs(region_list[*it]->getStdDev() - region_list[i]->getStdDev());
                        double diff_mean    = region_list[*it]->getMean() - region_list[i]->getMean();
                        double diff_size    = region_list[*it]->getSize() - region_list[i]->getSize();
                        double arr          = region_list[*it]->compArr(region_list[i], &region_num_img);
                        
                        if( bcan_be_merged(*it, i) ) //to implement
                        {
                            merge_regions(*it,i);
                            update_reg_num_image(*it);
                            bmerged = true; //used to only merge one region pair of each rand vector
                            break;
                        }
                    }
                }
            }
        }
        if( temp == region_list.size() )
            sat_count++;
    }
    cout << "Finished training" <<endl;
}


/* ********************************************* */
/*  checks if two adjacent regions can be merged */
/* ********************************************* */
bool Training::bcan_be_merged(int r1, int r2)
{
    return find_majority_element(r1) == find_majority_element(r2) ? true : false;
}


/* ********************************************************************************* */
/* First step uses Moore’s Voting Algorithm to get a candidate for majority element. */
/* ********************************************************************************* */
int Training::find_majority_element(int r_index)
{
    //fill vector with ground truth region number image
    list<Point> *reg_list = region_list[r_index]->getRegList();
    vector<int> v;
     for(list<Point>::iterator it = reg_list->begin(); it != reg_list->end(); ++it)
         v.push_back((int)gt_reg_num_image->at<float>(*it));
    
    
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
