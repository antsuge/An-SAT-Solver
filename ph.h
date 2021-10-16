#ifndef PH_H
#define PH_H
#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED
#include<stdio.h>
//ȫ�ֱ���
#define MAX_VARS 60000 //����Ԫ��
#define MAX_CLAUSES 230000 //����Ӿ���
#define ASSIGNED -2// ��ʱ�ĸ�ֵ
#define UNASSIGNED -1//��û�����ĸ�ֵ
#define TRUE 1 
#define FALSE 0 
#define YES 1
#define NO 0
#define SATISFIED 1
#define SHRUNK 0
#define SAT 1
#define UNSAT 0
typedef int status;
typedef struct literal_infomation {//���ֽṹ
	int* literal_clause;//�����ڹ�ʽ�е�λ��
	int* literal_clause_pos;//�������Ӿ��е�λ��
	int n_number;//����
	int is_assigned;//�����Ƿ񱻸�ֵ
	int is_in_unit_clause;//��¼�Ƿ��ڵ��Ӿ��г��� NO or YES
}literal_infomation;
extern literal_infomation literal_info[MAX_VARS][2];//���ִ洢����
typedef struct clause_infomation {//�Ӿ�ṹ
	int* literals;//�Ӿ��е�����
	int* literals_is_assigned;//1 for unassigned 0 for false
	int length_current;//���ڵ��־䳤��
	int length_original;//����Ӿ䳤��
	int is_clause_satisfied;//�Ƿ����� NO OR YES true 
	int unit_cluase_literal;//�洢���Ӿ������У�δ��ֵ����
}clause_infomation;
extern clause_infomation* clauses;//�Ӿ�洢����
extern int original_formula_length;//ԭ����
extern int current_formula_length;//�ֳ���
typedef struct changes_infomation {//������Ϣ
	int index_of_clause; //�Ӿ����
	int index_of_literal;//�������
}changes_infomation;//�Ӿ�����ʱ����¼�Ӿ���š����ֱ���Ϊ��ʱ����¼���ֺ��־����
extern changes_infomation changes_stack[8*MAX_CLAUSES];//��¼
extern unsigned int changes_stack_index;//����changes���±�
extern unsigned int n_changes[MAX_VARS][2];//ĳһ�����������ľ��ӵĸ���
typedef struct assign_infomation {//��¼��ֵ����
	int value; //��ֵ����UNASSIGNED TRUE FALSE
}assign_infomation;
extern assign_infomation result[MAX_VARS];//���ڻ��ݲ����ļ�¼
extern int is_contradicted;//�Ƿ��ó�ͻ�ĵ��Ӿ�
extern int conflicting_literal;//���ڼ�¼��ͻ������
extern int unit_clause_stack[MAX_CLAUSES];//���Ӿ����
extern int n_unit_clause_stack;//ȫ��ջ�����ڼ�¼ջ��λ��
extern int depth;//DPLL���У��������
extern int n_vars;//��Ԫ�ܸ���
extern int dpll_called;//ͳ��DPLLִ�д���
extern int sudokuterminal[10][10];//������������
extern int max_clause_len;//����־䳤��
typedef struct Sudoku {//�����ṹ
	int s[9][9];
	int n_current;
}Sudoku;
//��ʾ���ֵĺ�����
int Display();//չʾѡ��
FILE* Inputfilename(char* filename); //�����ļ���
int Is_Show_CNF();//�Ƿ�չʾcnf�ļ�
void PrintToScreen(int value, double time);//������������Ļ
void printSudoku(Sudoku* s);//��ӡ����
//CNFparser
int CNFparser(FILE* fp);//��ȡ��ʽ
void CheckCNF();//��������ȡ��ʽ
//solver part
void Preprocesser();//Ԥ������
int dpll();//dpll�㷨
void Value(int v);//������v�趨Ϊ�Ѹ�ֵ
void UnValue(int v);//�������趨Ϊδ��ֵ
inline int GetMinLenOfLiteral();//
inline int get_length_of();//��������־䳤��
void get_weight(int x, int k, unsigned int& s, unsigned int& t);//���Ȩ��
void PrintToRes(int value, double time, char* filename);//����������.res�ļ�
void Free();//�ͷŶ�̬����Ŀռ�
//�������ֵĺ�����
void startgame();//��������
void CopytoSudoku(Sudoku* s);//�����鸴�Ƶ�����
int createfinalsudoku(int i, int j);//��������
Sudoku* createSudoku(Sudoku* S, int difficulty, int flag);//������������
int startsolveSudoku(Sudoku* s, int* count);//��ʼ�������
int solveSudoku(Sudoku* s, int x, int y, int* count);//��������ĵݹ����
int checkkeyword(Sudoku* s, int x, int y);//���x��y���ϵ�����
inline int Location(int x, int y, int z);//��άѰַ
FILE* ToCNFFile(Sudoku* s, char* filename);//����������ΪCNF�ļ�
Sudoku* ResultToSudoku();//��������������resתΪ���Ƚ��
void game(Sudoku* s, Sudoku* final_patern);//��ʼ������Ϸ
int ChooseDifficluty();//ѡ���Ѷ�
#endif // HEAD_H_INCLUDED
#endif //PCH_H