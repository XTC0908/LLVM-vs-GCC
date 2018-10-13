#include <stdio.h>
int i,j,k,m,n;
char a[];
char b[];
char c[][];
char d[][];
double e[2];
double hmax,hmin;

void grayscale(unsigned char* base)
{
	int x, y, i;
	//double a, b, c;
	unsigned char* shared;
    //char a[i];

	shared = (unsigned char*) SHARED_ONCHIP_BASE;

	int size_x = *base++;
	int size_y = *base++;
	int max_col= *base++;
	*shared++  = size_x;
	*shared++  = size_y;
	*shared++  = max_col;
	printf("The image is: %d x %d!! \n", size_x, size_y);
	for(y = 0; y < size_y; y++)
	for(x = 0; x < size_x; x++)
    for(i=0; i <= 3*size_y * size_x;i++)
	{
		//a = (*base++) * 0.3125 + (*base++)
		a[i] = *base++; 	// R
		//*shared++ = *base++;	// G
		//*shared++ = *base++;	// B
    }
	if (size_x *3 mod 3==0)
		{
		    
                for (i,k=0;i < 3 * size_x * size_y && k < size_x * size_y;i=i+3,k++)
                {

		         b[k]=a[i] * 0.3125 + a[i+1] * 0.5625 + a[i+2] * 0.125;
            }

            }
          //  for (m=0;m<size_y;m++)
            //{
//
  //          for (n=0;n<size_x;n++)
    //        {


		//    c[m][n]=b[m*x/3 + n];
		//*shared++ = a[i] * 0.3125 + a[i+1] * 0.5625 + a[i+2] * 0.125;
		else
        printf("error X length is not a multiple of 3");
	}

	for(m=0;m<size_y;m++)
    {
        for (n=0;n<size_x;n++)
        {
          c[m][n]=b[m * size_x + n];
               }
    }
 void resized()
   {
       

       for (m,p=0;m<size_y && p<size_y/2;m=m+2,p++)
       {
           for (n,q=0;n<size_x && q<size_x/2;n=n+2,q++)
           {
               d[p][q] = (c[m][n]+c[m][n+1]+c[m+1][n]+c[m+1][n+1])/4;
           }
       }
       
   }
void bright()
{
double hmax = 0.0;
double hmin = 255.0;
for (k=0;k<size_x * size_y;k++)
{
    if b[k]>hmax
    {
        hmax=b[k];
    }
    if b[k]<hmin
    {
        hmin=b[k];
    }
}
double e[2] = {hmax,hmin};
}

#define N1 512  
#define M1 512  
#define N2 3  
#define M2 3  
/*x相当于矩阵A,y相当于J矩阵B,z相当于矩阵full时的输出，z2相当于矩阵与A维数相同时的输出，这里的参数类型根据实际情况作出修改。如果矩阵很大，运算时间比较久*/  
void conv2(unsigned char x[N1][M1],int y[N2][M2],float z[N1+N2-1][M1+M2-1],float z2[N1][M1])  
{  
    int i,j;  
    int n,m;  
    for(i=0; i<N1+N2-1; i++)  
        for(j=0; j<M1+M2-1; j++)  
        {  
            float temp = 0;  
            for(m=0; m<N1; m++)  
                for(n=0; n<M1; n++)  
                    if((i-m)>=0&&(i-m)<N2&&(j-n)>=0&&(j-n)<M2)  
                        temp+=x[m][n]*y[i-m][j-n];  
            z[i][j]=temp;  
        }  
    for(i=0; i<N1; i++)  
        for(j=0; j<M1; j++)  
        {  
            z2[i][j]=z[i+(N2-1)/2][j+(M2-1)/2];  
        }  
}  

/*
void sobel()
{
int a[3][3]={}
}

*/
void toAscii()
{
    double f[i][j]=z2[i][j]*15/255;
    int g[i][j]=(int)f[i][j];
   char h[16] = {' ','.',':','-','=','+','/','t','z','U','w','*','0','#','%','@'}；
   g[i][j] = h[g[i][j]];

}
