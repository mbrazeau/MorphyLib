//
//  testmorphy.c
//  MorPhy2
//
//  Created by mbrazeau on 12/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
//#include "testmorphy.h"
#include "../src/morphy.h"

int test_count_gaps_basic(void)
{
    theader("Testing counting of gaps in matrix");
    int failn = 0;
    int ntax	= 6;
    int nchar	= 10;
    int numna   = 0;
    char *rawmatrix =
"0000000010\
0-001-22-0\
0-001-110-\
10(03)0101100\
1-000-0000\
0-00{01}100-0;";

    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_attach_rawdata(rawmatrix, m1);

    numna = mpl_count_gaps_in_columns((Morphyp)m1);
    
    if (numna == 2) {
        ppass;
    } else {
        ++failn;
        pfail;
    }
    
    return failn;

}

int test_partition_push_index(void)
{

    theader("Testing new index pushing in partitioning");
    int failn = 0;
    
    MPLpartition* part = mpl_new_partition(FITCH_T, false);
    if (!part) {
        ++failn;
        pfail;
        return failn;
    }
    
    int nindices    = 4;
    int indices[]   = {10, 32, 21, 110};
    int i = 0;
    
    for (i = 0; i < nindices; ++i) {
        mpl_part_push_index(indices[i], part);
    }
    
    for (i = 0; i < nindices; ++i) {
        if (indices[i] != part->charindices[i]) {
            ++failn;
            pfail;
        }
        else {
            ppass;
        }
    }
    
    if (part->ncharsinpart != nindices) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (part->ncharsinpart != part->maxnchars) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_partition(part);
    
    return failn;
}
