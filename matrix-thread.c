/******************************************************************************/
  /*!
  \file   matrix-thread.c
  \author Henry Morgan
  \par    email: henry.morgan\@digipen.edu
  \par    DigiPen login: henry.morgan
  \par    Course: CS180
  \par    Assignment #4
  \brief  
      This file contains the row/column multiplication function for
      individual threads to go through.
  */
/******************************************************************************/
#include <stdlib.h> /* malloc, free*/
#include "matrix-thread.h"

/************************************************************************/
  /*!
    \brief
      Multiplies a specified row and column of a matrix.
      
    \param data
      Points to a thread_data structure with row/column/width/input/output info.
      
    \return
      Exit status. (NULL if exit status should be ignored.)
  */
/************************************************************************/
void* multiplyrc(void *data)
{
  int *rowData, *columnData; /* used to hold the relevant row and column*/
  thread_data *td = (thread_data*)data; /* Accessing the thread data. */
  int result = 0; /* Result of multiplying row and column. */
  int i;

  /* Reading the correct row and column.*/
  rowData = malloc(sizeof(int) * td->width);
  for(i = 0; i < td->width; ++i)
  {
    rowData[i] = td->input_matrix[(td->width * td->row_number) + i];
  }
  columnData = malloc(sizeof(int) * td->width);
  for(i = 0; i < td->width; ++i)
  {
    columnData[i] = td->input_matrix[td->column_number + (td->width * i)];
  }
  
  /* Multiplying the row and column together to get a single integer value.*/
  for(i = 0, result = 0; i < td->width; ++i)
  {
    result += rowData[i] * columnData[i];
  }
  
  /* Writing the integer value to the proper location in the shared memory.*/
  td->result_matrix[td->result_position] = result;

  /* Releasing any memory or resources needed, including the shared memory */
  free(columnData);
  free(rowData);
  
  
  return 0;
}