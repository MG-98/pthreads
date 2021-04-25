#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>
#define PI 3.141592653589

void sinxx(int N,  int terms,  float* x,  float* result)
{
	int i ;
	int j;

   for (i=0 ; i<N ; i++)
   {
	float  value  = x[i];
    float  numer  = x[i]*x[i]*x[i];
    int   denom = 6;  //   3 !
    int  sign = -1;


for ( j=1; j<=terms; j++)
{
value +=  sign * numer / denom; 
numer *= x[i] * x[i];
denom *=   (2*j+2) * (2*j+3);
sign  *= -1;
}
result[i] = value;
printf("Sin(%f)=%f\n",x[i]*(180/PI) ,result[i]);

}
}


void sinxx_vectorized(int N,  int terms,  float* x,  float* result)
{
	int i ;
	int j;

   for (i=0 ; i<N ; i+=8)
   {

    __m256 value = _mm256_load_ps(x);
    
    __m256 numer = _mm256_mul_ps(value, value);
    numer = _mm256_mul_ps(numer , value);


    
    __m256 demon = _mm256_set1_ps(6);
   __m256 sign = _mm256_set1_ps(-1);
   __m256 neg = _mm256_set1_ps(-1);
    

 

for ( j=1; j<=terms; j++)
{
value = _mm256_add_ps( _mm256_div_ps(_mm256_mul_ps(sign, numer) , demon) , value) ;
numer = _mm256_mul_ps(_mm256_mul_ps(value , value) , numer);
demon =  _mm256_mul_ps(_mm256_set1_ps((2*j+2)*(2*j+3)), demon);
sign = _mm256_mul_ps(sign , neg);
 
}
//result  = x;
  // _mm256_store_ps(x + i, value);

for (int n = i; n < i+8; n++) 
 {

       printf("Sin(%f)=%f\n",x[n]*(180/PI),result[n]);

    }

   }
}
int main()
{
	int N;
	int terms; 
	float* x;
	float* result;
	int index;
	clock_t begin, end;
	time_t start, stop;
    double time_spent_sin , time_spen_sin_vectorized;
     float startdegree;
	 char ch;

	printf("Please Enter the number of angles (N) \n");
	scanf("%d",&N);

	printf("please number of terms to calculate each angle:EX:10terms or more to get more accurate result\n");
	scanf("%d",&terms);
  
     printf(" Enter the start angle with degrees not radain\n");
	 printf("(NOTE:each angle increase from previous by 0.5 degrees)\n");
     scanf("%f",&startdegree);

   time(&start);
   begin = clock();

	x= (float*)malloc(N*sizeof(float));
	result= (float*)malloc( N * sizeof(float));
	for(index=0; index<N;index++)
	{
		
		 x[index]=startdegree*(PI/180);
		startdegree=startdegree+0.5;
        printf("%f",x[index]);
		result[index] = 0.005	;
		}
	
    
   time(&start);
   begin = clock();
	sinxx( N, terms, x, result);

	end = clock();
	
	time(&stop);


	time_spent_sin =(double)(end - begin)/ CLOCKS_PER_SEC;// get processor time * 
	printf("sin() took %f seconds to execute\n", time_spent_sin);


	printf("Finished in about %.0f seconds. \n", difftime(stop, start));//*calculate program execute time 

//=======================
     time(&start);
   begin = clock();
	sinxx_vectorized( N, terms, x, result);

	end = clock();
	
	time(&stop);
	time_spen_sin_vectorized =(double)(end - begin)/ CLOCKS_PER_SEC;// get processor time * 
	printf("sin_vectorized() took %f seconds to execute\n", time_spen_sin_vectorized);


	printf("Finished in about %.0f seconds. \n", difftime(stop, start));//*calculate program execute time 

     getchar();
	  
	return 0;

}