//
//  NaiveBayesClassifier.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "NaiveBayesClassifier.hpp"



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


/* *********************************************** */
/* computes the factorial of an integer n          */
/* *********************************************** */
double NaiveBayesClassifier::factorial(int n){
    
    int fact=1;
    if (n == 0)
        return 1;
    else{
        for ( int i = 1; i <= n; i++){
            fact=fact*i;
        }
    }
    return fact;
}


/* ****************************************************************************************************** */
/*          computation of the parameters of the Fast Gaussian KDE approximation                          */
/* ****************************************************************************************************** */
void  NaiveBayesClassifier::choose_parameters(double h, int N, double& rx, int &K, int &p, double &q){
    
   // vector<float> output(4);
    //rx: interval length.
    
    rx=h/2;
    
    //K: number of intervals.
    K=(int)ceil(1.0/rx);
    rx= 1.0/K;
    double rx_square= rx*rx;
    double h_square = pow(h,2);
    double two_h_square = 2*h_square;
    
    //rr: cutoff radius.
    double r_term=sqrt((double)factorial(r));
    
    
    double rr = 2*h*sqrt(log(r_term/eps));
    
    //ry: cluster cutoff radius.
    double ry = rx+rr;
    
    //p: truncation number.
    
    p=0;
    double error=1;
    double temp=1;
    double comp_eps=eps/r_term;
    
    while((error > comp_eps) & (p <= 500)){
        p+=1;
        double b=min(((rx+sqrt((rx_square)+(8*p*h_square)))/2),ry);
        double c=rx-b;
        temp=temp*(((rx*b)/h_square)/p);
        error=temp*(exp(-(c*c)/2*two_h_square));
    }
    p=p+1;
    //calculate q
    q=(pow(-1,r))/(sqrt(2*M_PI)*N*(pow(h,(r+1))));

}


/* *********************************************************************** */
/*       normalizes the training vector vector                             */
/* *********************************************************************** */
void NaiveBayesClassifier::normalizeTrainVector(vector<Stat>& train_vector, int numberfeatures){
 
    for (int col=0; col< numberfeatures; col++){
        float max=0;
        for(int i=0; i < train_vector.size();i++){
            if (train_vector[i].values[col] > max){
                max = train_vector[i].values[col];
            }
        }
        float min = max;
        for(int i=0; i < train_vector.size();i++){
            if (train_vector[i].values[col] < min){
                min = train_vector[i].values[col];
            }
        }
        for (int i=0; i < train_vector.size();i++){
            train_vector[i].values[col] = (train_vector[i].values[col] -min)/(max-min);
        }
    }
    
}


/* *************************************************************************************************************** */
/*              computation of the B-terms  (needed for Fast Gaussian KDE approximation                            */
/* *************************************************************************************************************** */
void NaiveBayesClassifier::compute_Bterms(int K, int p, int r, double h, int N, double q,  vector<int>  pClusterIndex,  vector<float> pClusterCenter,  vector<float> inputVec, vector<float>& BtermVec){
    
    
    
    int num_of_B_terms=K*p*(r+1);
    vector<float> resultVec(num_of_B_terms);
    double *k_factorial;
    k_factorial=new double[p];
    
    k_factorial[0]=1;
    for(int i=1; i<p ;i++){
        k_factorial[i]= k_factorial[i-1]/i;
        
    }
    
    double *temp3;
    temp3=new double[p+r];
    
    for(int n=0; n<K; n++){
        for(int k=0; k<p; k++){
            for(int m=0; m< r+1; m++){
                resultVec[(n*p*(r+1))+((r+1)*k)+m]=0.0;
                
            }
        }
        
    }
    
    
    
    
    for(int i=0; i<N; i++){
        int cluster_number=pClusterIndex[i];
        double temp1=(inputVec[i]-pClusterCenter[cluster_number])/h;
        double temp2=exp(-temp1*temp1/2);
        temp3[0]=1;
        for(int k=1; k<p+r; k++){
            temp3[k]=temp3[k-1]*temp1;
        }
        
        for(int k=0; k<p; k++){
            for(int m=0; m< r+1; m++){
                resultVec[(cluster_number*p*(r+1))+((r+1)*k)+m] += (temp2*temp3[k+m]);
            }
        }
    }
    
    for(int n=0; n<K; n++){
        for(int k=0; k<p; k++){
            for(int m=0; m< r+1; m++){
                resultVec[(n*p*(r+1))+((r+1)*k)+m] *=(k_factorial[k]*q);
                
            }
        }
        
    }
    
    for(int i=0; i<resultVec.size();i++){
        BtermVec.push_back(resultVec[i]);
    }
    
    
}


/* *************************************************************************************************************** */
/*          computation of the a-terms  (needed for Fast Gaussian KDE approximation,                               */
/*          especially for computing derivatives of the Gaussian                                                   */
/* *************************************************************************************************************** */
void NaiveBayesClassifier::compute_a(int r, vector<float>& a_termsOut){
    
    double r_factorial= factorial(r);
    
    double *l_constant;
    l_constant=new double[((int)floor((double)r/2))+1];
    l_constant[0]=1;
    for(int l=1; l <= (int)floor((double)r/2); l++){
        l_constant[l]=l_constant[l-1]*(-1.0/(2*l));
        
    }
    
    double *m_constant;
    m_constant=new double[r+1];
    m_constant[0]=1;
    for(int m=1; m <= r; m++){
        m_constant[m]=m_constant[m-1]*(-1.0/m);
        
    }
    
    int num_of_a_terms=0;
    for(int l=0; l <= (int)floor((double)r/2); l++){
        for(int m=0; m <= r-(2*l); m++){
            num_of_a_terms +=1;
        }
    }
    
    double *a_terms;
    a_terms= new double[num_of_a_terms];
    int k=0;
    for(int l=0; l <= (int)floor((double)r/2); l++){
        for(int m=0; m <= r-(2*l); m++){
            a_terms[k]=(l_constant[l]*m_constant[m]*r_factorial)/((double)factorial(r-(2*l)-m));
            k++;
        }
    }
    
    for(int i=0; i<num_of_a_terms; i++){
        a_termsOut.push_back(a_terms[i]);
    }
    
}
