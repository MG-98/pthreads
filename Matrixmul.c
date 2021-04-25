    #include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define N  1000000

struct thread_data
{
   long thread_id;
   int *arr1;
   int *arr2;
   int arr1_row;
   int arr1_col;
   int arr2_col;
   int arr2_row;
   //char *message;
};

//int num_cols = 0;

int m_m_mul[4][4]; 


struct thread_data thread_data_array[NUM_THREADS];




void *matrix_mul (void *inputs )
{
    int mysum = 0 ;
    struct thread_data *my_data;
    my_data = (struct thread_data *) inputs;


    


    long thready_id  = my_data->thread_id;
    int *arr_1 = my_data->arr1;
    int *arr_2 = my_data->arr2;
    int r_1 = my_data->arr1_row;
    int c_1 = my_data->arr1_col;
    int c_2 = my_data->arr2_col;
    int r_2 = my_data->arr2_row;

// printf("%d" , c_2);
//      for(int i=0; i<r_1; i++) {
//       for(int j=0;j<c_2;j++) {
//          m_m_mul[i][j] = 0 ;
//       }
//    }

    int n = sizeof(my_data->arr1) / sizeof(my_data->arr1[0]);

    int ele_per_thread = (r_1*c_1)/NUM_THREADS;
    int mystart = thready_id * ele_per_thread;
    int myend = (thready_id+1) * ele_per_thread;

  


     printf("my start %d\n", mystart);

    printf("myend %d\n", myend);

    for (int i = mystart; i <myend ; i++)

    {
        
        for (int j = i%c_1 ; j <r_2*c_2 ; j+=r_2)
        {
           
          //   printf("f_1: %d\n", arr_1[i] );
            //            printf("s_1 %d\n", arr_2[j]);
              //            printf("i: %d\n", i/c_1 );
                //        printf("j %d\n", j/r_2);

    pthread_mutex_t mutexvar;
    pthread_mutex_init(& mutexvar , NULL);
    pthread_mutex_lock(&mutexvar);
    m_m_mul[i/c_1][j/r_2] = m_m_mul[i/c_1][j/r_2] + arr_2[j]*arr_1[i] ;

    pthread_mutex_unlock(&mutexvar);
    pthread_mutex_destroy(&mutexvar);
    
        }
      
   }
     for(int i=0; i<r_1; i++) {
      for(int j=0;j<c_2;j++) {
         printf("%d\t", m_m_mul[i][j] ); 
      }
      printf("\n");
         
}
}
 








int main()
{
    // inisialize any matrix
    int matrix_1[4][2] = { { 2, 3 }, { 1, 5} , { 1, 3 } , {5 ,6} };
        int matrix_2[2][4] = { { 2, 3,5,6 }, { 1, 5,1,9 }  };


    // get the number of rows and coloums
    int num_rows_1 = sizeof(matrix_1) / sizeof(matrix_1[0]);
    int num_cols_1 = sizeof(matrix_1[0]) / sizeof(matrix_1[0][0]);

     int num_rows_2 = sizeof(matrix_2) / sizeof(matrix_2[0]);
    int num_cols_2 = sizeof(matrix_2[0]) / sizeof(matrix_2[0][0]);

    // Initialzie output matrix
     // int m_m_mul[num_rows_1][num_cols_2]; 

    for(int i=0; i<num_rows_1; i++) {
     for(int j=0;j<num_cols_2;j++) {
                   m_m_mul[i][j] = 0 ;
     }
    }


    
   // falatten 
   int first_arr_flatten[num_cols_1*num_rows_1];
   int sec_arr_flatten[num_cols_2*num_rows_2];

   for (int i=0; i < num_rows_1; i++)
    { for (int j =0; j< num_cols_1; j++)
    first_arr_flatten[i*num_cols_1+j] = matrix_1[i][j];
    } 
for (int i=0; i < num_rows_2; i++)
    { for (int j =0; j< num_cols_2; j++)
    {
     sec_arr_flatten[j*num_rows_2+i] = matrix_2[i][j];
        
    }
    } 



    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
clock_t begin, end;
	time_t start, stop;
    double time_spent_sin , time_spen_sin_vectorized;
time(&start);
   begin = clock();

	

for ( t = 0; t < NUM_THREADS; t++)
    {
        thread_data_array[t].thread_id = t;
        thread_data_array[t].arr1 = first_arr_flatten;
        thread_data_array[t].arr2 = sec_arr_flatten;
        thread_data_array[t].arr1_row = num_rows_1;
        thread_data_array[t].arr1_col = num_cols_1;
        thread_data_array[t].arr2_col = num_cols_2;
        thread_data_array[t].arr2_row= num_rows_2;




        rc = pthread_create(&threads[t],NULL , matrix_mul , (void*)&thread_data_array[t]);

        
    }

    end = clock();
	
	time(&stop);
	time_spen_sin_vectorized =(double)(end - begin)/ CLOCKS_PER_SEC;// get processor time * 

     if (rc)
    {
        printf("ERROR; return code from pthread_creat() is %d\n" , rc);
        exit(-1);
        
    }

    for (long i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i] , NULL);
        }
    //printf("this is Parent!\n");
    	printf("Matrix multiplication took %f seconds to execute\n", time_spen_sin_vectorized);

    
   // printf("SUM %d\n", GetTimeLx());
       


}