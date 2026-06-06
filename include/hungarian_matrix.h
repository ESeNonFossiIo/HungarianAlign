#ifndef __hungarian_matrix_h__
#define __hungarian_matrix_h__

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "permutation.h"
#include "mask.h"
#include "matrix.h"


/**
 *  Hungarian Matrix Class stores data in matrix form and solves the assignment
 *  problem of maximising the trace.
 *
 *  The class keeps the original cost matrix (inherited @p mat) and a working
 *  square matrix @p res on which the Hungarian algorithm operates. The typical
 *  flow is: populate the matrix with add_an_element(), turn it into a square
 *  cost matrix with update(), reduce rows and coloumns with first_step() /
 *  second_step() until check_rank() succeeds, then read the optimal assignment
 *  with check_result() / solution().
 */
class HMatrix : public Matrix<unsigned int>
{
public:
  /**
   *  Build an empty Hungarian matrix of size 0x0.
   */
  HMatrix();

  /**
   * Copy constructor: build a Hungarian matrix with the same entries as @p m
   * and immediately refresh its working matrix through update().
   */
  HMatrix (const HMatrix &m);

  /**
   *  (Re)build the square working matrix @p res from the current entries.
   *
   *  The result is padded with @p void_char to a square size equal to the
   *  larger dimension. When @p complement is true (default) each entry is
   *  replaced by its complement to the maximum value (max_val - entry), turning
   *  the maximise-trace problem into the minimise-cost form solved by the
   *  Hungarian algorithm; when false the original values are copied as they are.
   */
  void
  update(bool complement = true);

  /**
   *  Swap two rows (or two coloumns) of the working matrix.
   *
   *  Elements @p i and @p j of the row permutation (or, when @p row is false,
   *  of the coloumn permutation) are swapped and @p res is rebuilt accordingly
   *  so that it stays consistent with the current permutations.
   */
  void
  switch_elements(unsigned int i, unsigned int j, bool row=true);

  /**
   *  Reorder rows (or coloumns when @p row is false) of the matrix.
   */
  void
  sort(bool row=true);

  /**
   *  Print the optimal assignment found by the algorithm.
   *
   *  Calls diagonalize() to move the selected entries onto the main diagonal,
   *  then prints the diagonal solution matrix together with the resulting row
   *  and coloumn permutations (with their labels).
   */
  void
  solution();

  /**
   *  Permute the coloumns so that the selected assignment (the entries marked
   *  'O' in @p result_mask) lies on the main diagonal of the matrix.
   */
  void
  diagonalize();

  /**
   *  Register a pairing between row label @p c_row and coloumn label @p c_col.
   *
   *  The labels are added to the row/coloumn permutations (if new) and the
   *  matrix entry at their indices is incremented by one, growing the matrix
   *  when needed.
   */
  void
  add_an_element(char c_row, char c_col);

  /**
   *  Remove from each row (coloumn) the minimun element.
   *  @p row set if operation will be made on rows (true)
   *  or coloumns (false). Default value is trues.
   */
  void
  first_step(bool row = true);

  /**
   *  Cover-correcting step of the Hungarian algorithm.
   *
   *  Finds the smallest uncovered entry of @p res (the cells still marked as
   *  blank ' ' in @p mask), subtracts it from the uncovered coloumns and adds
   *  it back to the covered rows, thus creating new zeros while preserving the
   *  optimality conditions. @p row selects whether the correction is driven by
   *  rows (the only branch currently implemented) or coloumns.
   */
  void
  second_step(bool row = true);

  /**
   * Return the indices of the rows (or coloumns when @p row is false) that
   * still contain at least one blank ' ' cell in @p mask, i.e. an uncovered
   * zero of the working matrix.
   */
  std::vector<unsigned int>
  blank(bool row=true);

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
   *  Return the working-matrix element @p res in postion (@p i , @p j).
   */
  unsigned int
  status(unsigned int i, unsigned int j);

  /**
   * Cover all the zeros of the working matrix with the minimum number of lines
   * and decide whether an optimal assignment is already reachable.
   *
   * The zeros are repeatedly covered by drawing a line over the row or coloumn
   * that contains the most of them (bookkeeping done in @p mask). The function
   * returns true when the number of covering lines equals the matrix size,
   * meaning an optimal assignment exists; false otherwise (a second_step() is
   * then required).
   */
  bool
  check_rank();

  /**
   * Extract one optimal assignment from the reduced matrix.
   *
   * Starting from the rows/coloumns with the fewest zeros, it selects exactly
   * one zero per row and per coloumn, marking the chosen cells with 'O' in
   * @p result_mask and discarding the others. This mask is later consumed by
   * diagonalize() / solution().
   */
  bool
  check_result();

  /**
   *  Overload of the output operator <<.
   *  It is possible to check the ouput of the matrix in the following way:
   *  @code{.cpp}
   *   Matrix p;
   *   ...
   *   std::cout << p;
   *  @endcode
   */
  friend std::ostream &operator<< (std::ostream &os, const HMatrix &dt);

protected:
  /**
   *  Working mask used by check_rank() to cover the zeros of @p res with the
   *  minimum number of lines.
   */
  Mask mask;

  /**
   *  Mask holding the selected assignment (cells marked 'O') produced by
   *  check_result() and consumed by diagonalize().
   */
  Mask result_mask;

private:
  /**
   *  Square working matrix on which the Hungarian reductions are performed.
   */
  std::vector<std::vector<unsigned int> > res;

  /**
   *  Labeled permutation tracking the current ordering of the rows.
   */
  LPermutation p_row;

  /**
   *  Labeled permutation tracking the current ordering of the coloumns.
   */
  LPermutation p_col;

  /**
   * Side of the square working matrix @p res (the larger of the original row
   * and coloumn counts).
   */
  unsigned int size;

  /**
   * Maximum entry of the original cost matrix, used to build the complement in
   * update().
   */
  unsigned int max_val;
};

#endif
