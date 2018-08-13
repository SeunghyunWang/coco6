// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 sds소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean isFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/
#pragma once
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "Connect6Algo.h"
using namespace std;

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:눔뇽, Department:숭실대학교" };

/*
	커스텀 변수들
*/

int stage;							//지금이 몇번째 턴인지 판단 (board배열에 있는 돌의 개수(값이 1,2 인 것) search 하여 판단)
int BW;									//내가 흑시작인지 백시작인지
pair <int, int> Lastmymove1;			//최근 아군 첫번째 수
pair <int, int> Lastmymove2;			//최근 아군 두번째 수
pair <int, int> Lastopmove1;			//최근 적군 첫번째 수
pair <int, int> Lastopmove2;			//최근 적군 두번째 수
vector < pair<int, int> > player;		//턴마다 움직인 돌의 정보 아군이 흑으로 시작했으면 인덱스:0과 짝수 / 백이면 홀수
vector < pair<int, int> > simulplayer;
int human;								//TSS에서 아군 적군 판별


double CountStone(deque <int> &dq)		//1차원 큐에 대한 TS 판단 함수
{
	double TS = 0.0;
	int humancnt = 0;
	int semihuman = 0;
	for (int i = 0; i < dq.size(); i++)
	{
		if (dq[i] == human && i + 1 < dq.size() && dq[i + 1] == human) //첫 좌표가 1이나 2이고(human) 다음 수도 같다면?
		{
			humancnt += 10;
		}
		//처음에 빈칸인경우 아직 안함
		else if (dq[i] == human && i + 1<dq.size() && dq[i + 1] == 0
			&& humancnt >= 10 && i + 2 <dq.size() && dq[i + 2] == human)//연속된 상황이 끊겼는데(일단 dq[i]==human, humancnt>=20이고, 다음것이 빈칸이고 그다음것이 human이라면? 
		{
			humancnt += 5;

		}
		else
		{

		}

	}
	if (humancnt >= 30) //human 돌 4개연속
	{
		return TS = 10000.0;
	}
	else if (humancnt >= 20)	//human 돌 3개 연속 + 다음 빈칸 + human
	{
		return TS = 500.0;
	}
	else if (humancnt >= 10)	//human 2개 연속 + 다음 빈칸 + human
	{
		return TS = 10.0;
	}
	else
	{
		return TS;
	}


}


double TSS(int x, int y)		//한 좌표(최근에 적과 아군이 둔 수들 중 하나)에 대한 TS를 탐색하여 ES계산
{
	//extern double CountStone(deque <int> dq);
	double TS = 0.0;
	deque <int> dq;
	for (int i = player.size() - 1; i > 0; i--)
	{
		if (x >= 0 && y >= 0 && x < width && y < height)
		{

			int tempfx, tempbx, tempfy, tempby;

			if (showBoard(x, y) == 1)	//좌표가 아군이면
			{
				human = 1;
			}
			else if (showBoard(x, y) == 2) //좌표가 적군이면
			{
				human = 2;
			}
			else
			{
				return 0.0; //잘못된 좌표 들어온것 
			}
			//////가로 DFS
			dq.push_back(human);	//x,y 좌표에 있는 0,1,2 중에 값 대입 (기준값)
			for (int i = 1; i <= 6; i++)		//board의 가로 방향 DFS, 검색되는 애들은 디큐에 푸시
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


			///////세로 DFS
			dq.push_back(showBoard(x, y));	//x,y 좌표에 있는 0,1,2 중에 값 대입 (기준값)
			for (int i = 1; i <= 6; i++)		//board의 가로 방향 DFS, 검색되는 애들은 디큐에 푸시
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

			///////우대각 DFS
			dq.push_back(showBoard(x, y));	//x,y 좌표에 있는 0,1,2 중에 값 대입 (기준값)
			for (int i = 1; i <= 6; i++)		//board의 가로 방향 DFS, 검색되는 애들은 디큐에 푸시
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

			///////좌대각 DFS
			dq.push_back(showBoard(x, y));	//x,y 좌표에 있는 0,1,2 중에 값 대입 (기준값)
			for (int i = 1; i <= 6; i++)		//board의 가로 방향 DFS, 검색되는 애들은 디큐에 푸시
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
	MCT *Tree = new MCT();				//트리 생성
										//초기 8개 노드 초기화
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

	//내 흑2번째턴 or 백첫번째 턴에서는 무조건 적군 마지막수 백을 막아야함

	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	srand((unsigned)time(NULL));  //시간별로 난수 발생
	
	int tmpx=0, tmpy=0;

	for (int i = 0; i < cnt; i++)
	{
		while (!isFree(x[i], y[i])) // 돌이 다 놓여질 때까지 실행
		{
			x[i] = rand() % width;	//현재 스테이지에서 최선의 수
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

			if (terminateAI == 1) return;	//true이면 turn 종료



		}



		if (x[1] == x[0] && y[1] == y[0]) //첫수와 두수가 같으면 에러임 다시 시도
			i--;
	}


















	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.

	while (1)
	{
		l_time = GetTickCount();

		if (l_time - s_time >= (unsigned int)((limitTime * 1000) - 2500)) //1000이 시간기본단위,250 (0.25초) 
		{
			char buf[200] = { " " };
			sprintf_s(buf, "l_time: %u  s_time: %u  limitTime: %d 종료까지 남은시간: %u \n", l_time, s_time, limitTime, l_time - s_time);
			writeLog(buf);

			domymove(x, y, cnt);
			break;
		}

		//domymove(x, y, cnt);
		//break;
	}
}



