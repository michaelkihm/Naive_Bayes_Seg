//
//  Training.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Training.hpp"

void Training::init(string & image_path, string& seg_file_path)
{
   
    //read .seg file
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
    Mat *region_image = new Mat(rows, cols, CV_32F);
    float region;
    while(!f.eof()){
        f>>region;
        f>>row;
        f>>col1;
        f>>col2;
        for(int i=col1;i<=col2;i++)
            region_image->at<float>(row,i) = region;
    }
    f.close();
    //------------------------------------//
    //read source image
    Mat* ground_img = new Mat;
    *ground_img = imread(image_path);
    
    //init region vector
    for(int i=0; i < segments; i++)
        region_vector.push_back(new Region);
    
    for (int r = 0; r < region_image->rows; r++) {
        for (int c = 0; c < region_image->cols; c++) {
            Point p = Point(c,r);
            float ind = region_image->at<float>(r,c);
            region_vector[ind]->push_back(p);
        }
    }
    
}
