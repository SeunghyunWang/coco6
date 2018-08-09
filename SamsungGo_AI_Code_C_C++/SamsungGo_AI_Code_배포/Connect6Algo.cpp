// Samsung Go Tournament Form C Connect6Algo (g++-4.8.3)

// <--------------- �� Code�� �����ϸ�  �۵����� ���� �� �ֽ��ϴ� ------------------>
#define _CRT_SECURE_NO_WARNINGS 

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Connect6Algo.h"

unsigned s_time;					//�ð�����
unsigned l_time;					//������ �ð�����

int terminateAI;					//turn ���Ḧ ���� ����
int width = 19, height = 19;
int cnt = 2;						//���� �� �ִ� ���� ��, �⺻�� 2
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

static void stop() {			//���μ��� ����

		terminateAI = 1;						//������
		WaitForSingleObject(event2, INFINITE);	//�� �̺�Ʈ ��ٸ�
}

static void start() {			//����

	//s_time = GetTickCount();			//���� �ð� s_time�� ����
	stop();	
	
}

static void turn() {			//�Ͻ��� �Լ�

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
{	//������ �� 0? 1? 2?
	return board[x][y];
}


static void doCommand() {
	const char *param;


	char buf[200] = { " " };
	sprintf_s(buf, "-- input: %s \n",cmd);
	writeLog(buf);

	if ((param = getParam("START", cmd)) != 0) {	//START ��ư ������  �ٵ��� �ʱ�ȭ
		start();
		init();
		setLine("OK");

		char buf[200] = { " " };
		sprintf_s(buf, "-- output: %s \n", "OK");
		writeLog(buf);

	}
	else if ((param = getParam("BEGIN", cmd)) != 0) {	//��ó�� ������ ��(������ ù��)
		myColor = 1;									//�� ���� 1(���)
		cnt = 1;										//cnt���� 1
		start();
		turn();											//�Ͻ���
	}
	else if ((param = getParam("TURN", cmd)) != 0)		//���� �ٲ� 
	{	
		int x[2], y[2], r;								//�Է� 4���� 2���� �ȵ������� ERROR
		if (((r = sscanf_s(param, "%d,%d %d,%d", &x[0], &y[0], &x[1], &y[1])) != 4 && r != 2))
		{
			setLine("ERROR ���Ŀ� ���� �ʴ� ��ǥ�� �ԷµǾ����ϴ�");

			char buf[200] = { " " };
			sprintf_s(buf, "-- output: %s \n","ERROR ���Ŀ� ���� �ʴ� ��ǥ�� �ԷµǾ����ϴ�" );
			writeLog(buf);

			return;
		}
		else			
		{
			for (int i = 0; i < (r / 2); i++) 
			{
				if (x[i] < 0 || x[i] >= width || y[i] < 0 || y[i] >= height) //19X19�� �´� ��ǥ���� �˻�
				{
					setLine("ERROR ���Ŀ� ���� �ʴ� ��ǥ�� �ԷµǾ����ϴ�");

					char buf[200] = { " " };
					sprintf_s(buf, "-- output: %s \n", "ERROR ���Ŀ� ���� �ʴ� ��ǥ�� �ԷµǾ����ϴ�");
					writeLog(buf);

					return;
				}
			}
			cnt = 2;
			opmove(x, y, r / 2);	//x,y ��ǥ�� �� + cnt�� ����
			turn();					//�Ͻ���
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
	else if ((param = getParam("LimitTime", cmd)) != 0){	//�ð����� �ɼǿ��� �Է¹޾Ƽ� limitTime�� ����
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
		WaitForSingleObject(event1, INFINITE);	//���� ���� ���� ���ʱ��� ��ٸ�
		s_time = GetTickCount();			//�� ���μ����� ���۵� �ð�

		//�� ���
		//for (int i = 0; i < 19; i++)
		//{
		//	stage += count(board[i], board[i] + 19, 1 || 2); //board[x][y] �߿��� ���̳� �Ʊ� ���� ī��Ʈ
		//}
		//stage = stage / 2 + 1;
		if (player.size() / 2 != 0)
		{
			stage = player.size() / 2;	
		}
		else //stage /2 ���Ǽ�
		{
			stage = 1;
		}
	

		char buf[200] = { " " };
		sprintf_s(buf, "TURN!!!!!!!!!!!!!: %d \n", stage);
		writeLog(buf);

		if (cnt == 2)
			myturn(cnt);		//AI_Algorithm_Code.cpp�� �츮 �Լ� ���⸸ ����
		else //cnt == 1 ������ �����̸� ������ 9,9�� �д�.
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
								
		if (cnt == 1) cnt = 2;				//���� ���̰� ó�� �δ� �� �̾����� ó���ΰ�, cnt = 2 ����
		
		SetEvent(event2);					//�����ʷ� �̺�Ʈ ����
	}
}

void writeLog(char *myLog)		//�α� �ۼ�
{

	fp = fopen("myLog.txt", "a");

	if (fp != NULL)
	{
		fprintf(fp,myLog);
	}

	fclose(fp);	
}

int main() {		//���� �Լ�
	DWORD mode;
	

	if (GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode))
		puts("���� ���� �Ұ����� �����Դϴ�. ���� �˰��� ��ȸ ���� �̿��� �����ϼ���.");

	DWORD tid;
	event1 = CreateEvent(0, FALSE, FALSE, 0);		//���� ������ �̺�Ʈ
	CreateThread(0, 0, threadLoop, 0, 0, &tid);
	event2 = CreateEvent(0, TRUE, TRUE, 0);			//���� ������ �̺�Ʈ

	while (1) 
	{
		getLine();
		doCommand();
	}

	return 0;
}

int isFree(int x, int y)	//19X19 �����ǿ� x,y ��ǥ�� ������ + �ƹ� �͵� �������� ������
{
	return x >= 0 && y >= 0 && x < width && y < height && board[x][y] == 0;
}

void init() {								//Board 0���� �ʱ�ȭ
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board[i][j] = 0;
		}
	}
}

void mymove(int x[], int y[], int cnt) {	//���� ���� ���� �� = 1 board ��������
	for (int i = 0; i < cnt; i++) {
		if (isFree(x[i], y[i])) 
		{
			board[x[i]][y[i]] = 1;
			if (i == 0)
			{
				Lastmymove1 = make_pair(x[0], y[0]);	//�Ʊ� ���� �ֱ� ��
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
			setLine("ERROR �̹� ���� �ִ� ��ġ�Դϴ�. MY[%d, %d]", x[i], y[i]);
		}
	}
}

void opmove(int x[], int y[], int cnt) {	//���� ���� ���� �� = 2
	for (int i = 0; i < cnt; i++) {
		if (isFree(x[i], y[i])) 
		{
			board[x[i]][y[i]] = 2;
			if (i == 0)
			{
				Lastopmove1 = make_pair(x[0], y[0]);	//�� ���� �ֱ� ��
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
			setLine("ERROR �̹� ���� �ִ� ��ġ�Դϴ�. OP[%d, %d]", x[i], y[i]);
		}
	}
}

void block(int x, int y) {	//��� �� ���� = 3
	if (isFree(x, y)) {
		board[x][y] = 3;
	}
}

