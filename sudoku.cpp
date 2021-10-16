#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//开始生成数独棋盘
void startgame(){
	srand(unsigned(time(NULL)));
	for (int i = 1; i <= 9; ++i)
		for (int j = 1; j <= 9; ++j)
			sudokuterminal[i][j] = 0;
	int visited[10] = { 0 };
	for (int i = 1; i <= 8; i++) {
		while (sudokuterminal[1][i] == 0) {
			int data = rand() % 9 + 1;
			if (visited[data] == 0) {
				visited[data] = 1;
				sudokuterminal[1][i] = data;
			}
		}
	}
	for (int i = 9; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if (visited[j] == 0) {
				sudokuterminal[1][i] = j;
				visited[j] = 1;
				break;
			}
		}
	}
	createfinalsudoku(2, 1);  /* 从第二行开始搜索 */
}
//填充数独终盘
int createfinalsudoku(int i, int j){
	if (i > 9 || j > 9) {
		return true;
	}
	for (int k = 1; k <= 9; ++k){
		int type = true;                // type变量用于记录数字k能否放在 ( i , j ) 处
		if (type) {
			for (int n = 1; n < j; ++n) {
				if (sudokuterminal[i][n] == k){  // 检查同一行是否出现过数字k
					type = false;
					break;
				}
			}
		}
		for (int s = 1; s < i; ++s) {
			if (sudokuterminal[s][j] == k){  // 检查同一列是否出现过数字k
				type = false;
				break;
			}
		}
		if (type){
			int upi = ((i-1) / 3) * 3 + 3; // i坐标的上限
			int upj = ((j-1) / 3) * 3 + 3;   // j坐标的上限
			for (int p = upi - 2; p <= upi; ++p) { //检查在3×3的小方格中是否出现了同一个数字 
				if (type == false) {
					break;
				}
				for (int q = upj - 2; q <= upj; ++q) {
					if (sudokuterminal[p][q] == k) {
						type = false;
						break;
					}
				}
			}
		}
		if (type){//搜索剩余格子
			sudokuterminal[i][j] = k;
			if (j < 9){
				if (createfinalsudoku(i, j + 1)) //到同一行的下一位置开始搜索 
					return true;
			}
			else{
				if (i < 9){
					if (createfinalsudoku(i + 1, 1))   //下一行的第一个位置开始搜索
						return true;
				}
				else {
					return true;  //i >= 9  && j >= 9  , 搜索结束
				}
			}
			sudokuterminal[i][j] = 0;   //回溯
		}
	}
	return false;  
}
//将生成的数组转化为数独
void CopytoSudoku(Sudoku* s) {
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			s->s[i - 1][j - 1] = sudokuterminal[i][j];
		}
	}
	s->n_current = 81;
}
//利用挖洞发产生数独
Sudoku* createSudoku(Sudoku* s,int difficulty, int purpose){
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	k = s->s[x][y];
	s->s[x][y] = 0;//先挖掉1个洞
	--s->n_current;
	while (true) {//挖洞过程
		startsolveSudoku(s, &c);//求解该数独，得到解的个数c
		if (c > 1) {	//此时解不唯一
			s->s[x][y] = k;
			++s->n_current;
			flag++; //
			if (flag >= difficulty) {
				break;
			}
		}
		if (purpose >= s->n_current) {
			break;
		}
		do {
			x = rand() % 9;
			y = rand() % 9;
		} while (s->s[x][y] == 0);
		k = s->s[x][y]; //此时(x,y)不为0，记录下这个值
		s->s[x][y] = 0;
		--s->n_current;
	}
	return s;
}
//开始求解数独
int startsolveSudoku(Sudoku* s, int* count) {
	*count = 0;
	solveSudoku(s, 0, 0, count);
	if (count > 0) {
		return true;
	}
	else {
		return false;
	}
}
//求解数独
int solveSudoku(Sudoku* s, int x, int y, int* count) {
	int i, j, k;
	while (s->s[x][y] != 0 && x < 9){//从上往下找找到第一个为0的空
		y++;
		if (y == 9) {//搜索完一行
			x++;
			y = 0;
		}
	}
	if (x > 8) {//这时数独棋盘没有空的
		(*count)++;
		return true;
	}
	for (i = 1; i <= 9; i++){
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == true){
			if (x == 8 && y == 8) {//达到最后一个格子
				s->s[x][y] = 0;
				(*count)++;
				return true;
			}
			else{
				if (y == 8)  //求解下一行
					solveSudoku(s, x + 1, 0, count);
				else  //求解下一列
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return false;
}
//检验填入的数字是否正确
int checkkeyword(Sudoku* s, int x, int y) {
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) {//检查3*3九宫格是否冲突
		for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++) {
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y)) {
				return false;
			}
		}
	}
	for (int i = 0; i < 9; i++) {  //检查横行是否冲突
		if (s->s[x][y] == s->s[i][y] && i != x) {
			return false;
		}
	}
	for (int i = 0; i < 9; i++) {  //检查竖列是否冲突
		if (s->s[x][y] == s->s[x][i] && i != y) {
			return false;
		}
	}
	return true;
}
//三维数组寻址公式对应着9*9*9个CNF变元
inline int Location(int x, int y, int z) {
	return x * 81 + y * 9 + z;
}
//将数独转换为cnf文件
FILE* ToCNFFile(Sudoku* s, char* filename) {
	//11988
	//729
	FILE* fp = fopen(filename, "w+");
	if (fp == NULL) {
		
		return NULL;
	}
	fprintf(fp, "p cnf 729 11988\n");
	for (int i = 0; i < 9; i++) {//每个单元格只能填入唯一一个数字
		for (int j = 0; j < 9; j++) {
			if (s->s[i][j] == 0) {
				for (int k = 1; k <= 9; k++) {
					fprintf(fp, "%d ", Location(i, j, k));
				}
			}
			else {
				fprintf(fp, "%d ", Location(i, j, s->s[i][j]));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int i = 0; i < 9; i++) {//每个单元格只能填入唯一一个数字
		for (int j = 0; j < 9; j++) {
			for (int m = 1; m <= 9; m++) {
				for (int n = m + 1; n <= 9; n++) {
					fprintf(fp, "-%d -%d 0\n", Location(i, j, m), Location(i, j, n));
				}
			}
		}
	}
	for (int k = 1; k <= 9; k++) {//行约束
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				fprintf(fp, "%d ", Location(i, j, k));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int k = 1; k <= 9; k++) {
		for (int i = 0; i < 9; i++) {
			for (int m = 0; m < 9; m++) {
				for (int n = m + 1; n < 9; n++) {
					fprintf(fp, "-%d -%d 0\n", Location(i, m, k), Location(i, n, k));
				}
			}
		}
	}
	for (int k = 1; k <= 9; k++) {//列约束
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) {
				fprintf(fp, "%d ", Location(i,j,k));
			}
			fprintf(fp, "0\n");
		}
	}
	for (int k = 1; k <= 9; k++) {
		for (int j = 0; j < 9; j++) {
			for (int m = 0; m < 9; m++) {
				for (int n = m + 1; n < 9; n++) {
					fprintf(fp, "-%d -%d 0\n", Location(m, j, k), Location(n, j, k));
				}
			}
		}
	}
	for (int i = 0; i < 7; i = i + 3) {//九宫格约束
		for (int j = 0; j < 7; j = j + 3) {
			for (int k = 1; k <= 9; k++) {
				for (int pi = 0; pi < 3; pi++) {
					for (int pj = 0; pj < 3; pj++) {
						fprintf(fp, "%d ", Location(i + pi, j + pj, k));
					}
				}
				fprintf(fp, "0\n");
			}
		}
	}
	int respi[10] = { 0 };
	int respj[10] = { 0 };
	int countt = 0;
	for (int pi = 0; pi < 3; pi++) {
		for (int pj = 0; pj < 3; pj++) {
			respi[countt] = pi;
			respj[countt] = pj;
			countt++;
		}
	}
	for (int i = 0; i < 7; i = i + 3) {
		for (int j = 0; j < 7; j = j + 3) {
			for (int k = 1; k <= 9; k++) {
				//人两个位置，fprintf
				for (int m = 0; m < 9; m++) {
					for (int n = m + 1; n < 9; n++) {
						fprintf(fp, "-%d -%d 0\n", Location(i + respi[m], j + respj[m], k),
							Location(i + respi[n], j + respj[n], k)); 
					}
				}
			}
		}
	}
	return fp;
}
//将求解结果转化为数独
Sudoku* ResultToSudoku() {
	if (n_vars != 729) {
		return NULL;
	}
	Sudoku* S = (Sudoku*)malloc(sizeof(Sudoku));
	int countk = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int type = 0;
			for (int k = 1; k <= 9; k++) {
				if (result[++countk].value == TRUE) {
					if (type == 1) {
						printf("同一位置有两个数\n");
						return NULL;
					}
					else {
						type = 1;
						S->s[i][j] = k;
					}
				}
				if (result[countk].value == UNASSIGNED) {
					printf("有数字没有赋值\n");
				}
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		int visited[10] = { 0 };
		for (int j = 0; j < 9; j++) {
			if (visited[S->s[i][j]] == 0) {
				visited[S->s[i][j]] = 1;
			}
			else {
				printf("行冲突\n");
			}
		}
	}
	for (int j = 0; j < 9; j++) {
		int visited[10] = { 0 };
		for (int i = 0; i < 9; i++) {
			if (visited[S->s[i][j]] == 0) {
				visited[S->s[i][j]] = 1;
			}
			else {
				printf("列冲突\n");
			}
		}
	}
	for (int i = 0; i < 7; i = i + 3) {
		for (int j = 0; j < 7; j = j + 3) {
			int visited[10] = { 0 };
			for (int pi = 0; pi < 3; pi++) {
				for (int pj = 0; pj < 3; pj++) {
					if (visited[S->s[i + pi][j + pj]] == 0) {
						visited[S->s[i + pi][j + pj]] = 1;
					}
					else {
						printf("九宫格冲突\n");
						return NULL;
					}
				}
			}
		}
	}
	return S;
}
//游戏
void game(Sudoku* gameboard, Sudoku* ans) {
	int is_acc = FALSE;
	if (gameboard->n_current == 81) {
		is_acc = TRUE;
	}
	while (is_acc == FALSE) {
		system("cls");
		printSudoku(gameboard);
		printf("还有%d个数字等待填充\n", 81 - gameboard->n_current);
		printf("0 查看答案\n1 输入答案\n2 我要提示\n");
		int type;
		fflush(stdin);
		scanf("%d", &type);
		fflush(stdin);
		if (type == 0) {
			printf("答案即将公布\n");
			printSudoku(ans);
			is_acc = TRUE;
		}
		else if(type==1){ 
			int x, y, z;
			printf("请输入：行号 列号 数字(1~9)\n");
			fflush(stdin);
			scanf("%d%d%d", &x,&y,&z);
			if (1 <= x && x <= 9 && 1 <= y && y <= 9 && 1 <= z && z <= 9) {
				if (gameboard->s[x-1][y-1]) {
					printf("输入不合法，请重新输入\n");
				}
				else {
					if (z == ans->s[x - 1][y - 1]) {
						printf("恭喜你，这是正确的\n");
						gameboard->s[x - 1][y - 1] = z;
						gameboard->n_current++;
						if (gameboard->n_current == 81) {
							is_acc = TRUE;
							printf("你的计算准确无误，你的观察细致入微！\n");
						}
					}
					else {
						printf("很遗憾，这是错误的，再试一试吧\n");
					}
				}
			}
			else {
				printf("输入不合法，请重新输入\n");
			}
			fflush(stdin);
		}
		else if (type==2) {
			int x = rand() % 9;
			int y = rand() % 9;
			int i = x, j = y;
			do {
				if (gameboard->s[i][j] == 0) {
					break;
				}
				if (j == 8) {
					j = 0; i++;
				}
				else {
					j++;
				}
				if (i == 9) {
					i = 0;
				}
			} while (i != x || j != y);
			printf("第%d行第%d列为%d\n", i+1, j+1, ans->s[i][j]);
			gameboard->s[i][j] = ans->s[i][j];
			gameboard->n_current++;
			if (gameboard->n_current == 81) {
				is_acc = TRUE;
				printf("你的计算准确无误，你的观察细致入微！\n");
			}
		}
		else {
			printf("输入不合法\n");
		}
		system("pause");
		//0 for 答案
		//1 for 输入
		//2 for 提示
	}
}
//选择难度
int ChooseDifficluty() {
	printf("请选择已知数字的个数：\n");
	printf("24~80\n");
	int x;
	scanf("%d", &x);
	while (!(24 <= x && x <= 80)) {
		printf("非法输入，请重新输入\n");
		scanf("%d", &x);
		fflush(stdin);
	}
	return x;
}