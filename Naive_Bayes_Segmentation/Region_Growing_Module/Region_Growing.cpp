//
//  Region_Growing.cpp
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 22.06.18.
//  Copyright © 2018 MK. All rights reserved.
//

#include "Region_Growing.hpp"

Region_Growing::Region_Growing(Mat *src):src_rgb_img(src)
{
    cvtColor(*src_rgb_img, src_gray_img, CV_RGB2GRAY);
    Region::setSourceImages(src_rgb_img, &src_gray_img);  
}