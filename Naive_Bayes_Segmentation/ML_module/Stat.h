//
//  Stat.h
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 04.07.18.
//  Copyright © 2018 MK. All rights reserved.
//
// struct which includes both, the independent variables and the dependent variable.
// A vector of Stat structs is used for the training of the classifier.

#ifndef Stat_h
#define Stat_h

struct Stat
{
    //const double arr;
    //const double color;
    //const double text;
    //const int    size;
    double values[4]; // 0)colorDiff    1) textureDiff    2) size    3) arr
    bool    bmerged;
    
    Stat( double _color, double _text, double _size, double _arr, bool _bmerged)
    :values{_color, _text, _size, _arr},bmerged(_bmerged) { }
};
#endif /* Stat_h */

