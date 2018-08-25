//
//  GaussianKDE.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 29.07.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "GaussianKDE.hpp"

/* *********************************************** */
/* initializes the likelihood estimation           */
/* *********************************************** */
void GaussianKDE::init()
{
    choose_parameters();
    calc_cluster_center();
    calc_cluster_index();
    compute_a();
    compute_Bterms();
}

/* *********************************************** */
/* computes the likelihoods                        */
/* *********************************************** */
double GaussianKDE::predict(double input)
{
    
    double likelihood=0;
    double *temp3;
    temp3=new double[p+r];
    
    double h_square = h*h;
    double two_h_square = 2*h_square;
    double r_term=sqrt((double)factorial(r));
    double rr = 2*h*sqrt(log(r_term/eps));
    double ry = rx+rr;
    
    
    int target_cluster_number=min((int)floor(input/rx),K-1);
    
    double temp1=input-pClusterCenter[target_cluster_number];
    double dist=abs(temp1);
    while (dist <= ry && target_cluster_number <K && target_cluster_number >=0){
        
        
        double temp2=exp(-temp1*temp1/two_h_square);
        double temp1h=temp1/h;
        temp3[0]=1;
        for(int i=1; i<p+r; i++){
            temp3[i]=temp3[i-1]*temp1h;
        }
        
        for(int k=0; k<=p-1; k++){
            int dummy=0;
            for(int l=0; l <= (int)floor((double)r/2); l++){
                for(int m=0; m <= r-(2*l); m++){
                    
                    likelihood += (a_terms[dummy]*Bterms[(target_cluster_number*p*(r+1))+((r+1)*k)+m]*temp2*temp3[k+r-(2*l)-m]);
                    dummy +=1;
                }
            }
        }
        
        
        
        target_cluster_number +=1;;
        temp1=input-pClusterCenter[target_cluster_number];
        dist=abs(temp1);
    }
    
    target_cluster_number=min((int)floor(input/rx),K-1)-1;
    if (target_cluster_number >=0){
        double temp1= input -pClusterCenter[target_cluster_number];
        double dist=abs(temp1);
        while (dist <= ry && target_cluster_number <K && target_cluster_number >=0){
            
            double temp2=exp(-(temp1*temp1/two_h_square));
            double temp1h=temp1/h;
            temp3[0]=1;
            for(int i=1; i<p+r; i++){
                temp3[i]=temp3[i-1]*temp1h;
            }
            
            for(int k=0; k<=p-1; k++){
                int dummy=0;
                for(int l=0; l <= (int)floor((double)r/2); l++){
                    for(int m=0; m <= r-(2*l); m++){
                        likelihood += (a_terms[dummy]*Bterms[(target_cluster_number*p*(r+1))+((r+1)*k)+m]*temp2*temp3[k+r-(2*l)-m]);
                        dummy++;
                    }
                }
            }
            
            target_cluster_number -=1;
            temp1= input -pClusterCenter[target_cluster_number];
            dist=abs(temp1);
        }
    }
    
    delete temp3;
    return likelihood;
}




/* *********************************************** */
/* computes the factorial of an integer n          */
/* *********************************************** */
double GaussianKDE::factorial(int n){
    
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
void  GaussianKDE::choose_parameters(/*double h, int N, double& rx, int &K, int &p, double &q*/){
    
     //choose_parameters( hColor, (int)colorTDM.size(), rx_colorTDM, K_colorTDM, p_colorTDM, q_colorTDM);
    rx=h/2;
    int N = training_data.size();
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
    q=(pow(-1,r))/(sqrt(2*M_PI)*N*(pow(h,(r+1))));
}



/* *************************************************************************************************************** */
/*              computation of the B-terms  (needed for Fast Gaussian KDE approximation                            */
/* *************************************************************************************************************** */
void GaussianKDE::compute_Bterms()
{
    
    int N = training_data.size();
    int num_of_B_terms=K*p*(r+1);
    vector<float> resultVec(num_of_B_terms);
    double *k_factorial;
    k_factorial=new double[p];
    k_factorial[0]=1;
    
    
    for(int i=1; i<p ;i++)
        k_factorial[i]= k_factorial[i-1]/i;
    
    
    
    double *temp3;
    temp3=new double[p+r];
    
    for(int n=0; n<K; n++){
        for(int k=0; k<p; k++){
            for(int m=0; m< r+1; m++){
                resultVec[(n*p*(r+1))+((r+1)*k)+m]=0.0;
                
            }
        }
        
    }
    
    
    for(int i=0; i<N; i++)
    {
        int cluster_number=pClusterIndex[i];
        double temp1=(training_data[i]-pClusterCenter[cluster_number])/h;
        double temp2=exp(-temp1*temp1/2);
        temp3[0]=1;
        for(int k=1; k<p+r; k++)
            temp3[k]=temp3[k-1]*temp1;
        
        for(int k=0; k<p; k++)
            for(int m=0; m< r+1; m++)
                resultVec[(cluster_number*p*(r+1))+((r+1)*k)+m] += (temp2*temp3[k+m]);
    }
    
    for(int n=0; n<K; n++){
        for(int k=0; k<p; k++){
            for(int m=0; m< r+1; m++){
                resultVec[(n*p*(r+1))+((r+1)*k)+m] *=(k_factorial[k]*q);
                
            }
        }
        
    }
    
    for(int i=0; i<resultVec.size();i++)
        Bterms.push_back(resultVec[i]);
    
    delete temp3;
    
}


/* *************************************************************************************************************** */
/*          computation of the a-terms  (needed for Fast Gaussian KDE approximation,                               */
/*          especially for computing derivatives of the Gaussian                                                   */
/* *************************************************************************************************************** */
void GaussianKDE::compute_a(){

    double r_factorial= factorial(r);
    
    double *l_constant;
    l_constant=new double[((int)floor((double)r/2))+1];
    l_constant[0]=1;
    for(int l=1; l <= (int)floor((double)r/2); l++)
        l_constant[l]=l_constant[l-1]*(-1.0/(2*l));
        
    
    
    double *m_constant;
    m_constant=new double[r+1];
    m_constant[0]=1;
    for(int m=1; m <= r; m++)
        m_constant[m]=m_constant[m-1]*(-1.0/m);
        
    
    
    int num_of_a_terms=0;
    for(int l=0; l <= (int)floor((double)r/2); l++)
        for(int m=0; m <= r-(2*l); m++)
            num_of_a_terms +=1;
  
    double *a_terms_temp = new double[num_of_a_terms];
    int k=0;
    for(int l=0; l <= (int)floor((double)r/2); l++)
    {
        for(int m=0; m <= r-(2*l); m++){
            a_terms_temp[k]=(l_constant[l]*m_constant[m]*r_factorial)/((double)factorial(r-(2*l)-m));
            k++;
        }
    }
    
    for(int i=0; i<num_of_a_terms; i++)
        a_terms.push_back(a_terms_temp[i]);
    
    
    delete [] a_terms_temp;
    delete m_constant;
}

/* *********************************************************************************** */
/*          computation of the cluster centers                                         */
/* *********************************************************************************** */
void GaussianKDE::calc_cluster_center()
{
    for(int i=0; i<K; i++)
        pClusterCenter.push_back((i*rx)+(rx/2));
}


/* ******************************************************************************************* */
/*          calculate corrisponidng cluster indices for each entry of the training data        */
/* ******************************************************************************************* */
void GaussianKDE::calc_cluster_index()
{
    int N = training_data.size();
    for(int i=0; i<N; i++)
        pClusterIndex.push_back(min((int)floor(training_data[i]/rx),K-1));
}
