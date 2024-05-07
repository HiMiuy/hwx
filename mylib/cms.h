#ifndef _CMS_H_
#define _CMS_H_

typedef struct myCMS{
    int width_;
    int depth_;
    int **count_;
    unsigned int *a_, *b_;
} myCMS;

extern myCMS* cmsInit(int, int);
extern void cmsUpdate(myCMS*, long, int);
extern void cmsEst(myCMS*, long);
extern void cmsReset(myCMS*);

#endif