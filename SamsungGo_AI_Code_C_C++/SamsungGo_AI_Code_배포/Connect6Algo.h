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

/*
	Ŀ���� �Լ�
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
		double ES;					//����ġ( Evaluation Score  )
		int depth;				//Ʈ���� ���� (or ��� �湮 Ƚ��)
		int cntchild;			//�ڽĳ���� ��
		int treeboard[19][19];	//�ùķ��̼��� ����
		MCNode* parent;			//�θ��� ������
		vector <MCNode*> child;	//���� �ڽĳ��'��'�� ������ ����

	public:		
		MCNode()				//������
		{
			MCNode *root = new MCNode();
			ES = 0.0;
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
			ES = 0.0;
			this->depth = parent->depth + 1;
			cntchild = 0;
			Inittreeboard();
			this->parent = parent;
		}
		~MCNode()			//�Ҹ���
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
		MCNode* GetChild(int i)
		{
			if (child[i] != NULL)
				return child[i];
		}
		int Getcntchild()
		{
			return cntchild;
		}
		double GetES()
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

		MCNode *root;	//��Ʈ���
		MCNode *nownode;//���� ������� ���
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
		bool isTerminal() // �ùķ��̼ǿ��� �¸����� C6 �޼� �ߴ���(��or�Ʊ�)
		{
			return false;
		}
		void InitMCT()
		{
			root = new MCNode();
			nownode = root;
			MCTSelect(root);	//������ ���ÿ� MCTS ����
		}
		MCNode* &Getroot()
		{
			return root;
		}
		void Setnownode(MCNode* &nownode)
		{
			this->nownode = nownode;
		}

		void MCTSelect(MCNode* &toSelect)//���� ����� �ڽĳ���� �����ϴ� �Լ�
		{
			if (0) //�͹̳����� �ƴ��� �����ϴ� �Լ� �ʿ� => TSS�Լ��ʿ�
			{



			}


			else if (toSelect->Anychild(toSelect))
			{
				vector <MCNode*> tempchilds;
				for (int i = 0; toSelect->Getcntchild(); i++)
				{
					tempchilds[i] = toSelect->GetChild(i);
				}

				MCNode* bestchild = new MCNode();
				double tempmax = 0.0;
				/////////////TSS ����ġ ���κ� with  UCB



				///////////////////////////////

				for (int i = 0; i < toSelect->Getcntchild(); i++)
				{
					if (tempmax < tempchilds[i]->GetES())
					{
						tempmax = tempchilds[i]->GetES();
						bestchild = tempchilds[i];
					}
				}
				MCTSelect(bestchild);
			}
			else
			{
				if (toSelect->Getdepth()<40)
					MCTExpand(toSelect);
			}
			
		}

		void MCTExpand(MCNode* &Node)// Ž���� �ڽ� ��尡 ������ Ȯ��
		{
			/* ���� �ֱ� �� 8���⿡(isFree �����ϴ� �͸� �Ҵ�) ���� ���� �Լ� �ʿ�

			*/
			MCNode* childNode1 = new MCNode(Node);
			MCNode* childNode2 = new MCNode(Node);
			MCNode* childNode3 = new MCNode(Node);
			MCNode* childNode4 = new MCNode(Node);
			MCNode* childNode5 = new MCNode(Node);
			MCNode* childNode6 = new MCNode(Node);
			MCNode* childNode7 = new MCNode(Node);
			MCNode* childNode8 = new MCNode(Node);

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
		void MCTSimul()
		{

		}
		void MCTBackPro()
		{

		}
		inline double MCTUSB(MCNode* cur, MCNode* child, double num);

};
inline double MCT::MCTUSB(MCNode* cur, MCNode* child, double num)//����ġ ��� �˰���
{
	double factor = 0.0;
	if (cur == NULL)
		

	return factor;
}