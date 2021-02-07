#include <iostream>
#include <omp.h>
#include <stdlib.h>

#define ROW 40000
#define COL 50000

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
			
	
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			s += A[i][j];
		}
	}
			
	
	avg = s / (ROW*COL);
	
	cout << "\navg is : " << avg << endl;

	return 0;
}
