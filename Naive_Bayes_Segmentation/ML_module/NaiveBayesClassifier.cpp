//
//  NaiveBayesClassifier.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "NaiveBayesClassifier.hpp"

#define STR_EQU     0

/* *********************************************** */
/* computes the prior porobabilty                  */
/* *********************************************** */
void NaiveBayesClassifier::compute_prior_probs()
{
        
    int SUMmerged=0;
    for(int i=0; i < train_data.size(); i++)
        if(train_data[i].bmerged)
            SUMmerged += 1;
    
        
    Prior = SUMmerged / train_data.size();
    PriorNOT = 1 - Prior;
   
}




//--------------------------------------------------------------------------------------//
//                          KDE FUNCTIONS                                               //
//--------------------------------------------------------------------------------------//

    

