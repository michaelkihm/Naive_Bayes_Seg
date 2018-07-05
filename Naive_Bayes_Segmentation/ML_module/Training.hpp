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
#include "Region.hpp"

using namespace std;
using namespace cv;


class Training
{
private:
    vector<Stat> train_data;
    vector<Region*> region_vector;
public:
    Training() {}
    ~Training() {}
    
    vector<Stat>* get_train_data() { return &train_data; }
    void init(string &_image_path, string& seg_file_path );
    void train();
    
    
    
};



#endif /* Training_hpp */
