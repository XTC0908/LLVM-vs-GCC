//
//  main.c
//  Loop
//
//  Created by Tian XU on 2018/10/12.
//  Copyright © 2018年 Tian XU. All rights reserved.
//

#include <stdio.h>
#include <time.h>
int arithmetricSeries(int i){
    if(i==0) return 0;
    else return i+arithmetricSeries(i-1);
}
int main(int argc, const char * argv[]) {
    int j=0;
    clock_t start,finish;
    double total;
    start=clock();
    for(int i=0;i<10000;i++)
    {
        int k = arithmetricSeries(i);
        int l=arithmetricSeries(i);
        if(k>l) j+=1;
        else j+=k;
    }
    finish=clock();
    total=(double)(finish-start)/CLOCKS_PER_SEC;
    
    // insert code here...
    printf("%f",total);
    return 0;
}

