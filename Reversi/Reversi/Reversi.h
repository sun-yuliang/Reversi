#ifndef __REVERSI_H__
#define __REVERSI_H__

class Reversi
{
private:
	int stage;    //棋盘阶段
	int dep;    //搜索深度
	double p_div;    //divergence
	double p_squ;    //adjacent squares  
	double p_fro;    //frontier discs
	double wei[7][4];    //weight
public:
	Reversi();    //构造函数
	char board[10][10];    //棋盘局面
	char ai;    //只有两个值black和white，代表ai执什么棋
	int diff;    //难度=dep
	int stable_b[6561];    //black edge stable discs table
	int stable_w[6561];    //white edge stable discs table
	void get_wei(double d, double s, double f, double w[][4]);    //获得权重参数
	void initial();    //初始化
	void display();    //显示棋盘	
	void copy(char A[][10], char B[][10]);    //复制A到B
	bool isvalid(int loc, char who);    //判断who能否在loc位置落子，loc=10*row+col
	bool haveplace(char who);    //判断who能不能再棋盘上落子
	void move(int loc, char who);    //将who的棋下在loc位置
	void generate_move(char way[40], char who);    //产生who所有走法存到way里
	void get_valid(char who);    //把who能走的地方变成2
	void clear_valid();    //还原2
	int count(char who);    //棋盘里who的个数
	int difference(char who);    //尾盘数子
	double CurrentMobility(char who);    //current mobility
	double PotentialMobility(char who);    //potential mobility
	double EdgeStableDiscs(char who);    //edge stable discs
	double Penalty(char who);    //corner and balanced edge penalty
	void change();    //区分开局中盘尾局
	double evaluate(char who);    //评估函数，who表示对谁估值
	double alphabeta(int depth, double alpha, double beta, char next, char who);    //depth是搜索深度，alpha=-inf，beta=inf，next表示下一个是什么节点(1->max，-1->min)，who表示搜索对谁有利
	void sort_dec(char inp[40], int dep, char next, char who);    //将inp里的走法按估分从大到小排，dep是搜索深度，next表示下一个是什么节点(1->max，-1->min)，who表示对谁估分
	char get_best(char who);    //找who的最优落子位置
};

#endif
