#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ROW_SIZE 9
#define COL_SIZE 9
#define GRIDS 9
#define NUMS 9

int is_valid = 1;

const int puzzle[COL_SIZE][ROW_SIZE] = { 
	{6,2,4,5,3,9,1,8,7},
	{5,1,9,7,2,8,6,3,4},
	{8,3,7,6,1,4,2,9,5},
	{1,4,3,8,6,5,7,2,9},
	{9,5,8,2,4,7,3,6,1},
	{7,6,2,3,9,1,4,5,8},
	{3,7,1,9,5,6,8,4,2},
	{4,9,6,1,8,2,5,7,3},
	{2,8,5,4,7,3,9,1,6}
};

typedef struct {
	int row;
	int col;
	
} parameters;

void *check_col(void *param);
void *check_row(void *param);
void *check_grid(void *param);
int main(){
	int i;
	
	pthread_t col_checker;
	pthread_t row_checker;
	pthread_t grid_checkers[GRIDS];
	
	pthread_create(&col_checker, NULL, check_col, NULL);
	pthread_create(&row_checker, NULL, check_row, NULL);

	pthread_join(col_checker, NULL);
	pthread_join(row_checker, NULL);	
	
	parameters *params_for_grids[GRIDS];
	
	for (i = 0; i < GRIDS; ++ i) 
		params_for_grids[i] = (parameters *) malloc(sizeof(parameters));
	int c = 0;
	for (i=0; i < 3; ++ i) {
		params_for_grids[c] -> row = c;
		params_for_grids[c] -> col = 0;

		params_for_grids[c + 1] -> row = c;
		params_for_grids[c + 1] -> col = 3;
		
		params_for_grids[c + 2] -> row = c;
		params_for_grids[c + 2] -> col = 6;
		
		c += 3;
	}	
	
	for (i=0; i < GRIDS; ++i) {
		pthread_create(&grid_checkers[i], NULL, check_grid, 
				params_for_grids[i]);
		pthread_join(grid_checkers[i], NULL);
	
	}
	
	printf("IS_VALID: %d\n", is_valid);
	
	for (i=0; i < GRIDS; ++i) {
		free(params_for_grids[i]);
	}
}

void *check_grid(void *param) {
	parameters *prm = (parameters *) param; 
	int j=0;
	int row = prm -> row;
	int col = prm -> col;
	
	int counter[NUMS];

	for (int i =0; i < NUMS; ++i)
		counter[i] = 0;

	for (int i=0; i < 3; ++i) {
		
		for (j=0; j < 3; ++j) {
			
			if (counter[puzzle[i + col][j + row]] == 1) {
				is_valid = 0;
				return NULL;
			} counter[puzzle[i + col][j + row]] = 1;
		} 
	}	
}

void *check_col(void *param) {
	int i=0,j=0;
	int counter[NUMS];

	for (; i < NUMS; ++i)
		counter[i] = 0;

	for (i=0; i < COL_SIZE; ++i)
		for (; j < ROW_SIZE; ++j) {
			if (counter[puzzle[i][j]] == 1) {
				is_valid = 0;
				return NULL;
			} counter[puzzle[i][j]] ++;
		}
	
}

void *check_row(void *param) {
	int i=0,j=0;
	int counter[NUMS];

	for (; i < NUMS; ++i)
		counter[i] = 0;

	for (i=0; i < ROW_SIZE; ++i)
		for (; j < COL_SIZE; ++j) {
			if (counter[puzzle[j][i]] == 1) {
				is_valid = 0;
				return NULL;
			} counter[puzzle[j][i]] ++;
		}
	
}
