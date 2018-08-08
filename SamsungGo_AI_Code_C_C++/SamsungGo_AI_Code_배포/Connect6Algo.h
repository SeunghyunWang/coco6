// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>


#include <iostream>
#include <utility>
#include <vector>
using namespace std;

extern int width, height;	//19X19
extern int terminateAI;		//프로세스 종료를 위한 변수
extern int limitTime;		//제한시간
extern unsigned s_time;		//내 프로세스 시작한 시간
extern unsigned l_time;		//내 프로세스 실행후 현재 시간

extern FILE *fp;


extern int isFree(int x, int y);						//19X19 보드판에 x,y 좌표가 들어가는지 + 아무 것도 놓여진게 없는지
extern int showBoard(int x, int y);						//보드의 값 0? 1? 2? 확인가능
extern void init();										//초기화 함수
extern void restart();									//?? 넌 정체가뭐냐..
extern void mymove(int x[], int y[], int cnt = 2);		//board[x][y] = 1;	내돌로 할당 
extern void opmove(int x[], int y[], int cnt = 2);		//board[x][y] = 2;	적돌로 할당
extern void block(int, int);
extern int setLine(char *fmt, ...);
extern void domymove(int x[], int y[], int cnt = 2);	//돌놓기
extern void writeLog(char *log);						//로그파일작성(디버깅용)

void myturn(int cnt = 2);								//내 AI알고리즘
char info[];

/*
	여기서 부터는 커스텀 변수
	by WSH.

*/

extern int stage;							//지금이 몇번째 턴인지 판단 (board배열에 있는 돌의 개수(값이 1,2 인 것) search 하여 판단)
extern pair <int, int> Lastmymove1;			//최근 아군 첫번째 수
extern pair <int, int> Lastmymove2;			//최근 아군 두번째 수
extern pair <int, int> Lastopmove1;			//최근 적군 첫번째 수
extern pair <int, int> Lastopmove2;			//최근 적군 두번째 수
extern vector < pair<int, int> > player;		//턴마다 움직인 돌의 정보 아군이 흑으로 시작했으면 인덱스:0과 짝수 / 백이면 홀수

typedef struct MCT
{
	int ES;					//가중치( Evaluation Score  )
	int depth;				//트리의 깊이 (or 노드 방문 횟수)
	vector <MCT *> child;	//다음 자식노드를 저장할 벡터

}MCT;

/*
	커스텀 함수
*/

