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
	char name[10];//��������
	int PRIO;     //����������
	int ROUNT;    //��תʱ��Ƭ	
	int NEEDTIME; //��Ҫ��CPUʱ��Ƭ��
	int CPUTIME;  //�����ۼ�ռ��CPUʱ��Ƭ��
	char *STATE;  //����״̬
	int COUNT;    //������
}ElemPCB;

typedef struct QNode
{
	ElemPCB pcb;
	struct QNode *next;
}QNode,*QueuePtr;

typedef struct         //��������
{
	QueuePtr RUN;      //��ǰ���н���ָ��
	QueuePtr READY;    //ͷָ��
	QueuePtr TAIL;     //βָ��
}READYQueue;

typedef struct         //��ɶ���
{
	QueuePtr FINISH;    //ͷָ��
	QueuePtr TAIL;     //βָ��
}FINISHQueue;

Status Create(READYQueue &ready);                                  //������ͽ������������
Status Print(READYQueue ready,FINISHQueue finish);                 //��ӡ���������еĽ���״̬,������
Status Printr(READYQueue ready,FINISHQueue finish);                //��ӡ���������еĽ���״̬��ʱ��Ƭ��ת
Status First(READYQueue &ready);
Status Insert1(READYQueue &ready);
Status Insert2(READYQueue &ready);
Status Prisch(READYQueue &ready,FINISHQueue &finish);              //����������
Status Roundsch(READYQueue &ready,FINISHQueue &finish);            //ʱ��Ƭ��ת��

int main(int argc, char* argv[])
{
	char ch;
	READYQueue ready;
	FINISHQueue finish;
	ready.READY = ready.TAIL = (QueuePtr)malloc(sizeof(QNode));//�洢����
	ready.RUN = (QueuePtr)malloc(sizeof(QNode));
	ready.RUN->next = NULL;
	finish.FINISH = finish.TAIL = (QueuePtr)malloc(sizeof(QNode));
	Create(ready);  //������ͽ������������
	printf("\n���������г�ʼֵ��\n");
	Print(ready,finish);
	First(ready);
	printf("������Ҫѡ����ȵ��㷨��p-���������ȣ�r-ʱ��Ƭ��ת������\n");
	while(1)
	{
		do
		{
			ch = getchar();
			scanf("%c",&ch);
		}while(ch != 'p' && ch !='r');
		switch(ch)
		{
		case 'p':   //����������
			Prisch(ready,finish);
			break;
		case 'r':   //ʱ��Ƭ��ת��
			Roundsch(ready,finish);
			break;
		}
	}
	return 0;
}
Status Print(READYQueue ready,FINISHQueue finish)   //��ӡ���������еĽ���״̬,������
{
	QueuePtr p,q;
	p = ready.READY;
	q = finish.FINISH;
	//�����еĽ���
	if(ready.RUN->next != NULL)
	{
		printf("%s",ready.RUN->next->pcb.name);
		printf(":%s\t",ready.RUN->next->pcb.STATE);
		printf("��������%d\n",ready.RUN->next->pcb.PRIO);
	}
	//�������еĽ���
	while(p != ready.TAIL)
	{
		printf("%s",p->next->pcb.name);
		printf(":%s\t",p->next->pcb.STATE);
		printf("��������%d\n",p->next->pcb.PRIO);
		p = p->next;
	}
	//��ɶ����еĽ���
	while(q != finish.TAIL)
	{
		printf("%s",q->next->pcb.name);
		printf(":%s\t",q->next->pcb.STATE);
		printf("��������%d\n",q->next->pcb.PRIO);
		q = q->next;
	}
	return OK;
}

Status Printr(READYQueue ready,FINISHQueue finish)   //��ӡ���������еĽ���״̬��ʱ��Ƭ��ת
{
	QueuePtr p,q;
	p = ready.READY;
	q = finish.FINISH;
	//�����еĽ���
	if(ready.RUN->next != NULL)
	{
		printf("%s",ready.RUN->next->pcb.name);
		printf(":%s\t",ready.RUN->next->pcb.STATE);
		printf("ʣ��ʱ�䣺%d\n",ready.RUN->next->pcb.NEEDTIME);
	}
	//�������еĽ���
	while(p != ready.TAIL)
	{
		printf("%s",p->next->pcb.name);
		printf(":%s\t",p->next->pcb.STATE);
		printf("ʣ��ʱ�䣺%d\n",p->next->pcb.NEEDTIME);
		p = p->next;
	}
	//��ɶ����еĽ���
	while(q != finish.TAIL)
	{
		printf("%s",q->next->pcb.name);
		printf(":%s\t",q->next->pcb.STATE);
		printf("ʣ��ʱ�䣺%d\n",q->next->pcb.NEEDTIME);
		q = q->next;
	}
	return OK;
}
Status Create(READYQueue &ready)
{
	QueuePtr p;
	int i = 0;
	int n;
	printf("��������̵ĸ�����");
	scanf("%d",&n);
	while(i < n)
	{
		printf("--------------------------------------------------------------\n");
		p = (QueuePtr)malloc(sizeof(QNode));
		printf("�����%d��������",i+1);
		scanf("%s",& p -> pcb.name);
		printf("  ���������Ҫ��ʱ�䣺");
		scanf("%d",&p -> pcb.NEEDTIME);
		printf("  ������̵���������");
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
	ready.RUN -> next ->pcb.STATE = "RUN";  //�޸Ľ���״̬
	if(ready.TAIL == ready.READY -> next)
		ready.READY = ready.TAIL;
	else
		ready.READY -> next = ready.READY -> next -> next;  //ͷָ�����
	printf("\n%s���Ӿ������е�������\n",ready.RUN -> next ->pcb.name);
	return OK;
}

Status Insert1(READYQueue &ready)
{
	int i = 0,j = 0;
	QueuePtr p = ready.READY , q;
	ElemPCB temp;
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	s -> pcb = ready.RUN -> next -> pcb;
	s -> next = NULL;         //��δ��ɵĽ��̲����������
	ready.TAIL -> next = s;
	ready.TAIL = s;
	//����������С��������
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
		ready.TAIL -> next = p;    //���뵽��������
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
			finish.TAIL -> next = ready.RUN -> next;    //���뵽��ɶ���
			finish.TAIL -> next = NULL;           //βָ�����
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
			printf("��%s�����뵽����������\n",ready.RUN -> next -> pcb.name);
			Insert1(ready);
			First(ready);
		}
		i++;
		printf("\n����ִ�е�%d��ʱ��Ƭ�Ľ����\n",i);
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
			finish.TAIL -> next = ready.RUN -> next;    //���뵽��ɶ���
			finish.TAIL = ready.RUN -> next;    //βָ�����
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
			printf("%s����������������\n",ready.RUN -> next -> pcb.name);
			Insert2(ready);
			First(ready);
		}
		i++;
		printf("\n����ִ�е�%d��ʱ��Ƭ�Ľ����\n",i);
		Printr(ready,finish);
	}
	return OK;
}



