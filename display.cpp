#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//��ȡ�ļ���
FILE* Inputfilename(char* filename) {
	printf("filename:   .cnf\n");
	scanf("%s", filename);
	FILE* fp = fopen(filename, "r");
	return fp;
}
//����ѡ�������ʾ
int Display() {
	system("cls");
	printf("ѡ��һ��\n");
	printf("1 SAT\n");
	printf("2 Sudoku\n");
	printf("0 Exit\n");
	char c;
	c = getchar();
	while (!(c=='0'||c=='1'||c=='2')) {
		if (c != '\n') {
			printf("���Ϸ�������\n");
		}
		while ((c = getchar()) == ' ' || c == '\n' || c == '\t');
	}
	fflush(stdin);
	return c-'0';
}
//��ӡ����
void printSudoku(Sudoku* s){
	int i, j;
	//printf("��Ϊ���ɵ�����:\n");
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
//���������������������������Ļ
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
		printf("�ú�ȡ��ʽ�ǿ������\n");
	}
	else if (value == UNSAT) {
		printf("s 0\n");
		printf("t %.2f %d\n", time, dpll_called);
		printf("�ú�ȡ��ʽ�ǲ��������\n");
	}
}
//�Ƿ�չʾ��ȡ��ʽ
int Is_Show_CNF() {
	printf("�Ƿ�չʾ��ȡ��ʽY/N\n");
	char c;
	while ((c = getchar()) == ' ' || c == '\n' || c == '\t');
	if (c == 'y' || c == 'Y') {
		printf("�����ȡ��ʽ����\n");
		return YES;
	}
	else return NO;
}