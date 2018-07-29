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
#include "Stat.h"
#include <math.h>
#include <vector>

using namespace std;

class NaiveBayesClassifier
{
private:
    vector<Stat> train_data;
    const string train_data_path;
    double Prior, PriorNOT;
    
    double eps= 4.4e-6; //maximum error epsilon
    int r=0;            //derivative of Gaussian KDE - NB:LEAVE r=0
    //private class methods
    double factorial(int n);
    void normalizeTrainVector(vector<Stat>& train_vector, int numberfeatures);
    void compute_Bterms(int K, int p, int r, double h, int N, double q,  vector<int>  pClusterIndex,  vector<float> pClusterCenter,  vector<float> inputVec, vector<float>& BtermVec);
    void compute_a(int r, vector<float>& a_termsOut);
    
public:
    void init();
    NaiveBayesClassifier(string _train_data_path):train_data_path(_train_data_path) { }
    double predict();
    void compute_prior_probs();
    void  choose_parameters(double h, int N, double& rx, int &K, int &p, double &q);
};
#endif /* NaiveBayesClassifier_hpp */
