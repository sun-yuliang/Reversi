/************************************************************************************************************************
                                                      Reversi v2.1                                                       
                                                    By Sun    2017.1                                                     
简介：
代码简洁，总共不到1000行
有多个难度可选，最高难度为dep=6，尾局完美12步

改进：
改写估值函数，考虑行动力，潜在行动力（divergence + adjacent squares + frontier discs），边上稳定子，penalty
修复v2.0的一些bug，如对弃权的判断和预搜索

后记：
这是我在2017年年初大一寒假中完成的作品，代码非常青涩，两年多后写完五子棋程序想起来整理了一下
传到github之前进行过修改，这些修改仅存在于程序说明和棋盘输出函数，其他地方都完全维持原貌
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

/*全局变量区*/
Reversi game;
bool have_file = false;    //防作弊
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
	//读取用户输入坐标并转换为两位数输出（行*10+列），输入不对返回0
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
			cout << "请重新输入：";
			return 0;
		}
	}
	else
	{
		cout << "请重新输入：";
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
		cout << "  ●的个数 " << game.count(black) << "           ";
		cout << "  ○的个数 " << game.count(white) << endl;
		cout << "你太强了，AI必须弃权一次！" << endl;
		game.get_valid(-who);
		game.display();
		game.clear_valid();
	}
	else
	{
		game.move(best, who);
		system("cls");
		cout << "  ●的个数 " << game.count(black) << "           ";
		cout << "  ○的个数 " << game.count(white) << endl;
		cout << "  AI走的位置 " << loc2str(best) << endl;
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
		cout << "你没有地方能下了，必须弃权一个回合，按回车继续";
		getchar();
	}
	else
	{	
		cout << "请输入你要下的坐标(如d3)：";
	A:;
		loc = 0;
		while (!loc) loc = get_location();
		if (game.isvalid(loc, -game.ai)) game.move(loc, -game.ai);
		else
		{
			cout << "请重新输入：";
			goto A;
		}
		system("cls");
		cout << "  ●的个数 " << game.count(black) << "           ";
		cout << "  ○的个数 " << game.count(white) << endl << endl;
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
	//全局初始化
	ifstream in_b("stable_b.txt");
	ifstream in_w("stable_w.txt");
	int x = 0;
	for (x = 0; x < 6561; x++)    //导入edge stable table
	{
		game.stable_b[x] = in_b.get() - '0';
		game.stable_w[x] = in_w.get() - '0';
	}    
	if (game.stable_b[2] == 1 && game.stable_w[0] == 8) { cout << "Finish loading!" << endl << endl; have_file = true; }
	else { cout << "Loaded Incorrectly!" << endl << endl; have_file = false; }    //判断并记录文件读取
	cout << "黑白棋游戏" << endl << endl;
	cout << "输入行和列的字母和数字来落子" << endl;
	cout << "比如在d3的位置上落子，请输d3并按回车" << endl;
	cout << "有时AI也需要思考，请耐心等待，不要乱敲键盘 ( *^_^* )" << endl;
	while (1)
	{
		//每次从这里开始
		game.initial();    //初始化    
		game.get_wei(p_div1, p_squ1, p_fro1, wei1);    //获取参数   
		cout << endl << endl;
		cout << "模式1： 傻逼AI (搜索2步)   玩家执●(先手)" << endl;
		cout << "模式2： 傻逼AI (搜索2步)   玩家执○(后手)" << endl;
		cout << "模式3： 简单AI (搜索4步)   玩家执●(先手)" << endl;
		cout << "模式4： 简单AI (搜索4步)   玩家执○(后手)" << endl;
		cout << "模式5： 普通AI (搜索6步)   玩家执●(先手)" << endl;
		cout << "模式6： 普通AI (搜索6步)   玩家执○(后手)" << endl;
		cout << "模式7： Just for Fun!" << endl;
		cout << "退出8" << endl << endl;
		cout << "请选择模式(输入数字)：";
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
		else { cout << "请重新选择："; goto START; }
		system("cls");
		cout << "  ●的个数 " << game.count(black) << "           ";
		cout << "  ○的个数 " << game.count(white) << endl << endl;
		game.get_valid(black);
		game.display();
		game.clear_valid();
		game.change();
		while (1)
		{
			if (game.ai == black)
			{
				ai_move(game.ai);
				if (!game.haveplace(black) && !game.haveplace(white))    //判断游戏是否结束
				{
					cout << "游戏结束  当前为模式" << mode << "  请按回车继续  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
				player_move();
				if (!game.haveplace(black) && !game.haveplace(white))    //判断游戏是否结束
				{
					cout << "游戏结束  当前为模式" << mode << "  请按回车继续  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
			}
			else if (game.ai == white)
			{
				player_move();
				if (!game.haveplace(black) && !game.haveplace(white))    //判断游戏是否结束
				{
					cout << "游戏结束  当前为模式" << mode << "  请按回车继续  ";
					if (have_file == false) cout << "Without table!";
					getchar(); getchar();
					system("cls");
					break;
				}
				ai_move(game.ai);
				if (!game.haveplace(black) && !game.haveplace(white))    //判断游戏是否结束
				{
					cout << "游戏结束  当前为模式" << mode << "  请按回车继续  ";
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