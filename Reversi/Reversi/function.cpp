#include <iostream>
#include "Reversi.h"
using namespace std;
#define black 1    //black ��
#define white -1    //white ��
#define sign 2    //`
#define inf 9999
#define cor 749
#define bal 249
#define X 449
#define C 249

Reversi::Reversi()
{
	char i = 0, j = 0;
	stage = 0; dep = 0; ai = black; diff = 0;
	p_div = 0; p_squ = 0; p_fro = 0;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 4; j++) wei[i][j] = 0;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++) board[i][j] = 0;
	board[4][5] = black;
	board[5][4] = black;
	board[4][4] = white;
	board[5][5] = white;    
}

void Reversi::initial()
{
	char i = 0, j = 0;
	stage = 1;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++) board[i][j] = 0;
	board[4][5] = black;
	board[5][4] = black;
	board[4][4] = white;
	board[5][5] = white;
}

void Reversi::get_wei(double d, double s, double f, double w[][4])
{
	char i = 0, j = 0;
	p_div = d;
	p_squ = s;
	p_fro = f;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 4; j++) wei[i][j] = w[i][j];
}

void Reversi::display()		// modified to support win10 console in 1920 * 1080 dpi by Sun in 2019.4.29
{
	int i = 1, j = 0;
	cout << "     a  b  c  d  e  f  g  h" << endl;    //abcdefgh    
	//��1��
	cout << "   �������Щ����Щ����Щ����Щ����Щ����Щ����Щ�����" << endl;
	cout << " " << i << " ";
	for (j = 1; j <= 8; j++)
	{
		if (board[i][j] == black) cout << "����";
		else if (board[i][j] == white) cout << "����";
		else if (board[i][j] == sign) cout << "�� `";
		else cout << "��  ";
	}
	cout << "��" << endl;
	//2-8��
	for (i = 2; i <= 8; i++)    
	{
		cout << "   �������੥���੥���੥���੥���੥���੥���੥����" << endl;
		cout << " " << i << " ";
		for (j = 1; j <= 8; j++)
		{
			if (board[i][j] == black) cout << "����";
			else if (board[i][j] == white) cout << "����";
			else if (board[i][j] == sign) cout << "�� `";
			else cout << "��  ";
		}
		cout << "��" << endl;
	}
	//last
	cout << "   �������ة����ة����ة����ة����ة����ة����ة�����" << endl;
}

void Reversi::copy(char A[][10], char B[][10])
{
	char i = 0, j = 0;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++) B[i][j] = A[i][j];
}

bool Reversi::isvalid(int loc, char who)
{
	char i = 0;
	char const r = loc / 10;
	char const c = loc % 10;    //����
	if (board[r][c] != 0)  return false;    //��λ��������   	
	//�ж�˼·�����ж��������Ƿ��ǶԷ��ģ����ǣ�˳���������ҷ����ӣ��м��пյ�break	
	if (board[r - 1][c] == -who)    //��  
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c] == 0) break;    
			else if (board[r - i][c] == who) return true;    			
		}
	}	
	if (board[r + 1][c] == -who)    //��    
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c] == 0) break;   
			else if (board[r + i][c] == who) return true;    
		}
	}
	if (board[r][c - 1] == -who)    //��  
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r][c - i] == 0) break;    
			else if (board[r][c - i] == who) return true;   
		}
	}
	if (board[r][c + 1] == -who)    //��   
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r][c + i] == 0) break;    
			else if (board[r][c + i] == who) return true;    
		}
	}
	if (board[r - 1][c - 1] == -who)    //����	 
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c - i] == 0) break;   
			else if (board[r - i][c - i] == who) return true;    
		}
	}
	if (board[r - 1][c + 1] == -who)    //����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c + i] == 0) break;
			else if (board[r - i][c + i] == who) return true;
		}
	}
	if (board[r + 1][c - 1] == -who)	//����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c - i] == 0) break;   
			else if (board[r + i][c - i] == who) return true;    
		}
	}	
	if (board[r + 1][c + 1] == -who)	//����	 
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c + i] == 0) break;    
			else if (board[r + i][c + i] == who) return true;    
		}
	}
	return false;    //�����������
}

bool Reversi::haveplace(char who)
{
	char i = 0, j = 0;
	bool flag = false;
	for (i = 1; i <= 8; i++)
		for (j = 1; j <= 8; j++)
			if (isvalid(10 * i + j, who)) flag = true;
	return flag;
}

void Reversi::move(int loc, char who)
{
	char i = 0, j = 0;
	char const r = loc / 10;
	char const c = loc % 10;    //����
	board[r][c] = who;    //��λ��������
	if (board[r - 1][c] == -who)    //��  
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c] == 0) break;
			else if (board[r - i][c] == who)    
			{
				for (j = r; j >= r - i + 1; j--) board[j][c] = who;    //��ת
				break;    
			}
		}
	}
	if (board[r + 1][c] == -who)    //��
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c] == 0) break;    
			else if (board[r + i][c] == who)    
			{
				for (j = r; j <= r + i - 1; j++) board[j][c] = who;
				break;    
			}
		}
	}
	if (board[r][c - 1] == -who)	//��
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r][c - i] == 0) break;    
			else if (board[r][c - i] == who)    
			{
				for (j = c; j >= c - i + 1; j--) board[r][j] = who;
				break;   
			}
		}
	}
	if (board[r][c + 1] == -who)    //��
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r][c + i] == 0) break;    
			else if (board[r][c + i] == who)    
			{
				for (j = c; j <= c + i - 1; j++) board[r][j] = who;
				break;   
			}
		}
	}
	if (board[r - 1][c - 1] == -who)	//����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c - i] == 0) break;   
			else if (board[r - i][c - i] == who)    
			{
				for (j = 0; j <= i - 1; j++) board[r - j][c - j] = who;
				break;    
			}
		}
	}
	if (board[r - 1][c + 1] == -who)	 //����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r - i][c + i] == 0) break;   
			else if (board[r - i][c + i] == who)    
			{
				for (j = 0; j <= i - 1; j++) board[r - j][c + j] = who;
				break;    
			}
		}
	}
	if (board[r + 1][c - 1] == -who)	 //����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c - i] == 0) break;   
			else if (board[r + i][c - i] == who)    
			{
				for (j = 0; j <= i - 1; j++) board[r + j][c - j] = who;
				break;    
			}
		}
	}
	if (board[r + 1][c + 1] == -who)	 //����
	{
		for (i = 2; i <= 7; i++)
		{
			if (board[r + i][c + i] == 0) break;   
			else if (board[r + i][c + i] == who)    
			{
				for (j = 0; j <= i - 1; j++) board[r + j][c + j] = who;
				break;    
			}
		}
	}
}

void Reversi::generate_move(char way[40], char who)
{
	//way���who�����ӵ�����λ�ã��������У�
	char i = 0, j = 0, k = 0;
	for (i = 1; i <= 8; i++)
		for (j = 1; j <= 8; j++)
			if (isvalid(10 * i + j, who)) way[k++] = 10 * i + j;
}

void Reversi::get_valid(char who)
{
	//��who���ߵĵط����sign
	char way[40];
	char i = 0;
	for (i = 0; i < 40; i++) way[i] = 0;
	generate_move(way, who);
	for (i = 0; i < 40; i++)
		if (way[i] != 0) board[way[i] / 10][way[i] % 10] = sign;
}

void Reversi::clear_valid()
{
	//ɾ��sign
	char i = 0, j = 0;
	for (i = 1; i <= 8; i++)
		for (j = 1; j <= 8; j++)
			if (board[i][j] == sign) board[i][j] = 0;
}

int Reversi::count(char who)
{
	char i = 0, j = 0, cou = 0;
	for (i = 1; i <= 8; i++)
		for (j = 1; j <= 8; j++)
			if (board[i][j] == who) cou++;
	return cou;
}

int Reversi::difference(char who)
{
	//β�ֱȷ�֮��
	int res = 0;
	char i = 0, j = 0;
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			if (board[i][j] == who) res++;
			else if (board[i][j] == -who) res--;
		}
	}
	return res;
}

double Reversi::CurrentMobility(char who)
{
	double count_b = 0, count_w = 0;
	char i = 0, j = 0;
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			if (isvalid(10 * i + j, black)) count_b++;    //��������ӵ�
			if (isvalid(10 * i + j, white)) count_w++;    //��������ӵ�
		}
	}
	if (who == black) return (1000 * (count_b - count_w) / (count_b + count_w + 2));
	else return -(1000 * (count_b - count_w) / (count_b + count_w + 2));
}

double Reversi::PotentialMobility(char who)
{
	double div_b = 0, div_w = 0, squ_b = 0, squ_w = 0, fro_b = 0, fro_w = 0;
	char temp[10][10], i = 0, j = 0;
	bool temp_b[10][10], temp_w[10][10];
	for (i = 0; i < 10; i++)    //���������ʼ��
	{
		for (j = 0; j < 10; j++)
		{
			temp_b[i][j] = 0;
			temp_w[i][j] = 0;
			temp[i][j] = board[i][j];
		}
	}
	for (i = 0; i < 10; i++)    //�߽��ʼ��
	{
		temp[0][i] = 9;
		temp[9][i] = 9;
		temp[i][0] = 9;
		temp[i][9] = 9;
	}
	//divergence & adjacent squares
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			if (temp[i][j] == black)
			{
				if (temp[i - 1][j - 1] == 0) { div_b++; temp_b[i - 1][j - 1] = 1; }
				if (temp[i - 1][j] == 0)     { div_b++; temp_b[i - 1][j] = 1; }
				if (temp[i - 1][j + 1] == 0) { div_b++; temp_b[i - 1][j + 1] = 1; }
				if (temp[i][j - 1] == 0)     { div_b++; temp_b[i][j - 1] = 1; }
				if (temp[i][j + 1] == 0)     { div_b++; temp_b[i][j + 1] = 1; }
				if (temp[i + 1][j - 1] == 0) { div_b++; temp_b[i + 1][j - 1] = 1; }
				if (temp[i + 1][j] == 0)     { div_b++; temp_b[i + 1][j] = 1; }
				if (temp[i + 1][j + 1] == 0) { div_b++; temp_b[i + 1][j + 1] = 1; }	
			}
			else if (temp[i][j] == white)
			{
				if (temp[i - 1][j - 1] == 0) { div_w++; temp_w[i - 1][j - 1] = 1; }
				if (temp[i - 1][j] == 0)     { div_w++; temp_w[i - 1][j] = 1; }
				if (temp[i - 1][j + 1] == 0) { div_w++; temp_w[i - 1][j + 1] = 1; }
				if (temp[i][j - 1] == 0)     { div_w++; temp_w[i][j - 1] = 1; }
				if (temp[i][j + 1] == 0)     { div_w++; temp_w[i][j + 1] = 1; }
				if (temp[i + 1][j - 1] == 0) { div_w++; temp_w[i + 1][j - 1] = 1; }
				if (temp[i + 1][j] == 0)     { div_w++; temp_w[i + 1][j] = 1; }
				if (temp[i + 1][j + 1] == 0) { div_w++; temp_w[i + 1][j + 1] = 1; }
			}
		}
	}
	//frontier discs��������ϵ�����
	for (i = 2; i <= 7; i++)
	{
		for (j = 2; j <= 7; j++)
		{
			if (temp[i][j] == black)
			{
				if (temp[i - 1][j] == 0 || temp[i + 1][j] == 0 || temp[i][j - 1] == 0 || temp[i][j + 1] == 0) fro_b++;
			}
			else if (temp[i][j] == white)
			{
				if (temp[i - 1][j] == 0 || temp[i + 1][j] == 0 || temp[i][j - 1] == 0 || temp[i][j + 1] == 0) fro_w++;
			}
		}
	}
	//��squ�ĸ���
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			if (temp_b[i][j] == 1) squ_b++;
			if (temp_w[i][j] == 1) squ_w++;
		}
	}
	if (who == black) return (1000 * (p_div * (div_w - div_b) / (div_w + div_b + 2) + p_squ * (squ_w - squ_b) / (squ_w + squ_b + 2) + p_fro * (fro_w - fro_b) / (fro_w + fro_b + 2)));
	else return -(1000 * (p_div * (div_w - div_b) / (div_w + div_b + 2) + p_squ * (squ_w - squ_b) / (squ_w + squ_b + 2) + p_fro * (fro_w - fro_b) / (fro_w + fro_b + 2)));
}

double Reversi::EdgeStableDiscs(char who)
{
	//��һ��������
	int i = 0, index = 0, count_b = 0, count_w = 0;
	index = 3 * (3 * (3 * (3 * (3 * (3 * (3 * (board[1][1] + 1) + board[1][2] + 1) + board[1][3] + 1) + board[1][4] + 1) + board[1][5] + 1) + board[1][6] + 1) + board[1][7] + 1) + board[1][8] + 1;
	count_b += stable_b[index];
	count_w += stable_w[index];
	index = 3 * (3 * (3 * (3 * (3 * (3 * (3 * (board[8][1] + 1) + board[8][2] + 1) + board[8][3] + 1) + board[8][4] + 1) + board[8][5] + 1) + board[8][6] + 1) + board[8][7] + 1) + board[8][8] + 1;
	count_b += stable_b[index];
	count_w += stable_w[index];
	index = 3 * (3 * (3 * (3 * (3 * (3 * (3 * (board[1][1] + 1) + board[2][1] + 1) + board[3][1] + 1) + board[4][1] + 1) + board[5][1] + 1) + board[6][1] + 1) + board[7][1] + 1) + board[8][1] + 1;
	count_b += stable_b[index];
	count_w += stable_w[index];
	index = 3 * (3 * (3 * (3 * (3 * (3 * (3 * (board[1][8] + 1) + board[2][8] + 1) + board[3][8] + 1) + board[4][8] + 1) + board[5][8] + 1) + board[6][8] + 1) + board[7][8] + 1) + board[8][8] + 1;
	count_b += stable_b[index];
	count_w += stable_w[index];
	if (who == black) return (1000 * (count_b - count_w) / (count_b + count_w + 2));
	else return -(1000 * (count_b - count_w) / (count_b + count_w + 2));
}

double Reversi::Penalty(char who)
{
	double res = 0;
	//�Է�ռ��
	if (board[1][1] == -who) res -= cor;
	if (board[8][1] == -who) res -= cor;
	if (board[1][8] == -who) res -= cor;
	if (board[8][8] == -who) res -= cor;
	//�Է��γ�00111100
	if (board[1][1] == 0 && board[1][2] == 0 && board[1][3] == -who && board[1][4] == -who
		&& board[1][5] == -who && board[1][6] == -who && board[1][7] == 0 && board[1][8] == 0) res -= bal;
	if (board[8][1] == 0 && board[8][2] == 0 && board[8][3] == -who && board[8][4] == -who
		&& board[8][5] == -who && board[8][6] == -who && board[8][7] == 0 && board[8][8] == 0) res -= bal;
	if (board[1][1] == 0 && board[2][1] == 0 && board[3][1] == -who && board[4][1] == -who
		&& board[5][1] == -who && board[6][1] == -who && board[7][1] == 0 && board[8][1] == 0) res -= bal;
	if (board[1][8] == 0 && board[2][8] == 0 && board[3][8] == -who && board[4][8] == -who
		&& board[5][8] == -who && board[6][8] == -who && board[7][8] == 0 && board[8][8] == 0) res -= bal;
	//�Է��γ�01111110
	if (board[1][1] == 0 && board[1][2] == -who && board[1][3] == -who && board[1][4] == -who
		&& board[1][5] == -who && board[1][6] == -who && board[1][7] == -who && board[1][8] == 0) res -= bal;
	if (board[8][1] == 0 && board[8][2] == -who && board[8][3] == -who && board[8][4] == -who
		&& board[8][5] == -who && board[8][6] == -who && board[8][7] == -who && board[8][8] == 0) res -= bal;
	if (board[1][1] == 0 && board[2][1] == -who && board[3][1] == -who && board[4][1] == -who
		&& board[5][1] == -who && board[6][1] == -who && board[7][1] == -who && board[8][1] == 0) res -= bal;
	if (board[1][8] == 0 && board[2][8] == -who && board[3][8] == -who && board[4][8] == -who
		&& board[5][8] == -who && board[6][8] == -who && board[7][8] == -who && board[8][8] == 0) res -= bal;
	return res;
}

void Reversi::change()
{
	if (count(0) <= 12) { stage = 9; dep = 60; }    //β��12��
	else { stage = 1; dep = diff; }    //�����ѶȾ����������
}

double Reversi::evaluate(char who)
{
	double res = 0;
	int n = (64 - count(0)) / 10;    //���
	if (count(who) == 0) return -999;    //��ֹ���Թ�
	if (stage == 1)    //���ֺ�����
	{
		if (wei[n][0] != 0) res += wei[n][0] * CurrentMobility(who);
		if (wei[n][1] != 0) res += wei[n][1] * PotentialMobility(who);
		if (wei[n][2] != 0) res += wei[n][2] * EdgeStableDiscs(who);
		if (wei[n][3] != 0) res += wei[n][3] * Penalty(who);
		return res;
	}
	else return difference(who);    //β��
}

double Reversi::alphabeta(int depth, double alpha, double beta, char next, char who)
{
	double val = 0;
	char way[40], temp[10][10], i = 0, num = 40;
	
	if (depth <= 0) return evaluate(who);    //��ȵ��˵ݹ����
	if (!haveplace(black) && !haveplace(white)) return evaluate(who);    //��Ϸ�����ݹ����
	
	for (i = 0; i < 40; i++) way[i] = 0;    //��ʼ��way
	copy(board, temp);    //��������	
	if (next == -1)    //min�ڵ�
	{
		generate_move(way, -who);
		for (i = 0; i < 40; i++)
		{
			if (way[i] != 0)
			{
				move(way[i], -who);    //����
				if (haveplace(who)) val = alphabeta(depth - 1, alpha, beta, 1, who);    //who���ߣ���һ����max�ڵ�
				else val = alphabeta(depth - 1, alpha, beta, -1, who);    //who��Ȩ����һ������min�ڵ�
				copy(temp, board);    //��ԭ
				if (val < beta) beta = val;    //beta��white�����߷��е���С��ֵ��beta��������
				if (beta <= alpha) break;    //���min�ڵ��betaС�ڵ����ȱ���alpha��ֹͣ�ýڵ����µ�����
			}
		}
		return beta;
	}
	else if (next == 1)   //max�ڵ�
	{
		generate_move(way, who);
		for (i = 0; i < 40; i++)
		{
			if (way[i] != 0)    //����num��
			{
				move(way[i], who);    //����
				if (haveplace(-who)) val = alphabeta(depth - 1, alpha, beta, -1, who);    //-who���ߣ���һ����min�ڵ�
				else val = alphabeta(depth - 1, alpha, beta, 1, who);    //-who��Ȩ����һ������max�ڵ�
				copy(temp, board);    //��ԭ
				if (val > alpha) alpha = val;    //alpha��black�����߷��е�����ֵ��alpha�����½�    
				if (alpha >= beta) break;    //���max�ڵ��alpha���ڵ����ȱ���beta��ֹͣ�ýڵ����µ�����
			}
		}
		return alpha;
	}
	return 0;
}

void Reversi::sort_dec(char inp[40], int dep, char next, char who)
{
	//��way����߷������ִӴ�С��
	double score[40], x = 0;
	char temp[10][10];
	char i = 0, j = 0, y = 0, num = 0, point = 0;
	copy(board, temp);    //����
	for (i = 0; i < 40; i++) score[i] = -inf;    //��ʼ��
	for (i = 0; i < 40; i++)    //inp��ĸ���
	{
		if (inp[i] != 0) num++;
		else break;
	}
	for (i = 0; i < num; i++)    //�õ�����
	{
		move(inp[i], who);    //���� 
		score[j++] = alphabeta(dep, -inf, inf, next, who);    //����
		copy(temp, board);    //��ԭ
	}
	for (i = 0; i < num; i++)
	{
		for (j = i; j < num; j++)
		{
			if (score[j] > score[i])    //�Ӵ�С��
			{
				x = score[j];
				score[j] = score[i];
				score[i] = x;
				y = inp[j];
				inp[j] = inp[i];
				inp[i] = y;
			}
		}
	}
}

char Reversi::get_best(char who)
{
	double score[40], x = 0, start = 0, end = 0;
	int loc = 0, r = 0;
	char temp[10][10], way[40], i = 0, j = 0, y = 0, num = 0;
	//��ʼ��
	for (i = 0; i < 40; i++) { way[i] = 0; score[i] = -inf; }      	
	copy(board, temp); j = 0;  
	generate_move(way, who);    //��������who���ߵĵط������Ǹ�max�ڵ�
	if (diff >= 6)
	{
		if (stage == 1)    
		{
			//Ԥ����
			if (haveplace(-who)) sort_dec(way, 4, -1, who);    //���-who���ߣ�min�ڵ�
			else sort_dec(way, 4, 1, who);    //���-who��Ȩ��max�ڵ�
			num = 6;
		}
		else num = 12;
	}
	else num = 40;
	for (i = 0; i < num; i++)    //����num����֮��һ����-who�ߣ���who����
	{
		if (way[i] != 0)       
		{
			move(way[i], who);    //����
			if (haveplace(-who)) score[j++] = alphabeta(dep, -inf, inf, -1, who);    //���-who���ߣ�min�ڵ�
			else score[j++] = alphabeta(dep, -inf, inf, 1, who);    //���-who��Ȩ��max�ڵ�
			copy(temp, board);    //��ԭ����
		}
	}
	if (count(black) + count(white) < 40)    //ǰ�ڷ���X���C��
	{
		for (i = 0; i < 40; i++)
		{
			if (board[1][1] == 0 && way[i] == 22) score[i] -= X;
			else if (board[1][8] == 0 && way[i] == 27) score[i] -= X;
			else if (board[8][1] == 0 && way[i] == 72) score[i] -= X;
			else if (board[8][8] == 0 && way[i] == 77) score[i] -= X;
			else if (board[1][1] == 0 && board[1][3] == 0 && way[i] == 12) score[i] -= C;
			else if (board[1][8] == 0 && board[1][6] == 0 && way[i] == 17) score[i] -= C;
			else if (board[8][1] == 0 && board[8][3] == 0 && way[i] == 82) score[i] -= C;
			else if (board[8][8] == 0 && board[8][6] == 0 && way[i] == 87) score[i] -= C;
			else if (board[1][1] == 0 && board[3][1] == 0 && way[i] == 21) score[i] -= C;
			else if (board[8][1] == 0 && board[6][1] == 0 && way[i] == 71) score[i] -= C;
			else if (board[1][8] == 0 && board[3][8] == 0 && way[i] == 28) score[i] -= C;
			else if (board[8][8] == 0 && board[6][8] == 0 && way[i] == 78) score[i] -= C;
		}
	}
	//way�ﰴ�����Ӵ�С�� 
	for (i = 0; i < 40; i++)    
	{
		for (j = i; j < 40; j++)
		{
			if (score[j] > score[i])
			{
				x = score[j];
				score[j] = score[i];
				score[i] = x;
				y = way[j];
				way[j] = way[i];
				way[i] = y;
			}
		}
	}
	/*
	for (i = 0; i < 40; i++)    //print for test
	{
		if (way[i] != 0)
		{
			printf("%d:", way[i]);
			printf("%lf\n", score[i]);
		}
	}
	*/
	return way[0];
}