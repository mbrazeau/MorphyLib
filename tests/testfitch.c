//
//  testfitch.c
//  MorPhy2
//
//  Created by mbrazeau on 16/05/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "mpltest.h"
#include "../src/mpl.h"
#include "../src/fitch.h"
#include "testfitch.h"
#include "ctreelib/treelib.h"
#include <string.h>

int test_small_fitch(void)
{
    theader("Testing a two-tip Fitch optimisation");
    int failn = 0;
    int ntax	= 2;
    int nchar	= 10;
    //    int numna   = 0;
    char *rawmatrix =
    "0000000010\
    1111111111;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(1, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    length = mpl_first_down_recon(2, 1, 0, m1);
    printf("The length: %i\n", length);
    
    
    mpl_delete_Morphy(m1);
    
    if (length != 9) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_weighted_small_fitch(void)
{
    theader("Testing a two-tip weighted Fitch optimisation");
    
    int failn = 0;
    int ntax	= 2;
    int nchar	= 10;
    //    int numna   = 0;
    char *rawmatrix =
    "0000000010\
     1111111111;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_charac_weight(1, 3, m1);
    mpl_set_num_internal_nodes(1, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    length = mpl_first_down_recon(2, 1, 0, m1);
    printf("The length: %i\n", length);
    
    
    mpl_delete_Morphy(m1);
    
    if (length != 11) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}


int test_small_fitch_na(void)
{
    theader("A simple test of the NA counting procedure");
    int failn = 0;
    int ntax	= 12;
    int nchar	= 2;
    char *rawmatrix =
    "11\
    11\
    00\
    00\
    --\
    --\
    --\
    --\
    10\
    10\
    01\
    01;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    //    mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(13, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]   = {13, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]  = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[] = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[] = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    // ...
    int i = 0;
    for (i = 0; i < (ntax-1); ++i) {
        length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    // Update lower root
    mpl_update_lower_root(23, 22, m1);
    
    for (i = (ntax-2); i >= 0; --i) {
        length += mpl_first_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    for (i = 0; i < ntax; ++i) {
        mpl_update_tip(i, tipancs[i], m1);
    }
    
    for (i = 0; i < (ntax-1); ++i) {
        // Second downpass
        length += mpl_second_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    for (i = (ntax-2); i >=0 ; --i) {
        // Second uppass reconstruction
        length += mpl_second_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    // TODO: Final tip calculations
    //    // Optional for basic length count, but probably necessary for
    //    // indirect optimisation.
    //    for (i = 0; i < ntax; ++i) {
    //        mpl_finalize_tip(i, tipancs[i], m1);
    //        // Update tips first time
    //    }
    
    mpl_delete_Morphy(m1);
    
    if (length != 6) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}


int test_small_fitch_withmissing(void)
{
    theader("A simple test of fitch counting with ?");
    int failn = 0;
    int ntax	= 12;
    int nchar	= 1;
    char *rawmatrix =
    "23--1\?\?--032;";
    
    Morphy m1 = mpl_new_Morphy();
    mpl_init_Morphy(ntax, nchar, m1);
    //    mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(13, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]   = {13, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]  = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[] = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[] = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    // ...
    int i = 0;
    for (i = 0; i < (ntax-1); ++i) {
        length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    // Update lower root
    mpl_update_lower_root(23, 22, m1);
    
    for (i = (ntax-2); i >= 0; --i) {
        length += mpl_first_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    for (i = 0; i < ntax; ++i) {
        mpl_update_tip(i, tipancs[i], m1);
    }
    
    for (i = 0; i < (ntax-1); ++i) {
        // Second downpass
        length += mpl_second_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    for (i = (ntax-2); i >= 0; --i) {
        // Second uppass reconstruction
        length += mpl_second_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    // TODO: Final tip calculations
    //    // Optional for basic length count, but probably necessary for
    //    // indirect optimisation.
    //    for (i = 0; i < ntax; ++i) {
    //        mpl_finalize_tip(i, tipancs[i], m1);
    //        // Update tips first time
    //    }
    
    mpl_delete_Morphy(m1);
    
    if (length != 5) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_bulk_balanced_tree_cases(void)
{
    theader("Testing bulk tree cases");
    int err = 0;
    int failn = 0;
    int ntax	= 12;
    int nchar	= 1;
    char *rawmatrices[] = {(char*)"23--1??--032;", // 0
        (char*)"1---1111---1;", // 1
        (char*)"1100----1100;", // 2
        (char*)"11-------100;", // 3
        (char*)"----1111---1;", // 4
        (char*)"01----010101;", // 5
        (char*)"01---1010101;", // 6
        (char*)"1??--??--100;", // 7
        (char*)"21--3??--032;", // 8
        (char*)"11--1??--111;", // 9
        (char*)"11--1000001-;", // 10
        (char*)"01------0101;", // 11
        (char*)"110--?---100;", // 12
        (char*)"11--1??--111;", // 13
        (char*)"210--100--21;", // 14
        (char*)"????----1???;", // 15
        (char*)"23--1----032;", // 16
        (char*)"1----1----1-;", // 17
        (char*)"-1-1-1--1-1-;", // 18
        (char*)"23--1??--032;", // 19
        (char*)"--------0101;", // 20
        (char*)"10101-----01;", // 21
        (char*)"011--?--0011;", // 22
        (char*)"110--??--100;", // 23
        (char*)"11--1000001-;", // 24
        (char*)"21--1----012;", // 25
        (char*)"11----111111;", // 26
        (char*)"10101-----01;", // 27
        (char*)"210210------;", // 28
        (char*)"----1111----;", // 29
        (char*)"230--??1--32;", // 30
        (char*)"023--??1--32;", // 31
        (char*)"023-???1--32;", // 32
        (char*)"23--1?1--023;", // 33
        (char*)"----1010----;", // 34
        (char*)"------11---1;", // 35
        (char*)"10----11---1;", // 36
        (char*)"320--??3--21;", // 37
        (char*)"-------1----;", // 38
    };
    
    int nummatrices = 37;
    
    int expected[] = {5, 2, 3, 2, 1, 5, 5, 2, 5, 2, 2, 4, 3, 2, 5, 0, 5, 2, 4, 5, 2, 4, 3, 3, 2, 5, 1, 4, 4, 0, 5, 5, 4, 5, 2, 1, 3, 5, 0};
    
    int tipancs[]= {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]   = {13, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]  = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[] = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[] = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    int j = 0;
    
    for (j = 0; j < nummatrices; ++j) {
        Morphy m1 = mpl_new_Morphy();
        mpl_init_Morphy(ntax, nchar, m1);
        //    mpl_set_gaphandl(GAP_MISSING, m1);
        err = mpl_attach_rawdata(rawmatrices[j], m1);
        if (err) {
            ++failn;
            pfail;
        }
        else {
            ppass;
        }
        mpl_set_num_internal_nodes(13, m1);
        
        mpl_apply_tipdata(m1);
        
        int length = 0;
        
        // ...
        int i = 0;
        for (i = 0; i < (ntax-1); ++i) {
            length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
        }
        
        // Update lower root
        mpl_update_lower_root(23, 22, m1);
        
        for (i = (ntax-2); i >= 0; --i) {
            length += mpl_first_up_recon
            (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
        }
        
        for (i = 0; i < ntax; ++i) {
            mpl_update_tip(i, tipancs[i], m1);
        }
        
        for (i = 0; i < (ntax-1); ++i) {
            // Second downpass
            length += mpl_second_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
        }
        
        for (i = (ntax-2); i >= 0; --i) {
            // Second uppass reconstruction
            length += mpl_second_up_recon
            (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
        }
        
        // TODO: Final tip calculations
        //    // Optional for basic length count, but probably necessary for
        //    // indirect optimisation.
        //    for (i = 0; i < ntax; ++i) {
        //        mpl_finalize_tip(i, tipancs[i], m1);
        //        // Update tips first time
        //    }
        
        
        if (length != expected[j]) {
            printf("Calculated: %i, expected: %i\n", length, expected[j]);
            ++failn;
            pfail;
        }
        else {
            ppass;
        }
        
        char* result = NULL;
        for (i = 1; i < 5; ++ i) {
            int k = 0;
            printf("Pass %i: ", i);
            for (k = 0; k < ntax; ++k) {
                result = (char*)mpl_get_stateset(k, 0, i, m1);
                printf("%s ", result);
                
            }
            printf("\n");
        }
        printf("\n");
        
        for (i = ntax; i < (ntax-2 + mpl_get_num_internal_nodes(m1)); ++i) {
            int k = 0;
            printf("Node %i: ", i);
            for (k = 1; k < 5; ++k) {
                result = (char*)mpl_get_stateset(i, 0, k, m1);
                if (!(*result)) {
                    printf(". ");
                }
                else {
                    int pad = 0;
                    int paddiff = 6;
                    pad = paddiff - (int)strlen(result);
                    int x = 0;
                    for (x = 0; x < pad && pad > 0; ++x) {
                        printf(" ");
                    }
                    printf("%s ", result);
                }
            }
            printf("\n");
        }

        mpl_delete_Morphy(m1);
    }
    
    
    return failn;
}

int test_state_retrieval(void)
{
    theader("A simple test of state retrieval");
    
    int failn = 0;
    int ntax	= 12;
    int nchar	= 1;
    char *rawmatrix =
    "21--3\?\?--032;";
//    "1100----1100;";
    
    Morphy m1 = mpl_new_Morphy();
    
    mpl_init_Morphy(ntax, nchar, m1);
//        mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    
    mpl_set_num_internal_nodes(13, m1);
    
    mpl_apply_tipdata(m1);
    
    int length = 0;
    
    int tipancs[]   = {12, 12, 13, 14, 15, 16, 21, 20, 19, 18, 17, 17};
    int ancs[]      = {13, 14, 15, 16, 22, 18, 19, 20, 21, 22, 23};
    int nodes[]     = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    int ldescs[]    = {0,  12, 13, 14, 15, 10,  9,  8,  7,  6, 16};
    int rdescs[]    = {1,   2,  3,  4,  5, 11, 17, 18, 19, 20, 21};
    
    // ...
    // ...
    int i = 0;
    for (i = 0; i < (ntax-1); ++i) {
        length += mpl_first_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    // Update lower root
    mpl_update_lower_root(23, 22, m1);
    
    for (i = (ntax-2); i >= 0; --i) {
        length += mpl_first_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    for (i = 0; i < ntax; ++i) {
        mpl_update_tip(i, tipancs[i], m1);
    }
    
    for (i = 0; i < (ntax-1); ++i) {
        // Second downpass
        length += mpl_second_down_recon(nodes[i], ldescs[i], rdescs[i], m1);
    }
    
    for (i = (ntax-2); i >= 0; --i) {
        // Second uppass reconstruction
        length += mpl_second_up_recon
        (nodes[i], ldescs[i], rdescs[i], ancs[i], m1);
    }
    
    for (i = 0; i < ntax; ++i) {
        mpl_finalize_tip(i, tipancs[i], m1);
        // Update tips first time
    }
    
    char *result = NULL;
    
    for (i = 1; i < 5; ++ i) {
        int j = 0;
        printf("Pass %i: ", i);
        for (j = 0; j < ntax; ++j) {
            
            result = (char*)mpl_get_stateset(j, 0, i, m1);
            printf("%s ", result);
            
        }
        printf("\n");
    }
    printf("\n");
    
    for (i = ntax; i < (ntax-2 + mpl_get_num_internal_nodes(m1)); ++i) {
        int j = 0;
        printf("Node %i: ", i);
        for (j = 1; j < 5; ++j) {
            result = (char*)mpl_get_stateset(i, 0, j, m1);
            if (!(*result)) {
                printf(". ");
            }
            else {
                int pad = 0;
                int paddiff = 6;
                pad = paddiff - (int)strlen(result);
                int x = 0;
                for (x = 0; x < pad && pad > 0; ++x) {
                    printf(" ");
                }
                printf("%s ", result);
            }
        }
        printf("\n");
    }
    
    
    mpl_delete_Morphy(m1);
    
    if (length != 5) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}


int test_twopass_fitch(void)
{
    theader("Testing Fitch two passes");
    int err     = 0;
    int failn   = 0;
    
    int ntax	= 10;
    int nchar	= 10;
    char *rawmatrix =
    "1010312231\
    2130233203\
    3001210203\
    2131111202\
    3302222312\
    2231202332\
    1023203131\
    1123103001\
    2222213220\
    3203321302;";
    
    TLP tlp = tl_new_TL();
    tl_set_numtaxa(ntax, tlp);
    Morphy m1 = mpl_new_Morphy();
    
    mpl_init_Morphy(ntax, nchar, m1);
    mpl_set_gaphandl(GAP_MISSING, m1);
    mpl_attach_rawdata(rawmatrix, m1);
    mpl_set_num_internal_nodes(13, m1);
    mpl_apply_tipdata(m1);
    
    char* newick = "((((1,((2,7),(5,9))),(4,8)),6),(3,10));";
    tl_attach_Newick(newick, tlp);
    tl_set_current_tree(0, tlp);
    TLtree* tree = tl_get_TLtree(tlp);
    
    
    int length = 0;
    
    length = test_do_fullpass_on_tree(tree, m1);
    
    if (length != 53) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_set_parsim_t(8, WAGNER_T, m1);
    mpl_set_parsim_t(9, WAGNER_T, m1);
    mpl_apply_tipdata(m1);
    
    length = test_do_fullpass_on_tree(tree, m1);
    
    if (length != 59) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    mpl_delete_Morphy(m1);
    tl_delete_TL(tlp);
    
    return failn;
}
