#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

int rand_lim(int min, int max){	//   Returns a random integer with the range of min and max
    return (((double)rand() / (((double)RAND_MAX) + 1.0)) * (max-min+1) + min);
}

double testA()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
	while(i < 150)
	{
		char* p = (char*)malloc(sizeof(char));
		free(p);
		i++;
	}
	gettimeofday(&end, NULL);

	return (end.tv_usec - start.tv_usec);
}

double testB()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
	char* arr[150];
	while(i < 150)
	{
		arr[i] = (char*)malloc(sizeof(char));
		i++;
	}
	i = 0;
	while(i < 150)
	{
		free(arr[i]);
		i++;
	}
	gettimeofday(&end, NULL);

	return (end.tv_usec - start.tv_usec);
}

double testC()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
	int mCount = 0;	//keeps track of number of mallocs
	int fCount = 0;	//keeps track of number of frees
	char* arr[150];
	while((mCount < 150) || (fCount < 150))
	{
		if(mCount < 150)	//randomly malloc or free
		{
			i = rand_lim(0, 1);
			if(i == 0)	//malloc
			{
				arr[mCount] = (char*)malloc(sizeof(char));
				mCount++;
			}
			else if((i == 1) && (mCount > fCount))	//free
			{
				free(arr[fCount]);
				fCount++;
			}
			else
			{
				continue;
			}
		}
		else	//150 bytes have been malloced, free remaining bytes
		{
			while(fCount < 150)
			{
				free(arr[fCount]);
				fCount++;
			}
		}
	}
	gettimeofday(&end, NULL);

	return (end.tv_usec - start.tv_usec);
}

double testD()
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
	int j = 0;
        int mCount = 0; //keeps track of number of mallocs
        int fCount = 0; //keeps track of number of frees
        int memTrack = 0;	//keeps track of total memory allocated
	int mallocTrack[150];	//keeps track of # of bytes each malloc allocated
	char* arr[150];
        while((mCount < 150) || (fCount < 150))
        {
                if(mCount < 150)        //randomly malloc or free
                {
                        i = rand_lim(0, 1);
			j = rand_lim(1, 64);
                        if((i == 0) && ((memTrack + j) <= 5000))      //malloc
                        {
				mallocTrack[mCount] = j;
				memTrack += j;
                                arr[mCount] = (char*)malloc(sizeof(char)*j);
                                mCount++;
                        }
                        else if((i == 1) && (mCount > fCount))  //free
                        {
                                memTrack -= mallocTrack[fCount];
				free(arr[fCount]);
                                fCount++;
                        }
                        else
                        {
                                continue;
                        }
                }
                else    //150 bytes have been malloced, free remaining bytes
                {
                        while(fCount < 150)
                        {
                                memTrack -= mallocTrack[fCount];
				free(arr[fCount]);
                                fCount++;
                        }
                }
        }
	gettimeofday(&end, NULL);

	return (end.tv_usec - start.tv_usec);
}

double testE()	//tests assigning values to allocated memory locations
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
        char* arr[150];
        while(i < 150)
        {
                arr[i] = (char*)malloc(sizeof(char));
                *(arr[i]) = 'a';
		i++;
        }
        i = 0;
        while(i < 150)
        {
                free(arr[i]);
                i++;
        }

	gettimeofday(&end, NULL);
        return (end.tv_usec - start.tv_usec);
}

double testF()	//tests mallocs of increasing size
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int i = 0;
        while(i < 500)
        {
                char* p = (char*)malloc(sizeof(char) * (i+1));
                free(p);
		i++;
        }

	gettimeofday(&end, NULL);
        return (end.tv_usec - start.tv_usec);
}
	
int main(int argc, char* argv[])
{
	double meanA = 0;
        double meanB = 0;
        double meanC = 0;
        double meanD = 0;
        double meanE = 0;
        double meanF = 0;

	int i = 0;
	while(i < 100)	//runs each test 100 times
	{
		meanA += testA();
		meanB += testB();
		meanC += testC();
		meanD += testD();
		meanE += testE();
		meanF += testF();
		i++;
	}
	//each variable now holds total sum of 100 run times (in microseconds), 
	//dividing my 100 to find average
	meanA /= (100.0);
	meanB /= (100.0);
	meanC /= (100.0);
	meanD /= (100.0);
	meanE /= (100.0);
	meanF /= (100.0);

	//prints out average run times of each workload
	printf("Average run time for test A: %.2lf microseconds\n", meanA);
	printf("Average run time for test B: %.2lf microseconds\n", meanB);
	printf("Average run time for test C: %.2lf microseconds\n", meanC);
	printf("Average run time for test D: %.2lf microseconds\n", meanD);
	printf("Average run time for test E: %.2lf microseconds\n", meanE);
	printf("Average run time for test F: %.2lf microseconds\n", meanF);

        return 0;
}

