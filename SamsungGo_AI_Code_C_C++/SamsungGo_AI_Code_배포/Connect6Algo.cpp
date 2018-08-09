// Samsung Go Tournament Form C Connect6Algo (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>
#define _CRT_SECURE_NO_WARNINGS 

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Connect6Algo.h"

unsigned s_time;					//시간변수
unsigned l_time;					//마지막 시간변수

int terminateAI;					//turn 종료를 위한 변수
int width = 19, height = 19;
int cnt = 2;						//놓을 수 있는 돌의 수, 기본값 2
int myColor;
FILE *fp=NULL;

static char cmd[256];
static HANDLE event1, event2;
#define BOARD_SIZE 20
static int board[BOARD_SIZE][BOARD_SIZE];
int limitTime=0;

static void getLine() {
	int c, bytes;

	bytes = 0;
	do {
		c = getchar();
		if (c == EOF) exit(0);
		if (bytes < sizeof(cmd)) cmd[bytes++] = (char)c;
	} while (c != '\n');
	cmd[bytes - 1] = 0;
	if (cmd[bytes - 2] == '\r') cmd[bytes - 2] = 0;
}

int setLine(char *fmt, ...) {
	int i;
	va_list va;
	va_start(va, fmt);
	i = vprintf(fmt, va);
	putchar('\n');
	fflush(stdout);
	va_end(va);
	return i;
}

static const char *getParam(const char *command, const char *input) {
	int n1, n2;
	n1 = (int)strlen(command);
	n2 = (int)strlen(input);
	if (n1 > n2 || _strnicmp(command, input, n1)) return NULL;
	input += strlen(command);
	while (isspace(input[0])) input++;	
	return input;
}

static void stop() {			//프로세스 종료

		terminateAI = 1;						//적차례
		WaitForSingleObject(event2, INFINITE);	//적 이벤트 기다림
}

static void start() {			//시작

	//s_time = GetTickCount();			//시작 시간 s_time에 저장
	stop();	
	
}

static void turn() {			//턴시작 함수

	terminateAI = 0;
	ResetEvent(event2);
	SetEvent(event1);
}

void domymove(int x[], int y[], int cnt) {
	char buf[200] = { " " };

	mymove(x, y, cnt);
	if (cnt == 1){
		setLine("%d,%d", x[0], y[0]);
		sprintf_s(buf, "-- output: %d,%d \n", x[0], y[0]);		
	}
	else
	{
		setLine("%d,%d %d,%d", x[0], y[0], x[1], y[1]);
		sprintf_s(buf, "-- output: %d,%d %d,%d \n", x[0], y[0], x[1], y[1]);
	}

	writeLog(buf);
	
}

int showBoard(int x, int y) 
{	//보드의 값 0? 1? 2?
	return board[x][y];
}


static void doCommand() {
	const char *param;


	char buf[200] = { " " };
	sprintf_s(buf, "-- input: %s \n",cmd);
	writeLog(buf);

	if ((param = getParam("START", cmd)) != 0) {	//START 버튼 누르면  바둑판 초기화
		start();
		init();
		setLine("OK");

		char buf[200] = { " " };
		sprintf_s(buf, "-- output: %s \n", "OK");
		writeLog(buf);

	}
	else if ((param = getParam("BEGIN", cmd)) != 0) {	//맨처음 시작할 때(게임의 첫수)
		myColor = 1;									//내 색은 1(흑색)
		cnt = 1;										//cnt값은 1
		start();
		turn();											//턴시작
	}
	else if ((param = getParam("TURN", cmd)) != 0)		//턴이 바뀔때 
	{	
		int x[2], y[2], r;								//입력 4개나 2개가 안들어왔으면 ERROR
		if (((r = sscanf_s(param, "%d,%d %d,%d", &x[0], &y[0], &x[1], &y[1])) != 4 && r != 2))
		{
			setLine("ERROR 형식에 맞지 않는 좌표가 입력되었습니다");

			char buf[200] = { " " };
			sprintf_s(buf, "-- output: %s \n","ERROR 형식에 맞지 않는 좌표가 입력되었습니다" );
			writeLog(buf);

			return;
		}
		else			
		{
			for (int i = 0; i < (r / 2); i++) 
			{
				if (x[i] < 0 || x[i] >= width || y[i] < 0 || y[i] >= height) //19X19에 맞는 좌표인지 검사
				{
					setLine("ERROR 형식에 맞지 않는 좌표가 입력되었습니다");

					char buf[200] = { " " };
					sprintf_s(buf, "-- output: %s \n", "ERROR 형식에 맞지 않는 좌표가 입력되었습니다");
					writeLog(buf);

					return;
				}
			}
			cnt = 2;
			opmove(x, y, r / 2);	//x,y 좌표들 값 + cnt값 전달
			turn();					//턴시작
		}
	}
	else if ((param = getParam("INFO", cmd)) != 0) {
		setLine("%s", info);

		char buf[200] = { " " };
		sprintf_s(buf, "-- output: %s \n", info);
		writeLog(buf);
	}
	else if ((param = getParam("BLOCK", cmd)) != 0) {	
		int x, y;
		if (((sscanf_s(param, "%d,%d", &x, &y)) == 2)) {
			block(x, y);
			setLine("OK");

			char buf[200] = { " " };
			sprintf_s(buf, "-- output: %s \n", "OK");
			writeLog(buf);
		}
	}
	else if ((param = getParam("LimitTime", cmd)) != 0){	//시간제한 옵션에서 입력받아서 limitTime에 저장
		sscanf_s(param, "%d", &limitTime);	
	}
	else if ((param = getParam("QUIT", cmd)) != 0) {
		exit(0);
	}

}

static DWORD WINAPI threadLoop(LPVOID) 
{	
	while (1) 
	{
		WaitForSingleObject(event1, INFINITE);	//내가 돌을 놓을 차례까지 기다림
		s_time = GetTickCount();			//내 프로세스가 시작된 시간

		//턴 계산
		//for (int i = 0; i < 19; i++)
		//{
		//	stage += count(board[i], board[i] + 19, 1 || 2); //board[x][y] 중에서 적이나 아군 값을 카운트
		//}
		//stage = stage / 2 + 1;
		if (player.size() / 2 != 0)
		{
			stage = player.size() / 2;	
		}
		else //stage /2 턴의수
		{
			stage = 1;
		}
	

		char buf[200] = { " " };
		sprintf_s(buf, "TURN!!!!!!!!!!!!!: %d \n", stage);
		writeLog(buf);

		if (cnt == 2)
			myturn(cnt);		//AI_Algorithm_Code.cpp의 우리 함수 여기만 존재
		else //cnt == 1 흑으로 시작이면 무조건 9,9에 둔다.
		{
			int x[2], y[2];
			if (isFree(9, 9))
			{
				x[0] = 9;
				y[0] = 9;
				domymove(x, y, 1);
			}
			else if (isFree(8,8))
			{
				x[0] = 8;
				y[0] = 8;
				domymove(x, y, 1);
			}
			else
			{
				x[0] = 10;
				y[0] = 10;
				domymove(x, y, 1);
			}
		}
		//myturn(cnt);
								
		if (cnt == 1) cnt = 2;				//내가 흑이고 처음 두는 것 이었으면 처음두고, cnt = 2 대입
		
		SetEvent(event2);					//적차례로 이벤트 세팅
	}
}

void writeLog(char *myLog)		//로그 작성
{

	fp = fopen("myLog.txt", "a");

	if (fp != NULL)
	{
		fprintf(fp,myLog);
	}

	fclose(fp);	
}

int main() {		//메인 함수
	DWORD mode;
	

	if (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode))
		puts("직접 실행 불가능한 파일입니다. 육목 알고리즘 대회 툴을 이용해 실행하세요.");

	DWORD tid;
	event1 = CreateEvent(0, FALSE, FALSE, 0);		//내가 돌놓는 이벤트
	CreateThread(0, 0, threadLoop, 0, 0, &tid);
	event2 = CreateEvent(0, TRUE, TRUE, 0);			//적이 돌놓는 이벤트

	while (1) 
	{
		getLine();
		doCommand();
	}

	return 0;
}

int isFree(int x, int y)	//19X19 보드판에 x,y 좌표가 들어가는지 + 아무 것도 놓여진게 없는지
{
	return x >= 0 && y >= 0 && x < width && y < height && board[x][y] == 0;
}

void init() {								//Board 0으로 초기화
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board[i][j] = 0;
		}
	}
}

void mymove(int x[], int y[], int cnt) {	//내가 돌을 놨을 때 = 1 board 정보갱신
	for (int i = 0; i < cnt; i++) {
		if (isFree(x[i], y[i])) 
		{
			board[x[i]][y[i]] = 1;
			if (i == 0)
			{
				Lastmymove1 = make_pair(x[0], y[0]);	//아군 가장 최근 수
				player.push_back(make_pair(Lastmymove1.first, Lastmymove1.second));
			}	
			if (i == 1)
			{
				Lastmymove2 = make_pair(x[1], y[1]);
				player.push_back(make_pair(Lastmymove2.first, Lastmymove2.second));
			}
		}
		else 
		{
			setLine("ERROR 이미 돌이 있는 위치입니다. MY[%d, %d]", x[i], y[i]);
		}
	}
}

void opmove(int x[], int y[], int cnt) {	//적이 돌을 놨을 때 = 2
	for (int i = 0; i < cnt; i++) {
		if (isFree(x[i], y[i])) 
		{
			board[x[i]][y[i]] = 2;
			if (i == 0)
			{
				Lastopmove1 = make_pair(x[0], y[0]);	//적 가장 최근 수
				player.push_back(make_pair(Lastopmove1.first, Lastopmove1.second));
			}
			if (i == 1)
			{
				Lastopmove2 = make_pair(x[1], y[1]);
				player.push_back(make_pair(Lastopmove2.first, Lastopmove2.second));
			}
		}
		else 
		{
			setLine("ERROR 이미 돌이 있는 위치입니다. OP[%d, %d]", x[i], y[i]);
		}
	}
}

void block(int x, int y) {	//블록 돌 놓기 = 3
	if (isFree(x, y)) {
		board[x][y] = 3;
	}
}

