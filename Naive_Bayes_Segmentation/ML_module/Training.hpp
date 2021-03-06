//
//  Training.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.
//
// class if implemented to train on one ground truth image. output is a vector of "Stat"
// structs for one gound truth image.

#ifndef Training_hpp
#define Training_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Stat.h"
//#include "Region.hpp"
#include "Region_Growing.hpp"

using namespace std;
using namespace cv;


class Training : private Region_Growing
{
private:
    vector<Stat> train_data;
    vector<unique_ptr<Region> > gt_region_vector;
    string image_path;
    string seg_file_path;
    Mat *gt_reg_num_image;
    Mat *gt_edge_image;
    
    int segments;
public:
    Training(string& _image_path, string& _seg_file_path /*, Mat* _src_img*/)
    :Region_Growing(/*_src_img*/_image_path), image_path(_image_path), seg_file_path(_seg_file_path) { }
    
    
    ~Training() {}
    
    vector<Stat>* get_train_data() { return &train_data; }
    void init_gt();
    void create_gt_edge_image();
    void train();
    void save_train_data(); //to be implemented
    bool bcan_be_merged(int r1, int r2);
    int  find_majority_element(/*vector<int> v, */int r_index);
    bool boundary_test(int r1, int r2);
    bool is_in_list(Point p, list<Point> &l);
};



#endif /* Training_hpp */
