#include <stdio.h>

#define N 16

//��ʾ����������
void wzq_show(char (*GAME)[N])
{
int i=0,j=0;
printf("�������������£�\n");
for(i=0;i<N;i++)
{
for(j=0;j<N;j++)
{
printf("%c ",GAME[i][j]);
}
printf("\n");
}
}

//��ʼ������������
void wzq_init(char (*GAME)[N])
{
int i=0,j=0;
for(i=0;i<N;i++)
{
for(j=0;j<N;j++)
{
GAME[i][j]='+';
}
}
//memset(GAME,'+',N*N);
}

//��Ϸ�Ƿ�ʤ��������,һ������ʤ���򷵻�1
int wzq_play(int a,int b,char (*GAME)[N])
{
int i=0,j=0,k=0;
//����-�ϵ�
for(j=b-4;j<b+4;j++)
{
if(j<0 || j>N-1)
{
continue;
}
for(i=0;i<5;i++)
{
if('@'==GAME[a][j+i] ||'+'==GAME[a][j+i])
{
break;
}
if('O'==GAME[a][j+i] &&4==i)
{
printf("\n��ϲ�㣬�׼ң������ϻ�ʤ!\n");
return 1;
}
}
}

//����|�ϵ�
for(i=a-4;i<a+4;i++)
{
if(i<0 || i>N-1)
{
continue;
}
for(j=0;j<5;j++)
{
if('@'==GAME[i+j][b] ||'+'==GAME[i+j][b])
{
break;
}
if('O'==GAME[i+j][b] &&4==j)
{
printf("\n��ϲ�㣬�׼ң������ϻ�ʤ!\n");
return 1;
}
}
}

//����\�ϵ�
for(i=a-4,j=b-4;i<a+4 && j<b+4;i++,j++)
{
if(i<0 || i>N-1 || j<0 || j>N-1)
{
continue;
}
for(k=0;k<5;k++)
{
if('@'==GAME[i+k][j+k] ||'+'==GAME[i+k][j+k])
{
break;
}
if('O'==GAME[i+k][j+k] &&4==k)
{
printf("\n��ϲ�㣬�׼ңܷ����ϻ�ʤ!\n");
return 1;
}
}
}

//����/�ϵ�
for(i=a+4,j=b-4;i>a-4 && j<b+4;i--,j++)
{
if(i<0 || i>N-1 || j<0 || j>N-1)
{
continue;
}
for(k=0;k<5;k++)
{
if('@'==GAME[i-k][j+k] ||'+'==GAME[i-k][j+k])
{
break;
}
if('O'==GAME[i-k][j+k] &&4==k)
{
printf("\n��ϲ�㣬�׼�/�����ϻ�ʤ!\n");
return 1;
}
}
}
return 0;
}

int main()
{
char GAME[N][N]={0};
int count=0;//�ֻ����Ӽ���
int m=0,n=0;//�׼�
int x=0,y=0;//�ڼ�
int res=0;
int k=0;
wzq_init(GAME);
wzq_show(GAME);
while(1)
{
//�����̸��ӱ�������ʱ
if(N*N==count)
{
printf("�����Ѿ������ˣ�ƽ��!\n");
return 0;
}

if(0==count%2)
{
printf("��׼����ӣ�����0 3��1 3����");
scanf("%d%d",&m,&n);
while(m<0 || m>N-1 || n<0 || n>N-1)
{
printf("��������곬����Χ�����������룺");
scanf("%d%d",&m,&n);
}
while('+'!=GAME[m][n])
{
printf("�˴��Ѿ�����ӣ����������ӣ�");
scanf("%d%d",&m,&n);
}
GAME[m][n]='O';

res=wzq_play(m,n,GAME);
if(1==res)
{
wzq_show(GAME);
printf("�������\n");
return 0;
}

}else
{
printf("��ڼ����ӣ�����0 3��1 3����");
scanf("%d%d",&x,&y);
while(m<0 || m>N-1 || n<0 || n>N-1)
{
printf("��������곬����Χ�����������룺");
scanf("%d%d",&m,&n);
}
while('+'!=GAME[x][y])
{
printf("�˴��Ѿ�����ӣ����������ӣ�");
scanf("%d%d",&x,&y);
}
GAME[x][y]='@';

res=wzq_play(x,y,GAME);
if(1==res)
{
wzq_show(GAME);
printf("�������\n");
return 0;
}
}
count++;
wzq_show(GAME);
}
}
