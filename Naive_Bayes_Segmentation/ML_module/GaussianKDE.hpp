//
//  GaussianKDE.hpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef GaussianKDE_hpp
#define GaussianKDE_hpp

#include <iostream>
#include <stdio.h>
#include "Stat.h"
#include <math.h>
#include <vector>
#include <string.h>

using namespace std;


class GaussianKDE
{
private:
    const double eps= 4.4e-6; //maximum error epsilon
    const int r=0;            //derivative of Gaussian KDE - NB:LEAVE r=0
    vector<int>    pClusterIndex;
    vector<double> pClusterCenter, a_terms, Bterms, training_data;
    double rx, q;
    const double h;
    int K, p;
    
    //private class methods
    double factorial(int n);
    void compute_Bterms();
    void compute_a();
    void calc_cluster_center();
    void  choose_parameters();
    void calc_cluster_index();
    
    
public:
    GaussianKDE(double _h, vector<double> _training_data):h(_h),training_data(_training_data) { } //needs already normalized training data
    void init();
    double predict(double input);;
};
#endif /* GaussianKDE_hpp */
