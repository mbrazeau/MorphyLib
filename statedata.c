//
//  statedata.c
//  MorPhy2
//
//  Created by mbrazeau on 26/04/2017.
//  Copyright © 2017 brazeaulab. All rights reserved.
//
#include "morphydefs.h"
#include "mplerror.h"
#include "morphy.h"
#include "statedata.h"
#include "mpl.h"

char* mpl_skip_closure(const char *closure, const char openc, const char closec)
{
    if (*closure != openc) {
        return (char*)ERR_BAD_PARAM;
    }
    char *ret = (char*)closure;
    
    do {
        ++ret;
        if (*ret == closec) {
            return ret;
        }
    } while (*ret);
    
    return NULL;
}


int compare_char_t_states(const void *ptr1, const void *ptr2)
{
    return *(char*)ptr1 - *(char*)ptr2;
}


int mpl_compare_symbol_lists(const char* sym1, const char* sym2)
{
    for (int i = 0; sym1[i]; ++i) {
        if (!strchr(sym2, sym1[i])) {
            if (!isspace(sym1[i])) {
                return 1;
            }
        }
    }
    
    for (int i = 0; sym2[i]; ++i) {
        if (!strchr(sym1, sym2[i])) {
            if (!isspace(sym2[i])) {
                return 1;
            }
        }
    }
    
    return 0;
}


int mpl_assign_symbol_list_from_matrix
(const char *symbs, struct MPLsymbols* symlist)
{
    assert(symbs && symlist);
    
    int nsymbs = (int)strlen(symbs);
    
    ++nsymbs;
    symlist->symbolsinmatrix = (char*)calloc(nsymbs, sizeof(char));
    
    if (!symlist->symbolsinmatrix) {
        return ERR_BAD_MALLOC;
    }
    
    strcpy(symlist->symbolsinmatrix, symbs);
    
    return ERR_NO_ERROR;
}

char *mpl_query_symbols_from_matrix(Morphyp m)
{
    return m->symbols.symbolsinmatrix;
}


int mpl_get_states_from_rawdata(Morphyp handl)
{
    
    assert(handl);
    
    int count = 0;
    char *rawmatrix = handl->char_t_matrix;
    char *current = NULL;
    int listmax = MAXSTATES + 1; // +1 for terminal null.
    char statesymbols[listmax];
    int dbg_loopcount = 0;
    
    statesymbols[0] = '\0';
    current = rawmatrix;
    
    do {
//        dbg_printf("Current: %c\n", *current);
        
        if (strchr(gmpl_valid_symb, *current)) {
            
            if (strchr(gmpl_valid_matrix_punc, *current)) {
                ++current;
            }
            if (!strchr(statesymbols, *current) &&
                strchr(gmpl_valid_state, *current)) {
                // Put in list
                statesymbols[count] = *current;
                ++count;
                statesymbols[count] = '\0';
            }
        }
        else {
//            dbg_printf("Returning error\n");
            return ERR_INVALID_SYMBOL;
        }
        
        ++current;
        ++dbg_loopcount;
    } while (*current);
    
    // Sort alphanumerically
    qsort(statesymbols, strlen(statesymbols), sizeof(char),
          compare_char_t_states);
    
    dbg_printf("The state symbols: %s\n", statesymbols);
    int numstates = (int)strlen(statesymbols);
    mpl_set_numsymbols(numstates, handl);
    mpl_assign_symbol_list_from_matrix(statesymbols, &handl->symbols);
    return count-1;
}


int mpl_set_numsymbols(int numsymb, Morphyp handl)
{
    dbg_printf("Setting numsymbols\n");
    assert(handl);
    handl->symbols.numstates = numsymb;
    return ERR_NO_ERROR;
}

int mpl_get_numsymbols(Morphyp handl)
{
    dbg_printf("Getting numsymbols\n");
    assert(handl);
    return handl->symbols.numstates;
}


int mpl_create_state_dictionary(Morphyp handl)
{
//    dbg_printf("Creating state dictionary\n");
//    assert(handl->symboldict);
//    
//    char* states = handl->symboldict->rawsymbols;
//    assert(!strchr(states, ' '));
//    
//    mpl_set_numsymbols((int)strlen(states), handl);
//    int maxsymb = mpl_get_numsymbols(handl);
//    
//    handl->symboldict->symbols = (MPL_stsymb*)calloc(maxsymb,
//                                                     sizeof(MPL_stsymb));
//    if (!handl->symboldict->symbols) {
//        return ERR_BAD_MALLOC;
//    }
//    
//    for (int i = 0; i < maxsymb; ++i) {
//        handl->symboldict->symbols[i].aschar = states[i];
//        dbg_printf("Converting symbol: %c\n", states[i]);
//    }
    
    return ERR_NO_ERROR;
}


int mpl_init_symbolset(Morphyp m)
{
    if (!mpl_get_numsymbols(m)) {
//        <#statements#>
    }
    return ERR_NO_ERROR;
}


void mpl_destroy_symbolset(Morphyp m)
{
    assert(m);
    if (m->symbols.statesymbols) {
        free(m->symbols.statesymbols);
        m->symbols.statesymbols = NULL;
    }
}

bool mpl_is_valid_matrix_symbol(const char c)
{
    if (strchr(gmpl_valid_state, c)) {
        return true;
    }
    else if (strchr(gmpl_valid_matrix_wildcard, c)) {
        return true;
    }
    else if (strchr(gmpl_valid_matrix_punc, c)) {
        return true;
    }
    
    return false;
}

unsigned long mpl_get_valid_matrix_length(const char* rawmatrix)
{
    unsigned long len = 0;
    char* matptr = (char*)rawmatrix;
    
    do {
        if (mpl_is_valid_matrix_symbol(*matptr)) {
            ++len;
        }
        else if (*matptr == '[') {
            matptr = mpl_skip_closure(matptr, '[', ']');
            assert(!(matptr < 0));
        }
        ++matptr;
    } while (*matptr);
    
    return len;
}

void mpl_copy_valid_matrix_data(char *copy, const char* rawmatrix)
{
    int i = 0;
    char* matptr = (char*)rawmatrix;
    
    do {
        if (mpl_is_valid_matrix_symbol(*matptr)) {
            copy[i] = *matptr;
            ++i;
        }
        else if (*matptr == '[') {
            matptr = mpl_skip_closure(matptr, '[', ']');
            assert(!(matptr < 0));
        }
        ++matptr;
    } while (*matptr);
    
    copy[i] = '\0';
    dbg_printf("The truncated matrix: %s\n", copy);
}

// Copy the raw matrix, take out whitespace and comments
int mpl_copy_raw_matrix(const char* rawmatrix, Morphyp handl)
{
    unsigned long len = mpl_get_valid_matrix_length(rawmatrix);
    
    char *matcpy = (char*)calloc(len + 1, sizeof(char));
    if (!matcpy) {
        return ERR_BAD_MALLOC;
    }
    mpl_copy_valid_matrix_data(matcpy, rawmatrix);
    handl->char_t_matrix = matcpy;
    return ERR_NO_ERROR;
}

int mpl_check_nexus_matrix_dimensions
(char *preproc_matrix, int input_num_taxa, int input_num_chars)
{
    /* An input matrix should not have inline taxon names. This function
     * scans each row of the input matrix to determine whether or not the
     * number of places in the row corresponds to input number of
     * of characters. If the number exceeds the expected number of data
     * columns (num_input_chars), then it is inferred that taxon names or
     * other extraneous info are included in the matrix. */
    
    char* current = NULL;
    int matrix_size = 0;
    int expected_size = 0;
    
    expected_size = input_num_chars * input_num_taxa;
    
    current = preproc_matrix;
    
    do {
        if (strchr(gmpl_valid_state, *current)
            || strchr(gmpl_valid_matrix_wildcard, *current)) {
            ++matrix_size;
        }
        else if (*current == '(' || *current == '{') {
            
            char* err = 0;
            
            if (*current == '(') {
                err = mpl_skip_closure(current, '(', ')');
            }
            else {
                err = mpl_skip_closure(current, '{', '}');
            }
            if (err < 0) {
                return ERR_MATCHING_PARENTHS;
            }
            
            current = err;
            assert(!(current < 0));
            ++matrix_size;
        }

        ++current;
    } while (*current);
    
    dbg_printf("Expected dimensions: %i\n", expected_size);
    dbg_printf("matrix length: %i\n", matrix_size);
    
    if (matrix_size > expected_size) {
        return ERR_DIMENS_UNDER;
    }
    else if (matrix_size < expected_size) {
        return ERR_DIMENS_OVER;
    }
    
    return ERR_NO_ERROR;
}


char* mpl_get_preprocessed_matrix(Morphyp handl)
{
    assert(handl);
    return handl->char_t_matrix;
}

Mstates mpl_gap_value(Morphyp handl)
{
    switch (mpl_get_gaphandl(handl)) {
        case GAP_INAPPLIC:
            return NA;
        case GAP_MISSING:
            return MISSING;
        case GAP_NEWSTATE:
            return (Mstates)1;
        case GAP_MAX:
            return -1;
        default:
            break;
    }
    
    return -2;
}

MPLmatrix* mpl_new_mpl_matrix
(const int ntaxa, const int nchar, const int nstates)
{
    assert(nstates);
    MPLmatrix* ret = NULL;
    
    ret = (MPLmatrix*)calloc(1, sizeof(MPLmatrix));
    if (!ret) {
        return NULL;
    }
    
    ret->intweights = (int*)calloc(nchar, sizeof(int));
    if (!ret->intweights) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->fltweights = (Mflt*)calloc(nchar, sizeof(Mflt));
    if (!ret->fltweights) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->cells = (MPLcell*)calloc(ntaxa * nchar, sizeof(MPLcell));
    if (!ret->cells) {
        mpl_delete_mpl_matrix(ret);
        return NULL;
    }
    
    ret->ncells = ntaxa * nchar;
    
    for (int i = 0; i < ret->ncells; ++i) {
        ret->cells[i].asstr = (char*)calloc(nstates + 1, sizeof(char));
        if (!ret->cells[i].asstr) {
            for (int j = 0; j < i; ++j) {
                free(ret->cells[i].asstr);
                ret->cells[i].asstr = NULL;
            }
            mpl_delete_mpl_matrix(ret);
            return NULL;
        }
    }
    
    
    return ret;
}

int mpl_delete_mpl_matrix(MPLmatrix* m)
{
    if (!m) {
        return ERR_BAD_PARAM;
    }
    
    if (m->cells) {
        for (int i = 0; i < m->ncells; ++i) {
            if (m->cells[i].asstr) {
                free(m->cells[i].asstr);
                m->cells[i].asstr = NULL;
            }
        }
        free(m->cells);
    }
    
    if (m->fltweights) {
        free(m->fltweights);
        m->fltweights = NULL;
    }
    
    if (m->intweights) {
        free(m->intweights);
        m->intweights = NULL;
    }
    
    return ERR_NO_ERROR;
}

MPLmatrix* mpl_get_mpl_matrix(Morphyp m)
{
    return m->inmatrix;
}

int mpl_set_gap_push(Morphyp handl)
{
    gap_t gt = mpl_get_gaphandl(handl);
    
    if (gt == GAP_INAPPLIC || gt == GAP_NEWSTATE) {
        return 1;
    }
    else if (gt == GAP_MISSING) {
        return 0;
    }
    
    return -1;
}


int mpl_get_uncorrected_shift_value(char symb, Morphyp handl)
{
    // Gets the raw shift value as determined by the order in the symbols list
    assert(symb != DEFAULTGAP && symb != DEFAULTMISSING);
    int shift = 0;
    char* symbols = mpl_get_symbols((Morphy)handl);
    
    while (*symbols != symb && *symbols) {
        ++symbols;
        ++shift;
    }
    
    return shift;
}


void mpl_use_symbols_from_matrix(Morphyp handl)
{
    handl->symbols.statesymbols = handl->symbols.symbolsinmatrix;
}


int mpl_convert_rawdata(Morphyp handl)
{
    int ret = ERR_NO_ERROR;
    
    mpl_get_states_from_rawdata(handl);
    
    // If no symbols supplied by the user
    if (!mpl_get_symbols((Morphy)handl)) {
        // Assign internal symbols to list
        mpl_use_symbols_from_matrix(handl);
    }
    else {
        char *frommatrix = mpl_query_symbols_from_matrix(handl);
        char *user = mpl_get_symbols((Morphyp)handl);
        if (mpl_compare_symbol_lists(frommatrix, user)) {
            return ERR_SYMBOL_MISMATCH;
        }
    }
    
    handl->inmatrix = mpl_new_mpl_matrix(handl->numtaxa,
                                         handl->numcharacters,
                                         mpl_get_numsymbols(handl));
    if (!handl->inmatrix) {
        return ERR_BAD_MALLOC;
    }
    
    // Write the characters as strings into their cells
    
    // Loop over the matrix and find out which ones have
    // Create a state dictionary
    // Probably multipe dictionaries...? Regular and w/ NA?
    mpl_create_state_dictionary(handl);
    // Set shift values in the dictionary
    // Use dictionary to convert
    
    return ret;
}

