#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//读取文件名
FILE* Inputfilename(char* filename) {
	printf("filename:   .cnf\n");
	scanf("%s", filename);
	FILE* fp = fopen(filename, "r");
	return fp;
}
//功能选择界面显示
int Display() {
	system("cls");
	printf("选择一个\n");
	printf("1 SAT\n");
	printf("2 Sudoku\n");
	printf("0 Exit\n");
	char c;
	c = getchar();
	while (!(c=='0'||c=='1'||c=='2')) {
		if (c != '\n') {
			printf("不合法的输入\n");
		}
		while ((c = getchar()) == ' ' || c == '\n' || c == '\t');
	}
	fflush(stdin);
	return c-'0';
}
//打印数独
void printSudoku(Sudoku* s){
	int i, j;
	//printf("此为生成的数独:\n");
	printf("-------------------\n");
	for (i = 0; i < 9; i++) {
		printf("|");
		for (j = 0; j < 9; j++) {
			printf("%d%c", s->s[i][j], ((j + 1) % 3 == 0) ? '|' : ' ');
		}
		printf("\n");
		if ((i + 1) % 3 == 0) {
			printf("|");
			printf("------------------\n");
		}
	}
}
//将可满足性问题的求解结果输出到屏幕
void PrintToScreen(int value,double time) {
	if (value == SAT) {
		printf("s 1\n");
		printf( "v");
		for (int i = 1; i <= n_vars; i++) {
			if (result[i].value == TRUE) {
				printf(" %d", i);
			}
			else {
				printf( " -%d", i);
			}
		}
		printf( "\n");
		printf("t %.2f %d\n", time, dpll_called);
		printf("该合取范式是可满足的\n");
	}
	else if (value == UNSAT) {
		printf("s 0\n");
		printf("t %.2f %d\n", time, dpll_called);
		printf("该合取范式是不可满足的\n");
	}
}
//是否展示合取范式
int Is_Show_CNF() {
	printf("是否展示合取范式Y/N\n");
	char c;
	while ((c = getchar()) == ' ' || c == '\n' || c == '\t');
	if (c == 'y' || c == 'Y') {
		printf("这个合取范式如下\n");
		return YES;
	}
	else return NO;
}