#include <stdio.h>

#define N 16

//显示五子棋棋盘
void wzq_show(char (*GAME)[N])
{
int i=0,j=0;
printf("五子棋棋盘如下：\n");
for(i=0;i<N;i++)
{
for(j=0;j<N;j++)
{
printf("%c ",GAME[i][j]);
}
printf("\n");
}
}

//初始化五子棋棋盘
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

//游戏是否胜利的运算,一旦有人胜利则返回1
int wzq_play(int a,int b,char (*GAME)[N])
{
int i=0,j=0,k=0;
//方向-上的
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
printf("\n恭喜你，白家－方向上获胜!\n");
return 1;
}
}
}

//方向|上的
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
printf("\n恭喜你，白家｜方向上获胜!\n");
return 1;
}
}
}

//方向\上的
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
printf("\n恭喜你，白家＼方向上获胜!\n");
return 1;
}
}
}

//方向/上的
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
printf("\n恭喜你，白家/方向上获胜!\n");
return 1;
}
}
}
return 0;
}

int main()
{
char GAME[N][N]={0};
int count=0;//轮换落子计算
int m=0,n=0;//白家
int x=0,y=0;//黑家
int res=0;
int k=0;
wzq_init(GAME);
wzq_show(GAME);
while(1)
{
//当棋盘格子被下满了时
if(N*N==count)
{
printf("棋盘已经下满了，平局!\n");
return 0;
}

if(0==count%2)
{
printf("请白家落子（例如0 3或1 3）：");
scanf("%d%d",&m,&n);
while(m<0 || m>N-1 || n<0 || n>N-1)
{
printf("输入的坐标超出范围，请重新输入：");
scanf("%d%d",&m,&n);
}
while('+'!=GAME[m][n])
{
printf("此处已经落过子，请重新落子：");
scanf("%d%d",&m,&n);
}
GAME[m][n]='O';

res=wzq_play(m,n,GAME);
if(1==res)
{
wzq_show(GAME);
printf("程序结束\n");
return 0;
}

}else
{
printf("请黑家落子（例如0 3或1 3）：");
scanf("%d%d",&x,&y);
while(m<0 || m>N-1 || n<0 || n>N-1)
{
printf("输入的坐标超出范围，请重新输入：");
scanf("%d%d",&m,&n);
}
while('+'!=GAME[x][y])
{
printf("此处已经落过子，请重新落子：");
scanf("%d%d",&x,&y);
}
GAME[x][y]='@';

res=wzq_play(x,y,GAME);
if(1==res)
{
wzq_show(GAME);
printf("程序结束\n");
return 0;
}
}
count++;
wzq_show(GAME);
}
}
