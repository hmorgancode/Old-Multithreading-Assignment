/******************************************************************************/
  /*!
  \file   matrix-thread.h
  \author Henry Morgan
  \par    email: henry.morgan\@digipen.edu
  \par    DigiPen login: henry.morgan
  \par    Course: CS180
  \par    Assignment #4
  \brief  
      This file contains the definition for the thread_data struct used in
      main-thread.c and matrix-thread.c. (The thread_data struct holds data
      that is passed to threads as they are created.)
  */
/******************************************************************************/
/*!
  \brief Stores information to be passed to individual threads.
*/
typedef struct
{
  /* pointer to input matrix and result matrix */
  int *input_matrix, *result_matrix;
  /* width, row and column numbers  */
  int width, row_number, column_number, result_position;
}thread_data;

void* multiplyrc(void *data);