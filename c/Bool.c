//
//  main.c
//  Logic
//
//  Created by Tian XU on 2018/10/12.
//  Copyright © 2018年 Tian XU. All rights reserved.
//

#include <stdio.h>
#include<time.h>
#include <stdbool.h>
int main(int argc, const char * argv[]) {
    bool flag=false;
    bool flag1=false;
    bool flag2=true;
    bool flag3=true;
    // bool flag4=true;
    // bool flag5=true;
    clock_t start,finish;
    //start = clock();
    double total;
    for(int i=0;i<1000;i++){
        if((flag || flag1) && (flag2 || flag3))
        {
            flag=!flag;
            flag1=!flag1;
            flag2=!flag2;
            flag3=!flag3;
        }
    }
    
    // insert code here...
    finish = clock();
    total=(double)(finish-start)/CLOCKS_PER_SEC;
    printf("%f",total);
    return 0;
}
