#include <stdio.h>
#include <stdlib.h>

#include "mplerror.h"
#include "mpl.h"
#include "./ctreelib/treelib.h"
#include "./ctreelib/tltree.h"


#define pfail \
    printf("[  FAIL  ] %s, line: %i in: %s\n", __FUNCTION__, __LINE__, __FILE__)

#define ppass printf("[  PASS  ] %s\n", __FUNCTION__)

#define psumf(...) \
    printf("[  ** FAILED ** ] %i times. Review output to see details.\n", fails)

#define psump printf("[  ** PASSED ** ] All tests passed.\n")

#define theader(testname) printf("\n\n\t%s\n\n", testname);

int test_do_fullpass_on_tree(TLtree* t, Morphy m);
int test_full_reoptimization_for_inapplics(TLtree* t, Morphy m);
int test_do_fullpass_for_NAs(TLtree* t, Morphy m);
