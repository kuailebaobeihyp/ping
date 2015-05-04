// diaodu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>
typedef int Status;
#define ERROR 0
#define OK 1
typedef struct PCB
{
	char name[10];//进程名字
	int PRIO;     //进程优先数
	int ROUNT;    //轮转时间片	
	int NEEDTIME; //需要的CPU时间片数
	int CPUTIME;  //进程累计占用CPU时间片数
	char *STATE;  //进程状态
	int COUNT;    //计数器
}ElemPCB;

typedef struct QNode
{
	ElemPCB pcb;
	struct QNode *next;
}QNode,*QueuePtr;

typedef struct         //就绪队列
{
	QueuePtr RUN;      //当前运行进程指针
	QueuePtr READY;    //头指针
	QueuePtr TAIL;     //尾指针
}READYQueue;

typedef struct         //完成队列
{
	QueuePtr FINISH;    //头指针
	QueuePtr TAIL;     //尾指针
}FINISHQueue;

Status Create(READYQueue &ready);                                  //创建后就进入就绪队列中
Status Print(READYQueue ready,FINISHQueue finish);                 //打印就绪队列中的进程状态,优先数
Status Printr(READYQueue ready,FINISHQueue finish);                //打印就绪队列中的进程状态，时间片轮转
Status First(READYQueue &ready);
Status Insert1(READYQueue &ready);
Status Insert2(READYQueue &ready);
Status Prisch(READYQueue &ready,FINISHQueue &finish);              //优先数调度
Status Roundsch(READYQueue &ready,FINISHQueue &finish);            //时间片轮转法

int main(int argc, char* argv[])
{
	char ch;
	READYQueue ready;
	FINISHQueue finish;
	ready.READY = ready.TAIL = (QueuePtr)malloc(sizeof(QNode));//存储分配
	ready.RUN = (QueuePtr)malloc(sizeof(QNode));
	ready.RUN->next = NULL;
	finish.FINISH = finish.TAIL = (QueuePtr)malloc(sizeof(QNode));
	Create(ready);  //创建后就进入就绪队列中
	printf("\n就绪队列中初始值：\n");
	Print(ready,finish);
	First(ready);
	printf("请输入要选择调度的算法（p-优先数调度，r-时间片轮转法）：\n");
	while(1)
	{
		do
		{
			ch = getchar();
			scanf("%c",&ch);
		}while(ch != 'p' && ch !='r');
		switch(ch)
		{
		case 'p':   //优先数调度
			Prisch(ready,finish);
			break;
		case 'r':   //时间片轮转法
			Roundsch(ready,finish);
			break;
		}
	}
	return 0;
}
Status Print(READYQueue ready,FINISHQueue finish)   //打印就绪队列中的进程状态,优先数
{
	QueuePtr p,q;
	p = ready.READY;
	q = finish.FINISH;
	//运行中的进程
	if(ready.RUN->next != NULL)
	{
		printf("%s",ready.RUN->next->pcb.name);
		printf(":%s\t",ready.RUN->next->pcb.STATE);
		printf("优先数：%d\n",ready.RUN->next->pcb.PRIO);
	}
	//就绪队列的进程
	while(p != ready.TAIL)
	{
		printf("%s",p->next->pcb.name);
		printf(":%s\t",p->next->pcb.STATE);
		printf("优先数：%d\n",p->next->pcb.PRIO);
		p = p->next;
	}
	//完成队列中的进程
	while(q != finish.TAIL)
	{
		printf("%s",q->next->pcb.name);
		printf(":%s\t",q->next->pcb.STATE);
		printf("优先数：%d\n",q->next->pcb.PRIO);
		q = q->next;
	}
	return OK;
}

Status Printr(READYQueue ready,FINISHQueue finish)   //打印就绪队列中的进程状态，时间片轮转
{
	QueuePtr p,q;
	p = ready.READY;
	q = finish.FINISH;
	//运行中的进程
	if(ready.RUN->next != NULL)
	{
		printf("%s",ready.RUN->next->pcb.name);
		printf(":%s\t",ready.RUN->next->pcb.STATE);
		printf("剩余时间：%d\n",ready.RUN->next->pcb.NEEDTIME);
	}
	//就绪队列的进程
	while(p != ready.TAIL)
	{
		printf("%s",p->next->pcb.name);
		printf(":%s\t",p->next->pcb.STATE);
		printf("剩余时间：%d\n",p->next->pcb.NEEDTIME);
		p = p->next;
	}
	//完成队列中的进程
	while(q != finish.TAIL)
	{
		printf("%s",q->next->pcb.name);
		printf(":%s\t",q->next->pcb.STATE);
		printf("剩余时间：%d\n",q->next->pcb.NEEDTIME);
		q = q->next;
	}
	return OK;
}
Status Create(READYQueue &ready)
{
	QueuePtr p;
	int i = 0;
	int n;
	printf("请输入进程的个数：");
	scanf("%d",&n);
	while(i < n)
	{
		printf("--------------------------------------------------------------\n");
		p = (QueuePtr)malloc(sizeof(QNode));
		printf("输入第%d进程名：",i+1);
		scanf("%s",& p -> pcb.name);
		printf("  输入进程需要的时间：");
		scanf("%d",&p -> pcb.NEEDTIME);
		printf("  输入进程的优先数：");
		scanf("%d",& p -> pcb.PRIO);
		p -> pcb.STATE = "W";
		p -> pcb.ROUNT = 2;
		p -> pcb.COUNT = 0;
		i++;
		p -> next = NULL;
		ready.TAIL -> next = p;
		ready.TAIL = p;
	}
	printf("--------------------------------------------------------------\n");
	return OK;
}
Status First(READYQueue &ready)
{
	if(ready.READY == ready.TAIL)
		return ERROR;
	ready.RUN -> next = ready.READY -> next;
	ready.RUN -> next ->pcb.STATE = "RUN";  //修改进程状态
	if(ready.TAIL == ready.READY -> next)
		ready.READY = ready.TAIL;
	else
		ready.READY -> next = ready.READY -> next -> next;  //头指针后移
	printf("\n%s被从就绪队列调度运行\n",ready.RUN -> next ->pcb.name);
	return OK;
}

Status Insert1(READYQueue &ready)
{
	int i = 0,j = 0;
	QueuePtr p = ready.READY , q;
	ElemPCB temp;
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	s -> pcb = ready.RUN -> next -> pcb;
	s -> next = NULL;         //将未完成的进程插入就绪队列
	ready.TAIL -> next = s;
	ready.TAIL = s;
	//按优先数从小到大排序
	for(p;p != ready.TAIL;p = p -> next)
	{
		for(q = p -> next;q != ready.TAIL;q = q -> next)
		{
			if(p -> next -> pcb.PRIO < p -> next -> pcb.PRIO)
			{
				temp = p -> next ->pcb;
				p ->next -> pcb = q -> next -> pcb;
				q -> next -> pcb = temp;
			}
		}
	}
	return OK;
}

Status Insert2(READYQueue &ready)
{
	QueuePtr p = ready.RUN -> next;
	if(p -> pcb.NEEDTIME > 0)
	{
		ready.TAIL -> next = p;    //插入到就绪队列
		ready.TAIL = p;
		ready.RUN -> next = NULL;
	}
	return OK;
}

Status Prisch(READYQueue &ready,FINISHQueue &finish)
{
	int i = 0;
	while(ready.RUN -> next != NULL)
	{
		ready.RUN -> next ->pcb.CPUTIME++;
		ready.RUN -> next ->pcb.NEEDTIME--;
		ready.RUN -> next ->pcb.PRIO -= 3 ;
		if(ready.RUN -> next -> pcb.NEEDTIME == 0)
		{
			finish.TAIL -> next = ready.RUN -> next;    //插入到完成队列
			finish.TAIL -> next = NULL;           //尾指针后移
			ready.RUN -> next -> pcb.STATE = "FINISH";
			ready.RUN -> next = NULL;
			if(ready.READY != ready.TAIL)
			{
				First(ready);
			}
		}
		else if(ready.READY != ready.TAIL && (ready.RUN ->next -> pcb.PRIO) < (ready.READY -> next -> pcb.PRIO))
		{
			ready.RUN -> next -> pcb.STATE = "W";
			printf("中%s被调入到就绪队列里\n",ready.RUN -> next -> pcb.name);
			Insert1(ready);
			First(ready);
		}
		i++;
		printf("\n进程执行第%d个时间片的结果：\n",i);
		Print(ready,finish);
	}
	return OK;
}
Status Roundsch(READYQueue &ready,FINISHQueue &finish)
{
	int i = 0;
	while(ready.RUN -> next != NULL)
	{
		ready.RUN -> next -> pcb.CPUTIME++;
		ready.RUN -> next -> pcb.NEEDTIME--;
		ready.RUN -> next -> pcb.COUNT++;
		if(ready.RUN -> next -> pcb.NEEDTIME == 0)
		{
			finish.TAIL -> next = ready.RUN -> next;    //插入到完成队列
			finish.TAIL = ready.RUN -> next;    //尾指针后移
			ready.RUN ->next -> pcb.STATE = "FINISH";
			ready.RUN -> next = NULL;
			if(ready.READY != ready.TAIL)
			{
				First(ready);
			}
		}
		else if(ready.RUN -> next -> pcb.COUNT == ready.RUN -> next -> pcb.ROUNT)
		{
			ready.RUN -> next -> pcb.COUNT = 0;
			printf("%s被调到就绪队列里\n",ready.RUN -> next -> pcb.name);
			Insert2(ready);
			First(ready);
		}
		i++;
		printf("\n进程执行第%d个时间片的结果：\n",i);
		Printr(ready,finish);
	}
	return OK;
}



