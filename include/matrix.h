#ifndef __matrix_h__
#define __matrix_h__

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "permutation.h"

/**
 *  Matrix Class is a generic structure to store data in matrix form.
 */
template <typename type = unsigned int>
class Matrix
{
public:
  /**
   * Constructor takes the number of rows and coloumns and
   *  initialize mat with zeroes.
   */
  Matrix (unsigned int rows, unsigned int cols, type void_char = 0);

  /**
   * Constructor takes the number of rows and coloumns and
   *  initialize mat with the elements of m.
   */
  Matrix(unsigned int rows, unsigned int cols,
         std::vector<std::vector<type> > m,
         type void_char = 0);

  /**
   * Constructor a matrix with the same entries.
   */
  Matrix (const Matrix &m, type void_char = 0);

  /**
   * 	Allow to change the defaul value for empty position with the char @p c.
   */
  void
  change_void_char(type c);
  
  /**
   *  Resize the matrix in order to contain the element in position @p i
   *  and @p j. Empty cells are filled using @p void_char.
   */
  void
  resize  (unsigned int i, unsigned int j);

  /**
   *  Add one to the element in position @p i and @p j.
   *  @p output_matrix enables the output of the situation.
   */
  void
  add_one (unsigned int i, unsigned int j);

  /**
   *  Write the element @c in position (@p i , @p j ).
   */
  void
  write (unsigned int i, unsigned int j, type c);

  /**
   *  Return the number of rows and coloumns of the matrix.
   *  size().first is the number of rows and size().second is the number 
   *  of coloumns.  
   */
  std::pair<unsigned int, unsigned int>
  size ();

  /**
   * Calulate the numeber of zeroes in each row (coloumn).
   * If @p row is true it will be made using rows, otherwise
   * coloumns.
   * The result is a vector that indicates the number of zeroes
   * in each row (coloumn).
   */
  std::vector<unsigned int>
  zeroes(bool row=true);

  /**
   * Calulate the min value in each row (coloumn).
   * If @p row is true it will be made using rows, otherwise
   * coloumns.
   * The result is a vector that indicates the min value
   * in each row (coloumn).
   */
  std::vector<unsigned int>
  get_min(bool row=true);

  /**
   * Calulate the max value in each row (coloumn).
   * If @p row is true it will be made using rows, otherwise
   * coloumns.
   * The result is a vector that indicates the max value
   * in each row (coloumn).
   */
  std::vector<unsigned int>
  get_max(bool row=true);

  /**
   *  Calulate the maximum value of the matrix;
   */
  unsigned int
  maximum_value();

  /**
   *  Replace in place every entry with its complement to the maximum value,
   *  i.e. each entry becomes maximum_value() - entry.
   */
  void
  maximum_complement();

  /**
   *  Evaluate the trace, i.e. the sum of the diagonal entries up to the
   *  smaller dimension of the matrix.
   */
  unsigned int
  trace ();

  /**
   *  Status of the matrix:
   *  - Number of rows
   *  - Number of coloumns
   *  - Trace
   */
  void
  status();

  /**
   *  Copy operator.
   */
  Matrix &
  operator=(Matrix other);

  /**
   *  Overload of the output operator <<.
   *  It is possible to check the ouput of the matrix in the following way:
   *  @code{.cpp}
   *   Matrix p;
   *   ...
   *   std::cout << p;
   *  @endcode
   */
  friend std::ostream &operator<< (std::ostream &os,
                                   const Matrix &dt);

  /**
   * Access to the element (@p i , @p j).
   */
  type &operator()(unsigned int i, unsigned int j);
  // friend type &operator[](const unsigned int i, const unsigned int j);

protected:
  /**
   *  Number of rows.
   */
  unsigned int n_row;

  /**
   *  Number of coloumns.
   */
  unsigned int n_col;

  /**
   *  Matrix.
   */
  std::vector<std::vector<type> > mat;

private:
  /**
   *  Typeset that represent the empty value of the matrix. 
   *  For istance it could be 0 in the case of unsigned int or ' ' in the case 
   *  of char.
   */
  type void_char;
};

#endif
