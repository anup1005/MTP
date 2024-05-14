#include "libOpenCL.h"

__kernel void set_var_kernel(__global int* dflag){
    dflag[0]=0;
    printf("setting the value of the device variable to zero\n ");
}