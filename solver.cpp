#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//Ԥ���������Ӿ����ȫ�ֱ���
void Preprocesser() {
	for (int i = 0; i < original_formula_length; i++) {
		if (clauses[i].length_original == 1) {//������Ϊһ�ı�������ȫ��ջ
			unit_clause_stack[n_unit_clause_stack] = clauses[i].literals[0];
			++n_unit_clause_stack;
		}
	}
}
//�����Ӿ�
void Value(int v) {
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];
		if (clauses[j].is_clause_satisfied) continue;//��j��Ϊ�����
		clauses[j].is_clause_satisfied = YES;//����Ϊ����󣬲��ı���ӳ��ȣ�ֱ�����ݺ�
		--current_formula_length;//�����Ӿ�ļ�����
		changes_stack[changes_stack_index++].index_of_clause = j;//һ���Ӿ䱻����Ϊ���㣬��ô��¼����Ӿ�����
		n_changes[depth][SATISFIED]++;//depth��++
	}
	q = !q;
	for (i = 0; i < literal_info[p][q].n_number; ++i) {
		register int j = literal_info[p][q].literal_clause[i];//��¼�־����
		if (clauses[j].is_clause_satisfied) continue;
		register int k = literal_info[p][q].literal_clause_pos[i];//��¼����λ��
		--clauses[j].length_current;//�ı��־䳤��
		clauses[j].literals_is_assigned[k] = ASSIGNED;//�ñ�ʾ�Ӿ��и��������Ƿ񱻸�ֵ
		changes_stack[changes_stack_index].index_of_clause = j;//��¼�ı�
		changes_stack[changes_stack_index++].index_of_literal = k;//
		n_changes[depth][SHRUNK]++;
		if (clauses[j].length_current == 1) {//����־䳤�ȣ��ж��Ƿ�Ϊ���Ӿ�
			register int location = -1;
			for (int i_literal = 0; i_literal < clauses[j].length_original; i_literal++) {
				if (clauses[j].literals_is_assigned[i_literal] == UNASSIGNED) {
					location = i_literal;
					break;
				}
			}
			register int w = clauses[j].literals[location];//���Ӿ���ʣ�������
			register int s = abs(w), t = (w > 0) ? SATISFIED : SHRUNK;
			if (literal_info[s][(!t)].is_in_unit_clause == YES) {//��¼��ͻ
				is_contradicted = TRUE;
				conflicting_literal = w;
			}
			else if (literal_info[s][t].is_in_unit_clause == NO) {//û�г�ͻ,��������Ӿ����ȫ�ֱ���
				unit_clause_stack[n_unit_clause_stack] = clauses[j].unit_cluase_literal = w;
				literal_info[s][t].is_in_unit_clause = YES;
				++n_unit_clause_stack;
			}
		}
	}
	literal_info[p][SHRUNK].is_assigned = YES;
	literal_info[p][SATISFIED].is_assigned = YES;
	++depth;
}
//ȡ������
void UnValue(int v){
	register int i;
	register int p = abs(v), q = (v > 0) ? SATISFIED : SHRUNK;
	--depth;
	while (n_changes[depth][SHRUNK]){//����û��stack��ȡ������
		--n_changes[depth][SHRUNK];//
		register int j = changes_stack[--changes_stack_index].index_of_clause;
		register int k = changes_stack[changes_stack_index].index_of_literal;
		++clauses[j].length_current;//�ظ��Ӿ�ĳ���
		if (clauses[j].length_current == 2) {//��������Ϊ�ڵ��Ӿ��г��ֵ�������Ϊδ����
			int s = abs(clauses[j].unit_cluase_literal);
			int t = (clauses[j].unit_cluase_literal > 0) ? SATISFIED : SHRUNK;
			literal_info[s][t].is_in_unit_clause = NO;
			clauses[j].unit_cluase_literal = 0;
		}
		clauses[j].literals_is_assigned[k] = UNASSIGNED;////
	}
	while (n_changes[depth][SATISFIED])	{
		--n_changes[depth][SATISFIED];
		register int j = changes_stack[--changes_stack_index].index_of_clause;
		clauses[j].is_clause_satisfied = NO;
		++current_formula_length;
	}
	literal_info[p][SATISFIED].is_assigned = NO;
	literal_info[p][SHRUNK].is_assigned = NO;
}
//DPLL�ݹ��㷨
int dpll() {
	++dpll_called;//�������������Ϣ
	int* local_stack = NULL;
	register unsigned int n_local_stack = 0;
	while (true) {
		if (is_contradicted) {//return
			int cl = abs(conflicting_literal);
			while (n_local_stack) {
				UnValue(local_stack[--n_local_stack]);
				register int s = abs(local_stack[n_local_stack]);
				register int t = local_stack[n_local_stack] > 0 ? TRUE : FALSE;
				result[s].value = UNASSIGNED;
			}
			is_contradicted = FALSE;
			free(local_stack);
			n_unit_clause_stack = 0;
			return UNSAT;
		}
		else if (n_unit_clause_stack) {//
			local_stack = (int*)realloc(local_stack, (n_local_stack + 1) * sizeof(int));
			register int implied_lit = unit_clause_stack[--n_unit_clause_stack];//��Ҫ���е��Ӿ���������
			local_stack[n_local_stack++] = implied_lit;
			result[abs(implied_lit)].value = implied_lit > 0 ? TRUE : FALSE;
			Value(implied_lit);
		}
		else break;
	}
	if (!current_formula_length) return SAT;//�����ǰ�Ӿ���Ϊ0,�򷵻�SAT
	register int v = GetMinLenOfLiteral();//��Ԫѡȡ����
	result[abs(v)].value = v > 0 ? TRUE : FALSE;
	Value(v);
	if (dpll()) return SAT;
	UnValue(v);
	register int i, j, k, m ;
	result[abs(v)].value = !result[abs(v)].value;
	Value(-v);
	if (dpll()) return SAT;
	UnValue(-v);
	result[abs(v)].value = UNASSIGNED;
	while (n_local_stack) {
		int z = local_stack[--n_local_stack];
		UnValue(z);
		result[abs(z)].value = UNASSIGNED;
	}
	free(local_stack);
	is_contradicted = FALSE;
	return UNSAT;
}
inline int get_length_of() {
	register int i, s,type, C, min = max_clause_len;
	if (min == 2) return min;//�����Ӿ�ĳ���Ϊ2����ôminͬ��Ϊ
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {
			for (type = 0; type < 2; ++type) {
				for (s = 0; s < literal_info[i][type].n_number; ++s) {
					C = literal_info[i][type].literal_clause[s];//ÿһ�����֣���ÿһ�������е�λ��
					if (!clauses[C].is_clause_satisfied &&
						clauses[C].length_current < min) {
						min = clauses[C].length_current;
						if (min == 2) return 2;
					}
				}
			}
		}
	}
	return min;
}
void get_weight(int x, int k, unsigned int& s, unsigned int& t) {
	register int j, c;
	s = t = 0;
	for (j = 0; j < literal_info[x][SATISFIED].n_number; ++j) {
		c = literal_info[x][SATISFIED].literal_clause[j];//�Ӿ����
		if (clauses[c].length_current == k)
			s += 1 - clauses[c].is_clause_satisfied;
	}
	for (j = 0; j < literal_info[x][SHRUNK].n_number; ++j) {// �Ӿ����
		c = literal_info[x][SHRUNK].literal_clause[j];
		if (clauses[c].length_current == k)
			t += 1 - clauses[c].is_clause_satisfied;
	}
}
inline int GetMinLenOfLiteral() {
	register unsigned int i, k;
	register unsigned int max = 0, r, s, t;
	register int u;
	for (i = 1; i <= n_vars; ++i) {
		if (result[i].value == UNASSIGNED) {
			k = get_length_of();//����Ӿ�
			get_weight(i, k, s, t);//i ��Ԫ k  ����
			r = (s + 1) * (t + 1);
			if (r > max) {
				max = r;
				if (s >= t) u = i;
				else u = -(int)i;
			}
		}
	}
	return u;
}
//������������res�ļ�
void PrintToRes(int value, double time, char* filename) {
	int len = strlen(filename);
	filename[len - 3] = 'r';
	filename[len - 2] = 'e';
	filename[len - 1] = 's';
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("���ļ�%sʧ��\n", filename);
		return;
	}
	if (value == SAT) {
		fprintf(fp, "s 1\n");
		fprintf(fp, "v");
		for (int i = 1; i <= n_vars; i++) {
			if (result[i].value == TRUE) {
				fprintf(fp, " %d", i);
			}
			else {
				fprintf(fp, " -%d", i);
			}
		}
		fprintf(fp, "\n");
		fprintf(fp, "t %.2f %d\n", time, dpll_called);
	}
	else if (value == UNSAT) {
		fprintf(fp, "s 0\n");
		fprintf(fp, "t %.2f %d\n", time, dpll_called);
	}
	fclose(fp);
}
//�ͷŷ�����ڴ�
void Free() {
	for (int i = 1; i <= n_vars; i++) {
		if (literal_info[i][0].literal_clause) {
			free(literal_info[i][0].literal_clause);
			literal_info[i][0].literal_clause = NULL;
		}
		if (literal_info[i][0].literal_clause_pos) {
			free(literal_info[i][0].literal_clause_pos);
			literal_info[i][0].literal_clause_pos = NULL;
		}
		if (literal_info[i][1].literal_clause) {
			free(literal_info[i][0].literal_clause);
			literal_info[i][0].literal_clause_pos = NULL;
		}
		if (literal_info[i][1].literal_clause_pos) {
			free(literal_info[i][0].literal_clause_pos);
			literal_info[i][0].literal_clause_pos = NULL;
		}
	}
	for (int i = 0; i < original_formula_length; i++) {
		if (clauses[i].literals) {
			free(clauses[i].literals);
		}
		if (clauses[i].literals_is_assigned) {
			free(clauses[i].literals_is_assigned);
		}
	}
	free(clauses);
}