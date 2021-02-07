#include <iostream>
#include <omp.h>
#include <stdlib.h>

#define ROW 30000
#define COL 30000

using namespace std;

int main()
{
	int i = 0, j = 0;
	float s = 0, avg = 0;

	int ** A = new int * [ROW];
	for (i = 0; i < ROW; i++) {
		A[i] = new int [COL];
	}
	
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			A[i][j] = rand() % 1000;
		}
	}	
	omp_set_dynamic(0);
	omp_set_num_threads(8);		
	
	#pragma omp parallel for private(j) reduction(+:s)
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			s += A[i][j];
		}
	}
			
	
	avg = s / (ROW*COL);
	
	cout << "\navg is : " << avg << endl;

	return 0;
}

