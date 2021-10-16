#include "ph.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int CNFparser(FILE* fp) {
	//重置全局变量
	dpll_called = 0;
	max_clause_len = 0;
	for (int i = 0; i < MAX_VARS; i++) {
		literal_info[i][SATISFIED].is_assigned = UNASSIGNED;
		literal_info[i][SATISFIED].is_in_unit_clause = NO;
		literal_info[i][SATISFIED].literal_clause = NULL;
		literal_info[i][SATISFIED].literal_clause_pos = NULL;
		literal_info[i][SATISFIED].n_number = 0;
		literal_info[i][SHRUNK].is_assigned = UNASSIGNED;
		literal_info[i][SHRUNK].is_in_unit_clause = NO;
		literal_info[i][SHRUNK].literal_clause = NULL;
		literal_info[i][SHRUNK].literal_clause_pos = NULL;
		literal_info[i][SHRUNK].n_number = 0;
	}
	for (int i = 0; i < 8 * MAX_CLAUSES; i++) {
		changes_stack[i].index_of_clause = 0;
		changes_stack[i].index_of_literal = 0;
	}
	for (int i = 0; i < MAX_VARS; i++) {
		n_changes[i][0] = 0;
		n_changes[i][1] = 0;
	}
	changes_stack_index = 0;
	for (int i = 0; i < MAX_VARS; i++) {
		result[i].value = UNASSIGNED;
	}
	is_contradicted = 0;//是否用冲突的单子句
	conflicting_literal = 0;//用于记录冲突的文字
	n_unit_clause_stack = 0;//全局栈，用于记录栈顶位置
	depth = 0;//DPLL树中，结点的深度
	char c;
	char s[5];
	int x = 1;
	while ((c = fgetc(fp)) == 'c')
		while ((c = fgetc(fp)) != '\n')
			;  //跳过注释部分
	if (feof(fp)) {
		fclose(fp);
		return NULL;
	}
	if (c != 'p') {
		fclose(fp);
		return NO;
	}
	fscanf(fp, "%s%d%d", s, &n_vars, &original_formula_length);
	if (strcmp(s, "cnf")) {
		fclose(fp);
		return NO;
	}
	current_formula_length = original_formula_length;
	clauses = (clause_infomation*)malloc(sizeof(clause_infomation) * original_formula_length);
	for (int i = 0; i < original_formula_length; i++) {
		clauses[i].literals = NULL;
		clauses[i].length_current = 0;
		clauses[i].is_clause_satisfied = NO;
		clauses[i].literals_is_assigned = NULL;
		clauses[i].unit_cluase_literal = 0;
		while (1) {
			fscanf(fp, "%d", &x);
			if (x == 0) break;
			clauses[i].literals = (int*)realloc(clauses[i].literals, (++clauses[i].length_current)*(sizeof(int)));
			clauses[i].literals[clauses[i].length_current - 1] = x;
			clauses[i].literals_is_assigned = (int*)realloc(clauses[i].literals_is_assigned, (clauses[i].length_current) * (sizeof(int)));;
			clauses[i].literals_is_assigned[clauses[i].length_current - 1] = UNASSIGNED;
			int t = x > 0 ? SATISFIED : SHRUNK;
			int s = abs(x);

			++literal_info[s][t].n_number;
			literal_info[s][t].literal_clause = (int*)realloc(literal_info[s][t].literal_clause, literal_info[s][t].n_number * (sizeof(int)));
			literal_info[s][t].literal_clause[literal_info[s][t].n_number - 1] = i;
			literal_info[s][t].literal_clause_pos = (int*)realloc(literal_info[s][t].literal_clause_pos, literal_info[s][t].n_number * (sizeof(int)));
			literal_info[s][t].literal_clause_pos[literal_info[s][t].n_number - 1] = clauses[i].length_current - 1;
		}
		clauses[i].length_original = clauses[i].length_current;
		if (clauses[i].length_original > max_clause_len) {
			max_clause_len = clauses[i].length_original;
		}
	}
	fclose(fp);
	return YES;
}
void CheckCNF() {
	for (int i = 0; i < original_formula_length; i++) {
		for (int j = 0; j < clauses[i].length_current - 1; j++) {
			printf("%d∨", clauses[i].literals[j]);
		}
		if (clauses[i].length_current) {
			printf("%d", clauses[i].literals[clauses[i].length_current - 1]);
		}
		putchar('\n');
	}
}