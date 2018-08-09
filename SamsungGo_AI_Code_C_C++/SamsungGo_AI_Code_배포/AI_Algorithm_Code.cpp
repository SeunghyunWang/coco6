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
pair <int, int> Lastmymove1;			//�ֱ� �Ʊ� ù��° ��
pair <int, int> Lastmymove2;			//�ֱ� �Ʊ� �ι�° ��
pair <int, int> Lastopmove1;			//�ֱ� ���� ù��° ��
pair <int, int> Lastopmove2;			//�ֱ� ���� �ι�° ��
vector < pair<int, int> > player;		//�ϸ��� ������ ���� ���� �Ʊ��� ������ ���������� �ε���:0�� ¦�� / ���̸� Ȧ��




void myturn(int cnt) {

	int x[2], y[2];
	MCT *Tree = new MCT();				//Ʈ�� ����
										//�ʱ� 8�� ��� �ʱ�ȭ
	MCNode* childNode1 = new MCNode();
	MCNode* childNode2 = new MCNode();
	MCNode* childNode3 = new MCNode();
	MCNode* childNode4 = new MCNode();
	MCNode* childNode5 = new MCNode();
	MCNode* childNode6 = new MCNode();
	MCNode* childNode7 = new MCNode();
	MCNode* childNode8 = new MCNode();


	//�� ��2��°�� or ��ù��° �Ͽ����� ������ ���� �������� ���� ���ƾ���
	




	

	
	


	// �� �κп��� �˰��� ���α׷�(AI)�� �ۼ��Ͻʽÿ�. �⺻ ������ �ڵ带 ���� �Ǵ� �����ϰ� ������ �ڵ带 ����Ͻø� �˴ϴ�.
	// ���� Sample code�� AI�� Random���� ���� ���� Algorithm�� �ۼ��Ǿ� �ֽ��ϴ�.

	srand((unsigned)time(NULL));  //�ð����� ���� �߻�


	for (int i = 0; i < cnt; i++)
	{
		while (!isFree(x[i], y[i])) // ���� �� ������ ������ ����
		{
			x[i] = rand() % width;
			y[i] = rand() % height;
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



