#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//��ʼ������������
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
	createfinalsudoku(2, 1);  /* �ӵڶ��п�ʼ���� */
}
//�����������
int createfinalsudoku(int i, int j){
	if (i > 9 || j > 9) {
		return true;
	}
	for (int k = 1; k <= 9; ++k){
		int type = true;                // type�������ڼ�¼����k�ܷ���� ( i , j ) ��
		if (type) {
			for (int n = 1; n < j; ++n) {
				if (sudokuterminal[i][n] == k){  // ���ͬһ���Ƿ���ֹ�����k
					type = false;
					break;
				}
			}
		}
		for (int s = 1; s < i; ++s) {
			if (sudokuterminal[s][j] == k){  // ���ͬһ���Ƿ���ֹ�����k
				type = false;
				break;
			}
		}
		if (type){
			int upi = ((i-1) / 3) * 3 + 3; // i���������
			int upj = ((j-1) / 3) * 3 + 3;   // j���������
			for (int p = upi - 2; p <= upi; ++p) { //�����3��3��С�������Ƿ������ͬһ������ 
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
		if (type){//����ʣ�����
			sudokuterminal[i][j] = k;
			if (j < 9){
				if (createfinalsudoku(i, j + 1)) //��ͬһ�е���һλ�ÿ�ʼ���� 
					return true;
			}
			else{
				if (i < 9){
					if (createfinalsudoku(i + 1, 1))   //��һ�еĵ�һ��λ�ÿ�ʼ����
						return true;
				}
				else {
					return true;  //i >= 9  && j >= 9  , ��������
				}
			}
			sudokuterminal[i][j] = 0;   //����
		}
	}
	return false;  
}
//�����ɵ�����ת��Ϊ����
void CopytoSudoku(Sudoku* s) {
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			s->s[i - 1][j - 1] = sudokuterminal[i][j];
		}
	}
	s->n_current = 81;
}
//�����ڶ�����������
Sudoku* createSudoku(Sudoku* s,int difficulty, int purpose){
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	k = s->s[x][y];
	s->s[x][y] = 0;//���ڵ�1����
	--s->n_current;
	while (true) {//�ڶ�����
		startsolveSudoku(s, &c);//�����������õ���ĸ���c
		if (c > 1) {	//��ʱ�ⲻΨһ
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
		k = s->s[x][y]; //��ʱ(x,y)��Ϊ0����¼�����ֵ
		s->s[x][y] = 0;
		--s->n_current;
	}
	return s;
}
//��ʼ�������
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
//�������
int solveSudoku(Sudoku* s, int x, int y, int* count) {
	int i, j, k;
	while (s->s[x][y] != 0 && x < 9){//�����������ҵ���һ��Ϊ0�Ŀ�
		y++;
		if (y == 9) {//������һ��
			x++;
			y = 0;
		}
	}
	if (x > 8) {//��ʱ��������û�пյ�
		(*count)++;
		return true;
	}
	for (i = 1; i <= 9; i++){
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == true){
			if (x == 8 && y == 8) {//�ﵽ���һ������
				s->s[x][y] = 0;
				(*count)++;
				return true;
			}
			else{
				if (y == 8)  //�����һ��
					solveSudoku(s, x + 1, 0, count);
				else  //�����һ��
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return false;
}
//��������������Ƿ���ȷ
int checkkeyword(Sudoku* s, int x, int y) {
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) {//���3*3�Ź����Ƿ��ͻ
		for (int j = y / 3 * 3; j < y / 3 * 3 + 3; j++) {
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y)) {
				return false;
			}
		}
	}
	for (int i = 0; i < 9; i++) {  //�������Ƿ��ͻ
		if (s->s[x][y] == s->s[i][y] && i != x) {
			return false;
		}
	}
	for (int i = 0; i < 9; i++) {  //��������Ƿ��ͻ
		if (s->s[x][y] == s->s[x][i] && i != y) {
			return false;
		}
	}
	return true;
}
//��ά����Ѱַ��ʽ��Ӧ��9*9*9��CNF��Ԫ
inline int Location(int x, int y, int z) {
	return x * 81 + y * 9 + z;
}
//������ת��Ϊcnf�ļ�
FILE* ToCNFFile(Sudoku* s, char* filename) {
	//11988
	//729
	FILE* fp = fopen(filename, "w+");
	if (fp == NULL) {
		
		return NULL;
	}
	fprintf(fp, "p cnf 729 11988\n");
	for (int i = 0; i < 9; i++) {//ÿ����Ԫ��ֻ������Ψһһ������
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
	for (int i = 0; i < 9; i++) {//ÿ����Ԫ��ֻ������Ψһһ������
		for (int j = 0; j < 9; j++) {
			for (int m = 1; m <= 9; m++) {
				for (int n = m + 1; n <= 9; n++) {
					fprintf(fp, "-%d -%d 0\n", Location(i, j, m), Location(i, j, n));
				}
			}
		}
	}
	for (int k = 1; k <= 9; k++) {//��Լ��
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
	for (int k = 1; k <= 9; k++) {//��Լ��
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
	for (int i = 0; i < 7; i = i + 3) {//�Ź���Լ��
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
				//������λ�ã�fprintf
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
//�������ת��Ϊ����
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
						printf("ͬһλ����������\n");
						return NULL;
					}
					else {
						type = 1;
						S->s[i][j] = k;
					}
				}
				if (result[countk].value == UNASSIGNED) {
					printf("������û�и�ֵ\n");
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
				printf("�г�ͻ\n");
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
				printf("�г�ͻ\n");
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
						printf("�Ź����ͻ\n");
						return NULL;
					}
				}
			}
		}
	}
	return S;
}
//��Ϸ
void game(Sudoku* gameboard, Sudoku* ans) {
	int is_acc = FALSE;
	if (gameboard->n_current == 81) {
		is_acc = TRUE;
	}
	while (is_acc == FALSE) {
		system("cls");
		printSudoku(gameboard);
		printf("����%d�����ֵȴ����\n", 81 - gameboard->n_current);
		printf("0 �鿴��\n1 �����\n2 ��Ҫ��ʾ\n");
		int type;
		fflush(stdin);
		scanf("%d", &type);
		fflush(stdin);
		if (type == 0) {
			printf("�𰸼�������\n");
			printSudoku(ans);
			is_acc = TRUE;
		}
		else if(type==1){ 
			int x, y, z;
			printf("�����룺�к� �к� ����(1~9)\n");
			fflush(stdin);
			scanf("%d%d%d", &x,&y,&z);
			if (1 <= x && x <= 9 && 1 <= y && y <= 9 && 1 <= z && z <= 9) {
				if (gameboard->s[x-1][y-1]) {
					printf("���벻�Ϸ�������������\n");
				}
				else {
					if (z == ans->s[x - 1][y - 1]) {
						printf("��ϲ�㣬������ȷ��\n");
						gameboard->s[x - 1][y - 1] = z;
						gameboard->n_current++;
						if (gameboard->n_current == 81) {
							is_acc = TRUE;
							printf("��ļ���׼ȷ������Ĺ۲�ϸ����΢��\n");
						}
					}
					else {
						printf("���ź������Ǵ���ģ�����һ�԰�\n");
					}
				}
			}
			else {
				printf("���벻�Ϸ�������������\n");
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
			printf("��%d�е�%d��Ϊ%d\n", i+1, j+1, ans->s[i][j]);
			gameboard->s[i][j] = ans->s[i][j];
			gameboard->n_current++;
			if (gameboard->n_current == 81) {
				is_acc = TRUE;
				printf("��ļ���׼ȷ������Ĺ۲�ϸ����΢��\n");
			}
		}
		else {
			printf("���벻�Ϸ�\n");
		}
		system("pause");
		//0 for ��
		//1 for ����
		//2 for ��ʾ
	}
}
//ѡ���Ѷ�
int ChooseDifficluty() {
	printf("��ѡ����֪���ֵĸ�����\n");
	printf("24~80\n");
	int x;
	scanf("%d", &x);
	while (!(24 <= x && x <= 80)) {
		printf("�Ƿ����룬����������\n");
		scanf("%d", &x);
		fflush(stdin);
	}
	return x;
}