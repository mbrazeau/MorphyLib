//
//  fitch.c
//  MorPhy2
//
//  Created by mbrazeau on 02/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "morphydefs.h"
#include "mplerror.h"
#include "morphy.h"
#include "fitch.h"
#include "statedata.h"

/**/
int mpl_fitch_downpass
(MPLstate* left, MPLstate* right, MPLstate* n, int nchars)
{
    int i = 0;
    int steps = 0;
    
    for (i = 0; i < nchars; ++i) {
        if (left[i] & right[i]) {
            n[i] = left[i] & right[i];
        }
        else {
            n[i] = left[i] | right[i];
            ++steps;
        }
    }
    
    return steps;
}


int mpl_fitch_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars)
{
    
    return -1;
}


/**/
int mpl_NA_fitch_first_downpass
(MPLstate* left, MPLstate *right, MPLstate *n, int nchars)
{
    
    return -1;
}


int mpl_NA_fitch_first_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars)
{
    
    return -1;
}


int mpl_NA_fitch_second_downpass
(MPLstate* left, MPLstate* right, MPLstate* n, int nchars)
{
    int steps = 0;
    
    return steps;
}


int mpl_NA_fitch_second_uppass
(MPLstate *left, MPLstate *right, MPLstate *n, MPLstate *anc, int nchars)
{
    int steps = 0;
    
    return steps;
}

