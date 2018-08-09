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
pair <int, int> Lastmymove1;			//최근 아군 첫번째 수
pair <int, int> Lastmymove2;			//최근 아군 두번째 수
pair <int, int> Lastopmove1;			//최근 적군 첫번째 수
pair <int, int> Lastopmove2;			//최근 적군 두번째 수
vector < pair<int, int> > player;		//턴마다 움직인 돌의 정보 아군이 흑으로 시작했으면 인덱스:0과 짝수 / 백이면 홀수




void myturn(int cnt) {

	int x[2], y[2];
	MCT *Tree = new MCT();				//트리 생성
										//초기 8개 노드 초기화
	MCNode* childNode1 = new MCNode();
	MCNode* childNode2 = new MCNode();
	MCNode* childNode3 = new MCNode();
	MCNode* childNode4 = new MCNode();
	MCNode* childNode5 = new MCNode();
	MCNode* childNode6 = new MCNode();
	MCNode* childNode7 = new MCNode();
	MCNode* childNode8 = new MCNode();


	//내 흑2번째턴 or 백첫번째 턴에서는 무조건 적군 마지막수 백을 막아야함
	




	

	
	


	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.

	srand((unsigned)time(NULL));  //시간별로 난수 발생


	for (int i = 0; i < cnt; i++)
	{
		while (!isFree(x[i], y[i])) // 돌이 다 놓여질 때까지 실행
		{
			x[i] = rand() % width;
			y[i] = rand() % height;
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



