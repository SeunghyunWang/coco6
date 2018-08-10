// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>

#pragma once
#include <iostream>
#include <deque>
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
extern vector < pair<int, int> > simulplayer;
extern double TSS(int x, int y);
/*
	커스텀 함수
*/
//void isTerminal();
//void CreateMCT();
//
//void MCTSelect();
//void MCTExpand();
//void MCTSimul();
//void MCTBackPro();



class MCNode
{
	private:
		double ES;					//가중치( Evaluation Score  )
		int depth;				//트리의 깊이 (or 노드 방문 횟수)
		int cntchild;			//자식노드의 수
		int treeboard[19][19];	//시뮬레이션할 보드
		MCNode* parent;			//부모노드 포인터
		vector <MCNode*> child;	//다음 자식노드'들'을 저장할 벡터

	public:		
		MCNode()				//생성자
		{
			MCNode *root = new MCNode();

			for (int i = player.size() - 1; i > 0; i--)
			{
				ES += TSS( player[i].first, player[i].second );	//@@@@@@@노드 생성시 현재 board 상황에 대한 ES값 저장 
			}

			depth = 0;
			cntchild = 0;			
			Inittreeboard();
			parent = NULL;
		}
		MCNode(int depth)		
		{
			ES = 0.0;
			this->depth = depth;
			cntchild = 0;
			Inittreeboard();
			parent = NULL;			
		}
		MCNode(MCNode* parent)
		{
			for (int i = player.size() - 1; i > 0; i--)
			{
				ES += TSS(player[i].first, player[i].second);	//@@@@@@@노드 생성시 현재 board 상황에 대한 ES값 저장 
			}
			this->depth = parent->depth + 1;
			cntchild = 0;
			Inittreeboard();
			this->parent = parent;
		}
		MCNode(MCNode* parent , int dirx, int diry)
		{
			for (int i = player.size() - 1; i > 0; i--)
			{
				ES += TSS(player[i].first + dirx, player[i].second +diry );	//@@@@@@@노드 생성시 현재 board 상황에 대한 ES값 저장 
			}
			this->depth = parent->depth + 1;
			cntchild = 0;
			Inittreeboard();
			this->parent = parent;
		}
		~MCNode()			//소멸자
		{
			DeleteMCNode(child);
		}

		void UpdateES(double ES)
		{
			this->ES = ES;
		}
		void AddChild(MCNode* &child)
		{
			this->child.push_back(child);
			//child->parent = s
			++cntchild;
		}
		bool Anychild(MCNode* &node)
		{
			for (int i = 0; i < node->Getcntchild(); i++)
			{
				if (node->GetChild(i)!=NULL)
				{
					return true;
				}
			}
			return false;			
		}
		vector <MCNode*> &GetChilds()
		{
			return child;
		}
		MCNode* &GetChild(int i)
		{
			if (child[i] != NULL)
				return child[i];
		}
		MCNode* &GetParent()
		{
			return parent;
		}
		int Getcntchild()
		{
			return cntchild;
		}
		double &GetES()
		{
			return ES;
		}
		double Getdepth()
		{
			return depth;
		}
		void Inittreeboard()
		{
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					treeboard[i][j] = showBoard(i, j);
				}
			}
		}
		void DeleteMCNode(vector <MCNode*> &node)
		{
			if (node.empty())return;

			for (int i = 0; i < cntchild; i++)
				delete node[i];
			cntchild = 0;
		}
};

class MCT
{
	public:

		MCNode *root;	//루트노드
		MCNode *nownode;//현재 계산중인 노드
		MCT()
		{
			root = NULL;
			nownode = NULL;
			InitMCT();
		}
		~MCT()
		{
			delete root;
		}
		bool isTerminal() // 시뮬레이션에서 승리조건 C6 달성 했는지(적or아군)
		{
			return false;
		}
		void InitMCT()
		{
			root = new MCNode();
			nownode = root;
			MCTSelect(root);	//생성과 동시에 MCTS 시작
		}
		MCNode* &Getroot()
		{
			return root;
		}
		void Setnownode(MCNode* &nownode)
		{
			this->nownode = nownode;
		}

		void MCTSelect(MCNode* &toSelect)//현재 노드의 자식노드중 선택하는 함수
		{
			int selectdir = 0;
			double temp;
			if (temp=(toSelect->GetES()) >5000.0 ) //터미널인지 아닌지 구분하는 함수@@@@@@@@@@@@@@@
			{
				MCTBackPro(toSelect);
			}
			else if (toSelect->Anychild(toSelect)) //자식이 있을때
			{
				vector <MCNode*> tempchilds;
				for (int i = 0; toSelect->Getcntchild(); i++)
				{
					tempchilds[i] = toSelect->GetChild(i);
				}

				MCNode* bestchild = new MCNode();
				double tempmax1 = 0.0;
				double tempmax2 = 0.0;
				
				for (int i = 0; toSelect->Getcntchild(); i++)
				{
					tempchilds[i] = toSelect->GetChild(i);
				}

				/////////////TSS 가중치 계산부분 with  UCB
				for (int i = 0; i < toSelect->Getcntchild(); i++)
				{
					tempmax2 = MCTUSB(tempchilds[i], tempchilds[i]->GetES());

					if (tempmax1 <= tempmax2)
					{
						tempmax1 = tempmax2;
						bestchild = tempchilds[i];
						selectdir = i;
					}
					else
					{
					}
				}
				///////////////////////////////
				
				MCTSelect(bestchild);
			}
			else
			{
				if (toSelect->Getdepth()<40)
					MCTExpand(toSelect);
			}
			
		}

		void MCTExpand(MCNode* &Node)// 탐색중 자식 노드가 없으면 확장
		{
			/* 적의 최근 수 8방향에(isFree 만족하는 것만 할당) 돌을 놓는 함수 필요

			*/
			MCNode* childNode1 = new MCNode(Node, 1, 0);//x+1
			MCNode* childNode2 = new MCNode(Node, 0, 1);//y+1
			MCNode* childNode3 = new MCNode(Node, 1, 1);//y+1
			MCNode* childNode4 = new MCNode(Node, -1, 0);//x-1
			MCNode* childNode5 = new MCNode(Node, 0, -1);//y-1
			MCNode* childNode6 = new MCNode(Node, -1, -1);//x-1 y-1
			MCNode* childNode7 = new MCNode(Node, 1, -1); //x + 1 y - 1
			MCNode* childNode8 = new MCNode(Node, -1, 1);//x -1 1 y + 1

			Node->AddChild(childNode1);
			Node->AddChild(childNode2);
			Node->AddChild(childNode3);
			Node->AddChild(childNode4);
			Node->AddChild(childNode5);
			Node->AddChild(childNode6);
			Node->AddChild(childNode7);
			Node->AddChild(childNode8);

			MCTSelect(Node);
		}
		void MCTSimul(MCNode* &Node)
		{
			//가중치로 시뮬레이션 필요

			MCTBackPro(Node);
		}
		void MCTBackPro(MCNode* &Node)
		{
			MCNode* tempNode = new MCNode();
			tempNode = Node->GetParent();
			while (Node->GetParent()!=NULL)
			{
				tempNode->GetES() += Node->GetES(); //가중치값 위로 갈수록 합산
				MCTBackPro(Node->GetParent());	// 부모노드로 계속 이동
			}
		}
		inline double MCTUSB(MCNode* Node, double TS);

};
inline double MCT::MCTUSB(MCNode* Node, double TS)//가중치 계산 알고리즘
{
	double factor = 0.0;
	if (Node == NULL)
	{
		factor = 0.0;
	}
	else
	{
		int totalchilddepth=0;
		for (int i = 0; i < Node->Getcntchild(); i++)
		{
			totalchilddepth += Node->GetChild(i)->Getdepth();
		}

		factor = TS + sqrt(2.0 * log((double)Node->Getdepth() / (double)totalchilddepth));		
	}
	return factor;
}