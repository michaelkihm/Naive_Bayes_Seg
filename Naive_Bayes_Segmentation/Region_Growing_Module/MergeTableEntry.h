//
//  MergeTableEntry.h
//  Naive_Bayes_Segmentation
//
//  Created by Michael Kihm on 18.08.18.
//  Copyright © 2018 MK. All rights reserved.
//

#ifndef MergeTableEntry_h
#define MergeTableEntry_h
struct MergeTableEntry
{
    int r1,r2;
    float merge_prob;
    
    MergeTableEntry(int _r1, int _r2, double _merge_prob):r1(_r1),r2(_r2),merge_prob(_merge_prob) { }
    inline bool operator<(const MergeTableEntry& m1) const {return merge_prob < m1.merge_prob; }
};

#endif /* MergeTableEntry_h */
