
#include "headers.h"

myCMS * cmsInit(int width, int depth) {
    int i, j;
    myCMS* cms;
    cms = (myCMS*)malloc(sizeof(myCMS));
    cms->depth_ = depth;
    cms->width_ = width;
    cms->count_ = (int**)calloc(cms->depth_,sizeof(int*));
    for (i = 0; i < cms->depth_; i++) {
        cms->count_[i] = (int*)calloc(cms->width_, sizeof(int));
    }
    /* Initialize the array with 0 */
    for (i = 0; i < cms->depth_; i++) {
        for (j = 0; j< cms->width_; j++) {
            cms->count_[i][j] = 0;
        }
    }

    /* randInit is placed in myhash.c */
    /* this program init function of random num generator */
    randInit((unsigned int)time(NULL));
    cms->a_ = (unsigned int*)calloc(cms->depth_, sizeof(unsigned int));
    cms->b_ = (unsigned int*)calloc(cms->depth_, sizeof(unsigned int));
    for (i = 0; i < cms->depth_; i++) {
        cms->a_[i] = urand();
        cms->b_[i] = urand();
    }
    return cms;
}

void cmsUpdate(myCMS* cms, long key, int num) {
    int i;
    for (i = 0; i < cms->depth_; i++) {
        int a, b;
        int my_width;
        a = cms->a_[i];
        b = cms->b_[i];
        my_width = myHash31(a, b, key) % cms->width_;
        cms->count_[i][my_width] += num;
    }
}

uint64_t getEst (myCMS* cms, long key) {
    uint64_t mini_val = ULONG_MAX;
    int i;
    for (i = 0; i < cms->depth_; i++){
        int a, b, tmp_width;
        a = cms->a_[i];
        b = cms->b_[i];
        tmp_width = myHash31(a, b, key) % cms->width_;
        uint64_t comp_val = cms->count_[i][tmp_width];
        if (mini_val > comp_val) mini_val = comp_val;
    }
    return mini_val;
}
void cmsReset(myCMS* cms) {
    int i, j;
    for (i = 0; i < cms->depth_; i++) {
        for (j = 0; j < cms->width_; j++) {
            cms->count_[i][j] = 0;
        }
    }
}