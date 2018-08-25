//
//  NaiveBayesClassifier.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "NaiveBayesClassifier.hpp"

#define STR_EQU 0


/* *********************************************** */
/* deconstructor                                   */
/* *********************************************** */
NaiveBayesClassifier::~NaiveBayesClassifier()
{
    delete like_color;      delete like_color_NOT;
    delete  like_texture;   delete like_texture_NOT;
    delete  like_arr;       delete like_arr_NOT;
    delete  like_size;      delete like_size_NOT;
}

/* *********************************************** */
/* read training data and initialize KDE's         */
/* *********************************************** */
void NaiveBayesClassifier::init()
{
    //------------------------------------//
    //load training data file
    load_train_data();
    
    //------------------------------------//
    //compute prior probabilities
    compute_prior_probs();
    
    //for testing - get min and max values of training data
    CalcVectorMaxandMin();
    
    //------------------------------------//
    //normalize trainings data
    normalizeTrainVector();
    cout << "normalized!!"<< endl;
    //------------------------------------//
   
    //------------------------------------//
    //initialize kernel density estimators
    like_color = new GaussianKDE(h_color, extract_feature_vec(0,true));
    like_color_NOT = new GaussianKDE(h_NOT_color, extract_feature_vec(0,false));
    
    like_texture = new GaussianKDE(h_texture, extract_feature_vec(1,true));
    like_texture_NOT = new GaussianKDE(h_NOT_texture, extract_feature_vec(1,false));
    
    like_arr = new GaussianKDE(h_arr, extract_feature_vec(2,true));
    like_arr_NOT = new GaussianKDE(h_NOT_arr, extract_feature_vec(2,false));
    
    like_size = new GaussianKDE(h_size, extract_feature_vec(3,true));
    like_size_NOT = new GaussianKDE(h_NOT_size, extract_feature_vec(3,false));
    
    //------------------------------------//
    //
}



/* *********************************************** */
/* Loads the training                              */
/* *********************************************** */
void NaiveBayesClassifier::load_train_data()
{
    ifstream fin(train_data_path);
    double   color,texture,arrangement, size;
    bool    b_merged;
    
    while(!fin.eof()){
        fin>>color;
        fin>>texture;
        fin>>size;
        fin>>arrangement;
        fin>>b_merged;
        
        train_data.push_back(Stat(color,texture,size,arrangement,b_merged));
    }
    cout << "size training data: "<< train_data.size() << endl;
}

/* *********************************************** */
/* computes the prior porobabilty                  */
/* *********************************************** */
void NaiveBayesClassifier::compute_prior_probs()
{
    
    int SUMmerged=0;
    for(int i=0; i < train_data.size(); i++)
        if(train_data[i].bmerged)
            SUMmerged += 1;
    
    
    Prior = SUMmerged / (double)train_data.size();
    PriorNOT = 1 - Prior;
    cout << "Priors "<<Prior << " "<< PriorNOT << endl;
}
    

/* ***************************************************************************************************** */
/*                              sets the bandwidth for the KDE                                           */
/* ***************************************************************************************************** */
void NaiveBayesClassifier::set_bandwidth_KDE(const string &feature, const double h, const double h_NOT)
{
    if(strcmp(feature.c_str(), "color") == STR_EQU)
    {
        h_color = h;
        h_NOT_color = h_NOT;
    }
    else if(strcmp(feature.c_str(), "texture") == STR_EQU)
    {
        h_texture = h;
        h_NOT_texture = h_NOT;
    }
    else if(strcmp(feature.c_str(), "arrangement") == STR_EQU)
    {
        h_arr = h;
        h_NOT_arr = h_NOT;
    }
    if(strcmp(feature.c_str(), "size") == STR_EQU)
    {
        h_size = h;
        h_NOT_size = h_NOT;
    }
    else
        cerr << "Can not set KDE bandwidth for given string" << endl;
}

/* *********************************************** */
/* returns the given featur vector                 */
/* *********************************************** */
vector<double> NaiveBayesClassifier::extract_feature_vec(int col, bool merged)
{
    vector<double> vec;
    if(merged)
    {
        for(int i=0; i < train_data.size(); i++)
            if(train_data[i].bmerged)
                vec.push_back( train_data[i].values[col]);
    }
    else
    {
        for(int i=0; i < train_data.size(); i++)
            if(!train_data[i].bmerged)
                vec.push_back( train_data[i].values[col]);
    }
    if(vec.size() <= 1)
        cerr << "something wrong with vector given to KDE!!" << endl;
    return vec;
}



/* *********************************************** */
/* normalizes the loaded trining data              */
/* *********************************************** */
void  NaiveBayesClassifier::normalizeTrainVector(){
    int numberfeatures =4;
    for (int col=0; col< numberfeatures; col++)
    {
        double max=0;
        for(int i=0; i < train_data.size();i++){
            if (train_data[i].values[col] > max){
                max = train_data[i].values[col];
            }
        }
        double min = max;
        for(int i=0; i < train_data.size();i++){
            if (train_data[i].values[col] < min){
                min = train_data[i].values[col];
            }
        }
        for (int i=0; i < train_data.size();i++)
            train_data[i].values[col] = (train_data[i].values[col] -min)/(max-min);
        
    }
    
    
}


/* *********************************************** */
/*  compute max and min values of feature vector.  */
/*  Required to normalize the training data        */
/* *********************************************** */
void NaiveBayesClassifier::CalcVectorMaxandMin (){
    for(int col=0; col < 4; col++)
    {
    float max=0;
    for(int i=0; i < train_data.size();i++){
        if (train_data[i].values[col] > max){
            max = train_data[i].values[col];
        }
    }
    float min = max;
    for(int i=0; i < train_data.size();i++){
        if (train_data[i].values[col] < min){
            min = train_data[i].values[col];
        }
    }
    switch ( col )  {
        case 0:
        {
            maxColor = max;
            minColor = min;
            break;
        }
        case 1:
        {
            maxTexture = max;
            minTexture = min;
            break;
        }
        case 2:
        {
            maxSize = max;
            minSize = min;
            break;
        }
        case 3:
        {
            maxArrangement = max;
            minArrangement = min;
            break;
        }
            
    }
    }
    
}

/* *********************************************************************************************************** */
/*                  applies Naive Bayes formula and returns the                                                */
/*                  merging probability of two adjacent regions                                                */
/* *********************************************************************************************************** */
double NaiveBayesClassifier::predict(double diff_color,double diff_text, double diff_arr, double diff_size)
{
    
    double color = like_color->predict((diff_color-minColor)/(maxColor-minColor)), color_NOT = like_color_NOT->predict((diff_color-minColor)/(maxColor-minColor));
    double text = like_texture->predict((diff_text-minTexture)/(maxTexture-minTexture)), text_NOT = like_texture_NOT->predict((diff_text-minTexture)/(maxTexture-minTexture));
    double arr = like_arr->predict(diff_arr), arr_NOT = like_arr->predict(diff_arr);
    double size = like_size->predict((diff_size-minSize)/(maxSize - minSize)), size_NOT = like_size_NOT->predict((diff_size-minSize)/(maxSize - minSize));
    
    double out = (color*text*arr*size*Prior)/((color*text*arr*size*Prior)+(color_NOT*text_NOT*arr_NOT*size_NOT*PriorNOT));
    if(out < 0 || out > 1)
        cerr << "wrong merging probability!! Value: " <<out << endl;
    return out;
    
}
