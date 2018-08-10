// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI �ڵ� �ۼ� ���]

1. char info[]�� �迭 �ȿ�					"TeamName:�ڽ��� ����,Department:�ڽ��� �Ҽ�"					������ �ۼ��մϴ�.
( ���� ) Teamname:�� Department:�� �� ���� �մϴ�.
"�ڽ��� ����", "�ڽ��� sds�Ҽ�"�� �����ؾ� �մϴ�.

2. �Ʒ��� myturn() �Լ� �ȿ� �ڽŸ��� AI �ڵ带 �ۼ��մϴ�.

3. AI ������ �׽�Ʈ �Ͻ� ���� "���� �˰����ȸ ��"�� ����մϴ�.

4. ���� �˰��� ��ȸ ���� �����ϱ⿡�� �ٵϵ��� ���� ��, �ڽ��� "����" �� �� �˰����� �߰��Ͽ� �׽�Ʈ �մϴ�.



[���� �� �Լ�]
myturn(int cnt) : �ڽ��� AI �ڵ带 �ۼ��ϴ� ���� �Լ� �Դϴ�.
int cnt (myturn()�Լ��� �Ķ����) : ���� �� �� �־��ϴ��� ���ϴ� ����, cnt�� 1�̸� ���� ���� ��  �� ����  �δ� ��Ȳ(�� ��), cnt�� 2�̸� �� ���� ���� �δ� ��Ȳ(�� ��)
int  x[0], y[0] : �ڽ��� �� ù �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
int  x[1], y[1] : �ڽ��� �� �� �� ° ���� x��ǥ , y��ǥ�� ����Ǿ�� �մϴ�.
void domymove(int x[], int y[], cnt) : �� ������ ��ǥ�� �����ؼ� ���


//int board[BOARD_SIZE][BOARD_SIZE]; �ٵ��� �����Ȳ ��� �־� �ٷλ�� ������. ��, ���������ͷ� ���� �������
// ������ ���� ��ġ�� �ٵϵ��� ������ �ǰ��� ó��.

boolean isFree(int x, int y) : ���� [x,y]��ǥ�� �ٵϵ��� �ִ��� Ȯ���ϴ� �Լ� (������ true, ������ false)
int showBoard(int x, int y) : [x, y] ��ǥ�� ���� ���� �����ϴ��� �����ִ� �Լ� (1 = �ڽ��� ��, 2 = ����� ��, 3 = ��ŷ)


<-------AI�� �ۼ��Ͻ� ��, ���� �̸��� �Լ� �� ���� ����� �������� �ʽ��ϴ�----->
*/
#pragma once
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"
using namespace std;

// "�����ڵ�[C]"  -> �ڽ��� ���� (����)
// "AI�μ�[C]"  -> �ڽ��� �Ҽ� (����)
// ����� ���������� �ݵ�� �������� ����!
char info[] = { "TeamName:����, Department:���Ǵ��б�" };

/*
	Ŀ���� ������
*/

int stage;							//������ ���° ������ �Ǵ� (board�迭�� �ִ� ���� ����(���� 1,2 �� ��) search �Ͽ� �Ǵ�)
int BW;									//���� ��������� ���������
pair <int, int> Lastmymove1;			//�ֱ� �Ʊ� ù��° ��
pair <int, int> Lastmymove2;			//�ֱ� �Ʊ� �ι�° ��
pair <int, int> Lastopmove1;			//�ֱ� ���� ù��° ��
pair <int, int> Lastopmove2;			//�ֱ� ���� �ι�° ��
vector < pair<int, int> > player;		//�ϸ��� ������ ���� ���� �Ʊ��� ������ ���������� �ε���:0�� ¦�� / ���̸� Ȧ��
vector < pair<int, int> > simulplayer;
int human;								//TSS���� �Ʊ� ���� �Ǻ�


double CountStone(deque <int> &dq)		//1���� ť�� ���� TS �Ǵ� �Լ�
{
	double TS = 0.0;
	int humancnt = 0;
	int semihuman = 0;
	for (int i = 0; i < dq.size(); i++)
	{
		if (dq[i] == human && i + 1 < dq.size() && dq[i + 1] == human) //ù ��ǥ�� 1�̳� 2�̰�(human) ���� ���� ���ٸ�?
		{
			humancnt += 10;
		}
		//ó���� ��ĭ�ΰ�� ���� ����
		else if (dq[i] == human && i + 1<dq.size() && dq[i + 1] == 0
			&& humancnt >= 10 && i + 2 <dq.size() && dq[i + 2] == human)//���ӵ� ��Ȳ�� ����µ�(�ϴ� dq[i]==human, humancnt>=20�̰�, �������� ��ĭ�̰� �״������� human�̶��? 
		{
			humancnt += 5;

		}
		else
		{

		}

	}
	if (humancnt >= 30) //human �� 4������
	{
		return TS = 10000.0;
	}
	else if (humancnt >= 20)	//human �� 3�� ���� + ���� ��ĭ + human
	{
		return TS = 500.0;
	}
	else if (humancnt >= 10)	//human 2�� ���� + ���� ��ĭ + human
	{
		return TS = 10.0;
	}
	else
	{
		return TS;
	}


}


double TSS(int x, int y)		//�� ��ǥ(�ֱٿ� ���� �Ʊ��� �� ���� �� �ϳ�)�� ���� TS�� Ž���Ͽ� ES���
{
	//extern double CountStone(deque <int> dq);
	double TS = 0.0;
	deque <int> dq;
	for (int i = player.size() - 1; i > 0; i--)
	{
		if (x >= 0 && y >= 0 && x < width && y < height)
		{

			int tempfx, tempbx, tempfy, tempby;

			if (showBoard(x, y) == 1)	//��ǥ�� �Ʊ��̸�
			{
				human = 1;
			}
			else if (showBoard(x, y) == 2) //��ǥ�� �����̸�
			{
				human = 2;
			}
			else
			{
				return 0.0; //�߸��� ��ǥ ���°� 
			}
			//////���� DFS
			dq.push_back(human);	//x,y ��ǥ�� �ִ� 0,1,2 �߿� �� ���� (���ذ�)
			for (int i = 1; i <= 6; i++)		//board�� ���� ���� DFS, �˻��Ǵ� �ֵ��� ��ť�� Ǫ��
			{
				tempfx = x - i;
				tempbx = x + i;

				if (tempfx >= 0 && y >= 0 && tempfx < width && y < height)
					dq.push_front(showBoard(tempfx, y));

				if (tempbx >= 0 && y >= 0 && tempbx < width && y < height)
					dq.push_back(showBoard(tempbx, y));
			}
			TS += CountStone(dq);
			dq.clear();


			///////���� DFS
			dq.push_back(showBoard(x, y));	//x,y ��ǥ�� �ִ� 0,1,2 �߿� �� ���� (���ذ�)
			for (int i = 1; i <= 6; i++)		//board�� ���� ���� DFS, �˻��Ǵ� �ֵ��� ��ť�� Ǫ��
			{
				tempfy = y - i;
				tempby = y + i;
				if (x >= 0 && tempfy >= 0 && x < width && tempby < height)
					dq.push_front(showBoard(x, tempfy));

				if (x >= 0 && tempfy >= 0 && x < width && tempby < height)
					dq.push_back(showBoard(x, tempby));
			}
			TS += CountStone(dq);
			dq.clear();

			///////��밢 DFS
			dq.push_back(showBoard(x, y));	//x,y ��ǥ�� �ִ� 0,1,2 �߿� �� ���� (���ذ�)
			for (int i = 1; i <= 6; i++)		//board�� ���� ���� DFS, �˻��Ǵ� �ֵ��� ��ť�� Ǫ��
			{
				tempfx = x - i;
				tempbx = x + i;
				tempfy = y - i;
				tempby = y + i;
				if (tempfx >= 0 && tempfy >= 0 && tempbx < width && tempby < height)
					dq.push_front(showBoard(tempfx, tempfy));

				if (tempfx >= 0 && tempfy >= 0 && tempbx < width && tempby < height)
					dq.push_back(showBoard(tempfx, tempby));
			}
			TS += CountStone(dq);
			dq.clear();

			///////�´밢 DFS
			dq.push_back(showBoard(x, y));	//x,y ��ǥ�� �ִ� 0,1,2 �߿� �� ���� (���ذ�)
			for (int i = 1; i <= 6; i++)		//board�� ���� ���� DFS, �˻��Ǵ� �ֵ��� ��ť�� Ǫ��
			{
				tempfx = x - i;
				tempbx = x + i;
				tempfy = y + i;
				tempby = y - i;
				if (tempfx >= 0 && tempfy >= 0 && tempbx < width && tempby < height)
					dq.push_front(showBoard(tempfx, tempfy));

				if (tempfx >= 0 && tempfy >= 0 && tempbx < width && tempby < height)
					dq.push_back(showBoard(tempfx, tempby));
			}
			TS += CountStone(dq);
			dq.clear();
		}
	}
	return TS;
}



void myturn(int cnt) {

	int x[2], y[2];
	MCT *Tree = new MCT();				//Ʈ�� ����
										//�ʱ� 8�� ��� �ʱ�ȭ
	Tree->MCTExpand(Tree->Getroot());

	//MCNode* childNode1 = new MCNode(Tree->root);
	//MCNode* childNode2 = new MCNode(Tree->root);
	//MCNode* childNode3 = new MCNode(Tree->root);
	//MCNode* childNode4 = new MCNode(Tree->root);
	//MCNode* childNode5 = new MCNode(Tree->root);
	//MCNode* childNode6 = new MCNode(Tree->root);
	//MCNode* childNode7 = new MCNode(Tree->root);
	//MCNode* childNode8 = new MCNode(Tree->root);

	//Tree->root->AddChild(childNode1);
	//Tree->root->AddChild(childNode2);
	//Tree->root->AddChild(childNode3);
	//Tree->root->AddChild(childNode4);
	//Tree->root->AddChild(childNode5);
	//Tree->root->AddChild(childNode6);
	//Tree->root->AddChild(childNode7);
	//Tree->root->AddChild(childNode8);

	//Tree->MCTSelect(Tree->Getroot());

	//�� ��2��°�� or ��ù��° �Ͽ����� ������ ���� �������� ���� ���ƾ���

	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.

	srand((unsigned)time(NULL));  //�ð����� ���� �߻�
	
	int tmpx=0, tmpy=0;

	for (int i = 0; i < cnt; i++)
	{
		while (!isFree(x[i], y[i])) // ���� �� ������ ������ ����
		{
			x[i] = rand() % width;	//���� ������������ �ּ��� ��
			y[i] = rand() % height;

			tmpx = rand() % 2;
			if (tmpx == 0)
				tmpx -= 1;
			
			tmpy = rand() % 2;
			if (tmpy == 0)
				tmpy -= 1;
			
			if (isFree((player[player.size() - 1].first + tmpx), (player[player.size() - 1].second + tmpx)))
			{
				x[i] = player[player.size() - 1].first + tmpx;
				y[i] = player[player.size() - 1].second + tmpy;
			}

			if (terminateAI == 1) return;	//true�̸� turn ����



		}



		if (x[1] == x[0] && y[1] == y[0]) //ù���� �μ��� ������ ������ �ٽ� �õ�
			i--;
	}


















	// �� �κп��� �ڽ��� ���� ���� ����Ͻʽÿ�.
	// �ʼ� �Լ� : domymove(x�迭,y�迭,�迭ũ��)
	// ���⼭ �迭ũ��(cnt)�� myturn()�� �Ķ���� cnt�� �״�� �־���մϴ�.

	while (1)
	{
		l_time = GetTickCount();

		if (l_time - s_time >= (unsigned int)((limitTime * 1000) - 2500)) //1000�� �ð��⺻����,250 (0.25��) 
		{
			char buf[200] = { " " };
			sprintf_s(buf, "l_time: %u  s_time: %u  limitTime: %d ������� �����ð�: %u \n", l_time, s_time, limitTime, l_time - s_time);
			writeLog(buf);

			domymove(x, y, cnt);
			break;
		}

		//domymove(x, y, cnt);
		//break;
	}
}



