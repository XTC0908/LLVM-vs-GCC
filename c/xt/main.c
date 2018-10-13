
#include<stdio.h>
#include<stdlib.h>
void main()
{
    char c;
    scanf("%c",&c);
    if ((c>='A')&&(c<='Z')) c+=37;

    else if((c>='a')&&(c<='z')) c+=34;

    else
    {
        printf("gun\n");
        exit(0);
    }
    printf("%c\n",c);
}
