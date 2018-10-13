#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void arymul(int a[4][5], int b[5][3])
{
    int i, j, k;
    int c[4][3];
    int temp;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 3; j++){
            temp = 0;
            for(k = 0; k < 5; k++){
                temp += a[i][k] * b[k][j];
            }
            c[i][j] = temp;
            // printf("%d/t", c[i][j]);
        }
        // printf("%d/n");
    }
}
int main(){
    clock_t start,finish;
    //start = clock();
    double total;
    
    int a[4][5] = {{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20}};
    int b[5][3]= {{1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15}};
    //int c= {(1,2,3),(4,5,6),(7,8,9),(10,11,12)};
    //int c[4][3];
    start = clock();
    for(int f=0;f<100000;f++)
    {
        arymul(a,b);
        
    }
    finish = clock();
    total=(double)(finish-start)/CLOCKS_PER_SEC;
    printf("%f",total);
    return 0;
}
