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
#include <stdio.h>
#include "Stat.h"
#include <math.h>
#include <vector>
#include <string.h>
#include "GaussianKDE.hpp"


using namespace std;

class NaiveBayesClassifier
{
private:
    vector<Stat> train_data;
    const string train_data_path;
    double Prior, PriorNOT;
    
   
    //private class methods
    void normalizeTrainVector(vector<Stat>& train_vector, int numberfeatures);

    
public:
    void init();
    NaiveBayesClassifier(string _train_data_path):train_data_path(_train_data_path) { }
    double predict();
    void compute_prior_probs();


};
#endif /* NaiveBayesClassifier_hpp */
