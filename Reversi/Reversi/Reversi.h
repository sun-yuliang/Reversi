#ifndef __REVERSI_H__
#define __REVERSI_H__

class Reversi
{
private:
	int stage;    //���̽׶�
	int dep;    //�������
	double p_div;    //divergence
	double p_squ;    //adjacent squares  
	double p_fro;    //frontier discs
	double wei[7][4];    //weight
public:
	Reversi();    //���캯��
	char board[10][10];    //���̾���
	char ai;    //ֻ������ֵblack��white������aiִʲô��
	int diff;    //�Ѷ�=dep
	int stable_b[6561];    //black edge stable discs table
	int stable_w[6561];    //white edge stable discs table
	void get_wei(double d, double s, double f, double w[][4]);    //���Ȩ�ز���
	void initial();    //��ʼ��
	void display();    //��ʾ����	
	void copy(char A[][10], char B[][10]);    //����A��B
	bool isvalid(int loc, char who);    //�ж�who�ܷ���locλ�����ӣ�loc=10*row+col
	bool haveplace(char who);    //�ж�who�ܲ���������������
	void move(int loc, char who);    //��who��������locλ��
	void generate_move(char way[40], char who);    //����who�����߷��浽way��
	void get_valid(char who);    //��who���ߵĵط����2
	void clear_valid();    //��ԭ2
	int count(char who);    //������who�ĸ���
	int difference(char who);    //β������
	double CurrentMobility(char who);    //current mobility
	double PotentialMobility(char who);    //potential mobility
	double EdgeStableDiscs(char who);    //edge stable discs
	double Penalty(char who);    //corner and balanced edge penalty
	void change();    //���ֿ�������β��
	double evaluate(char who);    //����������who��ʾ��˭��ֵ
	double alphabeta(int depth, double alpha, double beta, char next, char who);    //depth��������ȣ�alpha=-inf��beta=inf��next��ʾ��һ����ʲô�ڵ�(1->max��-1->min)��who��ʾ������˭����
	void sort_dec(char inp[40], int dep, char next, char who);    //��inp����߷������ִӴ�С�ţ�dep��������ȣ�next��ʾ��һ����ʲô�ڵ�(1->max��-1->min)��who��ʾ��˭����
	char get_best(char who);    //��who����������λ��
};

#endif
