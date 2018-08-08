// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- �� Code�� �����ϸ�  �۵����� ���� �� �ֽ��ϴ� ------------------>


#include <iostream>
#include <utility>
#include <vector>
using namespace std;

extern int width, height;	//19X19
extern int terminateAI;		//���μ��� ���Ḧ ���� ����
extern int limitTime;		//���ѽð�
extern unsigned s_time;		//�� ���μ��� ������ �ð�
extern unsigned l_time;		//�� ���μ��� ������ ���� �ð�

extern FILE *fp;


extern int isFree(int x, int y);						//19X19 �����ǿ� x,y ��ǥ�� ������ + �ƹ� �͵� �������� ������
extern int showBoard(int x, int y);						//������ �� 0? 1? 2? Ȯ�ΰ���
extern void init();										//�ʱ�ȭ �Լ�
extern void restart();									//?? �� ��ü������..
extern void mymove(int x[], int y[], int cnt = 2);		//board[x][y] = 1;	������ �Ҵ� 
extern void opmove(int x[], int y[], int cnt = 2);		//board[x][y] = 2;	������ �Ҵ�
extern void block(int, int);
extern int setLine(char *fmt, ...);
extern void domymove(int x[], int y[], int cnt = 2);	//������
extern void writeLog(char *log);						//�α������ۼ�(������)

void myturn(int cnt = 2);								//�� AI�˰���
char info[];

/*
	���⼭ ���ʹ� Ŀ���� ����
	by WSH.

*/

extern int stage;							//������ ���° ������ �Ǵ� (board�迭�� �ִ� ���� ����(���� 1,2 �� ��) search �Ͽ� �Ǵ�)
extern pair <int, int> Lastmymove1;			//�ֱ� �Ʊ� ù��° ��
extern pair <int, int> Lastmymove2;			//�ֱ� �Ʊ� �ι�° ��
extern pair <int, int> Lastopmove1;			//�ֱ� ���� ù��° ��
extern pair <int, int> Lastopmove2;			//�ֱ� ���� �ι�° ��
extern vector < pair<int, int> > player;		//�ϸ��� ������ ���� ���� �Ʊ��� ������ ���������� �ε���:0�� ¦�� / ���̸� Ȧ��

typedef struct MCT
{
	int ES;					//����ġ( Evaluation Score  )
	int depth;				//Ʈ���� ���� (or ��� �湮 Ƚ��)
	vector <MCT *> child;	//���� �ڽĳ�带 ������ ����

}MCT;

/*
	Ŀ���� �Լ�
*/

