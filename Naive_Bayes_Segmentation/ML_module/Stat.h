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
    const double arr;
    const double color;
    const double text;
    const bool    bmerged;
    
    Stat( double _arr, double _color, double _text, bool _bmerged):arr(_arr),color(_color),text(_text),bmerged(_bmerged) { }
};
#endif /* Stat_h */
