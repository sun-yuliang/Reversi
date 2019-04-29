/************************************************************************************************************************
                                                      Reversi v2.1                                                       
                                                    By Sun    2017.1                                                     
��飺
�����࣬�ܹ�����1000��
�ж���Ѷȿ�ѡ������Ѷ�Ϊdep=6��β������12��

�Ľ���
��д��ֵ�����������ж�����Ǳ���ж�����divergence + adjacent squares + frontier discs���������ȶ��ӣ�penalty
�޸�v2.0��һЩbug�������Ȩ���жϺ�Ԥ����

��ǣ�
��������2017�������һ��������ɵ���Ʒ������ǳ���ɬ��������д�����������������������һ��
����github֮ǰ���й��޸ģ���Щ�޸Ľ������ڳ���˵����������������������ط�����ȫά��ԭò
************************************************************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Reversi.h"
using namespace std;
#define white -1    //O
#define black 1    //X
#define sign 2    //`

/*ȫ�ֱ�����*/
Reversi game;
bool have_file = false;    //������
double p_div1 = 0.2;    //divergence
double p_squ1 = 0.3;    //adjacent squares  
double p_fro1 = 0.5;    //frontier discs
double wei1[7][4] = {
	{ 0.00, 1.00, 0.00, 1.00 },    //0-9
	{ 0.35, 0.65, 0.00, 1.00 },    //10-19
	{ 0.45, 0.55, 0.00, 1.00 },    //20-29
	{ 0.55, 0.45, 0.00, 1.00 },    //30-39
	{ 0.65, 0.35, 0.00, 1.00 },    //40-49
	{ 0.40, 0.00, 0.60, 0.00 },    //50-59
	{ 0.40, 0.00, 0.60, 0.00 },    //60-64
};  //mob   pot   sta   pen

int get_location()
{
	//��ȡ�û��������겢ת��Ϊ��λ���������*10+�У������벻�Է���0
	string str;
	stringstream s;
	int loc;
	char r, c;
	getline(cin, str);
	if (str != "" && str.length() <= 2)
	{
		s << str[0]; s >> c; s.clear();
		s << str[1]; s >> r; s.clear();
		if (c >= 'a' && c <= 'h' && r >= '1' && r <= '8')
		{
			loc = 10 * (r - '0') + c - 'a' + 1;
			return loc;
		}
		else
		{
			cout << "���������룺";
			return 0;
		}
	}
	else
	{
		cout << "���������룺";
		return 0;
	}
}

string loc2str(char loc)
{
	//e.g    35 -> e3
	int r = 0, c = 0;
	string str, temp;
	stringstream s;
	r = loc / 10;
	c = loc % 10;
	if (c == 1) str += "a";
	else if (c == 2) str += "b";
	else if (c == 3) str += "c";
	else if (c == 4) str += "d";
	else if (c == 5) str += "e";
	else if (c == 6) str += "f";
	else if (c == 7) str += "g";
	else if (c == 8) str += "h";
	s << r; s >> temp;
	str += temp;
	return str;
}

int ai_move(char who)
{
	char best = 0;
	best = game.get_best(who);
	if (best == 0)
	{
		system("cls");
		cout << "  ��ĸ��� " << game.count(black) << "           ";
		cout << "  ��ĸ��� " << game.count(white) << endl;
		cout << "��̫ǿ�ˣ�AI������Ȩһ�Σ�" << endl;
		game.get_valid(-who);
		game.display();
		game.clear_valid();
	}
	else
	{
		game.move(best, who);
		system("cls");
		cout << "  ��ĸ��� " << game.count(black) << "           ";
		cout << "  ��ĸ��� " << game.count(white) << endl;
		cout << "  AI�ߵ�λ�� " << loc2str(best) << endl;
		game.get_valid(-who);
		game.display();
		game.clear_valid();
	}
	game.change();
	return 0;
}

int player_move()
{
	int loc = 0;
	if (!game.haveplace(-game.ai))
	{
		cout << "��û�еط������ˣ�������Ȩһ���غϣ����س�����";
		getchar();
	}
	else
	{	
		cout << "��������Ҫ�µ�����(��d3)��";
	A:;
		loc = 0;
		while (!loc) loc = get_location();
		if (game.isvalid(loc, -game.ai)) game.move(loc, -game.ai);
		else
		{
			cout << "���������룺";
			goto A;
		}
		system("cls");
		cout << "  ��ĸ��� " << game.count(black) << "           ";
		cout << "  ��ĸ��� " << game.count(white) << endl << endl;
		game.get_valid(game.ai);
		game.display();
		game.clear_valid();
	}
	game.change();
	return 0;
}

int main()
{
	string mode;
	//ȫ�ֳ�ʼ��
	ifstream in_b("stable_b.txt");
	ifstream in_w("stable_w.txt");
	int x = 0;
	for (x = 0; x < 6561; x++)    //����edge stable table
	{
		game.stable_b[x] = in_b.get() - '0';
		game.stable_w[x] = in_w.get() - '0';
	}    
	if (game.stable_b[2] == 1 && game.stable_w[0] == 8) { cout << "Finish loading!" << endl << endl; have_file = true; }
	else { cout << "Loaded Incorrectly!" << endl << endl; have_file = false; }    //�жϲ���¼�ļ���ȡ
	cout << "�ڰ�����Ϸ" << endl << endl;
	cout << "�����к��е���ĸ������������" << endl;
	cout << "������d3��λ�������ӣ�����d3�����س�" << endl;
	cout << "��ʱAIҲ��Ҫ˼���������ĵȴ�����Ҫ���ü��� ( *^_^* )" << endl;
	while (1)
	{
		//ÿ�δ����￪ʼ
		game.initial();    //��ʼ��    
		game.get_wei(p_div1, p_squ1, p_fro1, wei1);    //��ȡ����   
		cout << endl << endl;
		cout << "ģʽ1�� ɵ��AI (����2��)   ���ִ��(����)" << endl;
		cout << "ģʽ2�� ɵ��AI (����2��)   ���ִ��(����)" << endl;
		cout << "ģʽ3�� ��AI (����4��)   ���ִ��(����)" << endl;
		cout << "ģʽ4�� ��AI (����4��)   ���ִ��(����)" << endl;
		cout << "ģʽ5�� ��ͨAI (����6��)   ���ִ��(����)" << endl;
		cout << "ģʽ6�� ��ͨAI (����6��)   ���ִ��(����)" << endl;
		cout << "ģʽ7�� Just for Fun!" << endl;
		cout << "�˳�8" << endl << endl;
		cout << "��ѡ��ģʽ(��������)��";
	START:;
		getline(cin, mode);
		if (mode == "1") { game.ai = white; game.diff = 2; }
		else if (mode == "2") { game.ai = black; game.diff = 2; }
		else if (mode == "3") { game.ai = white; game.diff = 4; }
		else if (mode == "4") { game.ai = black; game.diff = 4; }
		else if (mode == "5") { game.ai = white; game.diff = 6; }
		else if (mode == "6") { game.ai = black; game.diff = 6; }
		else if (mode == "7") { game.ai = white; game.diff = 6; game.board[1][1] = black; }
		else if (mode == "8") break;
		else { cout << "������ѡ��"; goto START; }
		system("cls");
		cout << "  ��ĸ��� " << game.count(black) << "           ";
		cout << "  ��ĸ��� " << game.count(white) << endl << endl;
		game.get_valid(black);
		game.display();
		game.clear_valid();
		game.change();
		while (1)
		{
			if (game.ai == black)
			{
				ai_move(game.ai);
				if (!game.haveplace(black) && !game.haveplace(white))    //�ж���Ϸ�Ƿ����
				{
					cout << "��Ϸ����  ��ǰΪģʽ" << mode << "  �밴�س�����  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
				player_move();
				if (!game.haveplace(black) && !game.haveplace(white))    //�ж���Ϸ�Ƿ����
				{
					cout << "��Ϸ����  ��ǰΪģʽ" << mode << "  �밴�س�����  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
			}
			else if (game.ai == white)
			{
				player_move();
				if (!game.haveplace(black) && !game.haveplace(white))    //�ж���Ϸ�Ƿ����
				{
					cout << "��Ϸ����  ��ǰΪģʽ" << mode << "  �밴�س�����  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
				ai_move(game.ai);
				if (!game.haveplace(black) && !game.haveplace(white))    //�ж���Ϸ�Ƿ����
				{
					cout << "��Ϸ����  ��ǰΪģʽ" << mode << "  �밴�س�����  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
			}
		}
	}
	return 0;
}