#include "ph.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
int dpll_called;//统计DPLL调用次数的全局变量
int sudokuterminal[10][10];//数独棋盘
literal_infomation literal_info[MAX_VARS][2];//文字存储数组
clause_infomation* clauses;//子句存储数组++++
int original_formula_length, current_formula_length;//原长度，现长度
changes_infomation changes_stack[8*MAX_CLAUSES];//记录++++++
unsigned int changes_stack_index;//数组changes的下标
unsigned int n_changes[MAX_VARS][2];//某一层满足或不满足的句子的个数
assign_infomation result[MAX_VARS];//用于回溯操作的记录
int is_contradicted;//是否用冲突的单子句
int conflicting_literal;//用于记录冲突的文字
int unit_clause_stack[MAX_CLAUSES];//单子句规则++++++
int n_unit_clause_stack;//全局栈，用于记录栈顶位置
int depth;//DPLL树中，结点的深度
int n_vars;//变元总个数
int max_clause_len;//最长单子句
double t;
int main() {
	int type = 3;
	while (type) {
		if (type != 3) system("pause");
		type = Display();
		if (type == 1) {
			char filename[100];//文件名
			FILE* fp = Inputfilename(filename);//输入文件名
			if (fp == NULL) {
				printf("文件打开失败\n");
				continue;
			}
			int is_cnf = CNFparser(fp);//创建新的内部结构
			if (is_cnf == NO) {
				printf("文件格式不正确\n");
				continue;
			}
			if (Is_Show_CNF()) {
				CheckCNF();//解析公式
			}
			clock_t starttime, endtime;
			printf("开始求解\n");
			starttime = clock();
			Preprocesser();//预处理
			int value =dpll();
			endtime = clock();
			t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;
			PrintToRes(value,t,filename);
			PrintToScreen(value,t);
			Free();
		}
		else if (type == 2) {
			Sudoku* s = (Sudoku*)malloc(sizeof(Sudoku));
			Sudoku* FinalSudoku = NULL;
			char filename[100] = "Sudoku.cnf";
			startgame();
			CopytoSudoku(s);
			int flag = ChooseDifficluty();
			s = createSudoku(s,100,flag);//挖洞法生成数独//第二个数是难度
			printf("数独生成成功\n");
			FILE* fp = ToCNFFile(s,filename);
			if (fp == NULL) {
				printf("文件打开失败\n");
				continue;
			}
			fclose(fp);
			fp = fopen(filename, "r");
			if (fp == NULL) {
				printf("文件打开失败\n");
				continue;
			}
			CNFparser(fp);//创建新的内部结构
			clock_t starttime, endtime;
			starttime = clock();
			Preprocesser();//预处理
			int value = dpll();
			endtime = clock();
			t = (double)(endtime - starttime) * 1000 / CLOCKS_PER_SEC;
			PrintToRes(value, t, filename);
			if (value == SAT) {//解出数独
				FinalSudoku = ResultToSudoku();
				game(s,FinalSudoku);//
				free(s);
				free(FinalSudoku);
			}
			else {//没有接触数独
				printf("数独没有解\n");
				free(s);
			}
			Free();
		}
	}
	return 0;
}