#include <stdio.h>

int get_len_file(FILE *fp);
int get_len_col_file(FILE *fp, int row, int col);
int gets_lines_file(FILE *fp);
int gets_jumplines_file(FILE *fp);

void move_to_line_file(FILE *fp, int row);
void move_to_col_file(FILE *fp, int col);

void read_line_file(FILE *fp, int row, char *str);
void read_col_file(FILE *fp, int row, int col, char *str);

int search_data_file(FILE *fp, int col, char *str);

void delete_line_file(FILE *fp, int row);
void add_line_file(FILE *fp, char *str);
void modify_col_file(FILE *fp, int row, int col, char *str);