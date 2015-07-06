/******************************************************************************/
  /*!
  \file   main-thread.c
  \author Henry Morgan
  \par    email: henry.morgan\@digipen.edu
  \par    DigiPen login: henry.morgan
  \par    Course: CS180
  \par    Assignment #4
  \brief  
      This file contains the main flow for matrix squaring. (individual
      operations are made into separate threads)
      
      Functions:
        - get_matrix (not student-implemented)
        - print_matrix (not student-implemented)
        - main (largely student-implemented)
        - wait_for_threads (student-implemented)
      
    Hours spent on this assignment: 2

    Specific portions that gave you the most trouble: None

  */
/******************************************************************************/
#include <stdio.h>         /* printf, fopen, fclose, fscanf */
#include <stdlib.h>        /* exit, malloc                  */
#include <pthread.h>       /* thread create/join/exit       */
#include "matrix-thread.h" /* thread data struct, thread fn */

/************************************************************************/
  /*!
    \brief
      Gets a matrix (and its width) from a file, stores it in memory.
        
    \param filename
      The name of a file to get a matrix from.
      
    \param width
      Where the width of the matrix will be stored.
      
    \return
      A pointer to the now-stored matrix.
  */
/************************************************************************/
int *get_matrix(const char *filename, int *width)
{
  int value, *matrix, result;
  FILE *fp;
  
  fp = fopen(filename, "rt");
  if (!fp)
  {
    printf("Can't open file: %s\n", filename);
    exit(-1);
  }
  
  result = fscanf(fp, "%d", width);
  if (result == -1)
  {
    printf("Can't read from file: %s\n", filename);
    fclose(fp);
    exit(-1);
  }

  matrix = malloc(*width * *width * sizeof(int));
  if (!matrix)
  {
    printf("Can't malloc matrix\n");
    fclose(fp);
    exit (-2);
  }
  
  while (!feof(fp))
  {
    result = fscanf(fp, "%d", &value);
    if (result == -1)
      break;
    *matrix++ = value;
  }
  fclose(fp);
  return matrix - (*width * *width);
}

/************************************************************************/
  /*!
    \brief
      Prints out a matrix that is stored in memory.
        
    \param matrix
      The matrix to print out.
      
    \param width
      The width of the matrix (to know its overall size).
  */
/************************************************************************/
void print_matrix(int *matrix, int width)
{
  int i, size = width * width;
  for (i = 0; i < size; i++)
  {
    printf("%8i", matrix[i]);
    if ( (i + 1) % width == 0)
      printf("\n");
  }
  printf("\n");
}

/************************************************************************/
  /*!
    \brief
      Holds while waiting for all threads to complete tasks.
      
    \param matsize
      The total size of the matrix. (I.E. 9 for a 3x3 matrix.)
      
    \param thread_id
      Points to an array containing IDs of all created threads.
  */
/************************************************************************/
void wait_for_threads(int matsize, pthread_t *thread_id)
{
  int i;

  for (i = 0; i < matsize; ++i) /* Going through all possible threads*/
  {
    /* Nothing for the parent to do while waiting but check again. */
    /* Currently waits for each process in order.*/
    pthread_join(thread_id[i], NULL);
  }
}

/************************************************************************/
  /*!
    \brief
      Squares a matrix
        
    \param argc
      The number of arguments.
      
    \param argv
      Array of arguments. (input filename)
      
    \return
      Because this is main.
  */
/************************************************************************/
int main(int argc, char **argv) 
{
  int i;
  int width;          /* width of the matrix           */
  int matsize;        /* total matrix values           */
  int *input_matrix;  /* the matrix read in            */
  int *result_matrix; /* threads will put results here */
  pthread_t *thread_id; /* thread IDs will be stored here */
  thread_data *td; /* Array of thread data */
  
  if (argc < 2)
  {
    printf("Insufficient parameters supplied\n");
    return -1;
  }
  
  /* Reading the input matrix from a file into it's memory. */
  input_matrix = get_matrix(argv[1], &width);
  matsize = width * width;
  
  /* Printing the input matrix. */
  print_matrix(input_matrix, width);
  
  /* Preparing the result matrix. */
  result_matrix = (int*)malloc(sizeof(int) * matsize);
  
  /* Preparing the thread ID matrix and data structs */
  thread_id = (pthread_t*)malloc(sizeof(pthread_t*) * matsize);
  td = (thread_data*)malloc(sizeof(thread_data) * matsize);
  
  /* Creating all of the other threads and supplying them with */
  /* their parameters                                          */ 
  for (i = 0; i < matsize; ++i)
  {
    /* Setting thread data. */
    td[i].input_matrix = input_matrix;
    td[i].result_matrix = result_matrix;
    td[i].width = width;
    td[i].row_number = i / width;
    td[i].column_number = i % width;
    td[i].result_position = (td[i].row_number * width) + td[i].column_number;
    
    /* Creating thread*/
    pthread_create(&thread_id[i], NULL, multiplyrc, &td[i]);
  }

  /* Waiting for all of the threads to finish. */
  wait_for_threads(matsize,thread_id);
    
  /* Printing the resulting squared matrix. */
  print_matrix(result_matrix, width);
  
  /* Cleaning up any memory or resources the main thread created. */
  free(td);
  free(thread_id);
  free(result_matrix);
  free(input_matrix);
  
  return 0;
}
