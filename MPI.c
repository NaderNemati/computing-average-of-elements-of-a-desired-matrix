#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define MASTER 0
#define row 10000
#define col 20000

int main(int argc, char *argv[])
{
    int *a;
    int *ap;
    int N = row*col;
    int rank,block,size,n,i;
    float sum=0;
    float res=0;
    //start time of run
    clock_t tic = clock();
    MPI_Status status;
	//MPI started
    MPI_Init(&argc,&argv);
	//Find number of processors and size
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank==0)
    {	
	    //size of each block
    	block = ceil((float)N/(float)size);
        n = block * size;
        //definitio of arrays
        double *a = (double*)malloc(n * sizeof(double));
	    double *ap= (double*)malloc(block * sizeof(double));
		float sum=0;
		float res=0;
		//definition of array elements
		for (int i = 0; i < n; i++)
		{
			a[i] = i;
		}
		//broudcasting and scatter array
		MPI_Bcast (&block, 1, MPI_INT, MASTER,MPI_COMM_WORLD);
		MPI_Scatter(a, block, MPI_INT, ap, block,MPI_INT, MASTER, MPI_COMM_WORLD);
		for (int i = 0; i < block; ++i)
		{
			sum+=ap[i];
		}
		//calculation of array elements
		MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, MASTER,MPI_COMM_WORLD);

		printf("Total sum = %f, Matrix avg = %f\n", res, res/n);
   }	

   else
   {
   		//processing in other processors
		block = ceil((float)N/(float)size);
       	n = block * size;
		double *ap= (double*)malloc(block * sizeof(double));
		MPI_Scatter(a, block, MPI_INT, ap, block,MPI_INT, MASTER, MPI_COMM_WORLD);
		float sum=0;	
		for (int i = 0; i < block; ++i)
		{
			sum+=ap[i];
		}
		MPI_Reduce(&sum, &res, 1, MPI_INT, MPI_SUM, MASTER,MPI_COMM_WORLD);


    }
	MPI_Finalize();
	clock_t toc = clock();;
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
}
