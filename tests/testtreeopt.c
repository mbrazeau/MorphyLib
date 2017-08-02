//
//  testtreeopt.c
//  morphylib
//
//  Created by mbrazeau on 09/07/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//

#include "mpltest.h"
#include "../src/morphydefs.h"


int test_do_fullpass_on_tree(TLtree* t, Morphy m)
{
    int length = 0;
    
    int i = 0;
    int end = 0;
    
    int index = 0;
    int postorder[2 * t->ntaxa];
    tl_traverse_tree(t->start, &index, postorder);
    end = index-1;
    printf("\n");
    for (i = 0; i <= end; ++i)
    {
        TLnode* n = &t->trnodes[postorder[i]];
        
        if (!n->tip) {
            length += mpl_first_down_recon(n->index, I_LDESC(n->index, t), I_RDESC(n->index, t), m);
        }
    }
    
    mpl_update_lower_root(I_ANCESTOR(t->trnodes[end].index, t), t->trnodes[end].index, m);
    
    for (i = end; i >= 0; --i) {
        TLnode* n = &t->trnodes[postorder[i]];
        
        if (n->tip != 0) {
            mpl_update_tip(n->index, I_ANCESTOR(n->index, t), m);
        } else {
            mpl_first_up_recon(n->index, I_LDESC(n->index, t), I_RDESC(n->index, t), I_ANCESTOR(n->index, t), m);
        }
    }
    
    if (mpl_query_gaphandl(m) == GAP_INAPPLIC) {
        
        for (i = 0; i <= end; ++i)
        {
            TLnode* n = &t->trnodes[postorder[i]];
            
            if (!n->tip) {
                length += mpl_second_down_recon(n->index, I_LDESC(n->index, t), I_RDESC(n->index, t), m);
            }
        }
        
        for (i = end; i >= 0; --i) {
            TLnode* n = &t->trnodes[postorder[i]];
            
            if (n->tip != 0) {
                mpl_finalize_tip(n->index, I_ANCESTOR(n->index, t), m);
            } else {
                length += mpl_second_up_recon(n->index, I_LDESC(n->index, t), I_RDESC(n->index, t), I_ANCESTOR(n->index, t), m);
            }
        }
    }
    
    return length;
}

int test_partial_downpass_for_inapplicables(TLtree* t, Morphy m)
{
    int end = 0;
    int length = 0;
    
    int index = 0;
    int postorder[2 * t->ntaxa];
    tl_traverse_tree(t->start, &index, postorder);
    end = index-1;
    printf("\n");
    
    int i = 0;
    for (i = 0; i <= end; ++i) {
        
        TLnode* n = &t->trnodes[postorder[i]];
        
        if (!n->tip) {
            // Do the eoptimzation
            mpl_first_down_update(n->index, I_LDESC(n->index, t), I_RDESC(n->index, t), m);
            // Mark the node as in the path
            // Check whether any updates have occurred;
            // If not, break the loop
            // Check if at the root of the tree; if so, update the root
        }
        
        if (i == end) {
            // Do the root.
        }
    }
    
    // Now proceed all the way upward until at least 1 node past the clip insertion
    // (For optimisation, can push these nodes to a new list)
    // Then from the new list, pass all the way down, adding steps
    // And finally all the way back up again, adding steps.
    
    return length;
}
