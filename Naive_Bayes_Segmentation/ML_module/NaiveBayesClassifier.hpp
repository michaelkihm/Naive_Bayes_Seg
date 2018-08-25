//
//  NaiveBayesClassifier.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef NaiveBayesClassifier_hpp
#define NaiveBayesClassifier_hpp

#include <iostream>
#include <string>
#include <stdio.h>
#include "Stat.h"
#include <cmath>
#include <vector>
#include <string.h>
#include "GaussianKDE.hpp"
#include<fstream>


using namespace std;

class NaiveBayesClassifier
{
private:
    vector<Stat> train_data;
    const string train_data_path;
    
    //priors
    double Prior, PriorNOT;
    
    //bandwidth for the KDE
    double h_color = 0.0135, h_NOT_color=0.0329;
    double h_texture=0.0100, h_NOT_texture=0.0559;
    double h_arr=0.0127, h_NOT_arr=0.0026;
    double h_size=0.0127, h_NOT_size=0.0123;
    
    //for testing
    float maxColor, minColor;
    float maxTexture, minTexture;
    float maxArrangement, minArrangement;
    float maxSize, minSize;
    
    //pointer to KDE
    GaussianKDE  *like_color, *like_color_NOT;
    GaussianKDE  *like_texture, *like_texture_NOT;
    GaussianKDE  *like_arr, *like_arr_NOT;
    GaussianKDE  *like_size, *like_size_NOT;
   
    //private class methods
    void  normalizeTrainVector();
    void load_train_data();
    void CalcVectorMaxandMin ();

    
public:
    NaiveBayesClassifier(string _train_data_path):train_data_path(_train_data_path) {  }
    ~NaiveBayesClassifier();
    void init();
    double predict(double diff_color,double diff_text, double diff_arr, double diff_size);
    void compute_prior_probs();
    void set_bandwidth_KDE(const string &feature ,const double h, const double h_NOT);
    vector<double> extract_feature_vec(int col, bool merged);

};
#endif /* NaiveBayesClassifier_hpp */
