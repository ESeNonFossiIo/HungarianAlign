#ifndef __mask_h__
#define __mask_h__

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "permutation.h"
#include "matrix.h"
// #include "hungarian_matrix.h"


/**
 *  Mask is a character matrix used to annotate a numeric matrix during the
 *  Hungarian algorithm.
 *
 *  Each cell mirrors a cell of the matrix being processed and stores a marker:
 *  ' ' for a blank zero, 'x' for a covered/padded cell, '|' '-' '+' for the
 *  covering lines drawn over coloumns/rows, and 'O' / '.' for the selected and
 *  discarded entries of an assignment.
 */
class Mask : public Matrix<char>
{
public:
  /**
   *  Build an empty 1x1 mask whose cells default to the blank char ' '.
   */
  Mask();

  /**
   *  Build a mask sized after the numeric matrix @p m and initialise it from
   *  it (see initialize()).
   */
  Mask(std::vector<std::vector<unsigned int>> m);

  /**
   *  Refresh the mask state.
   */
  void
  update();

  /**
   * Calulate the numeber of 'x' in each row (coloumn) of mask
   * matrix.
   * If @p row is true it will be made using rows, otherwise
   * coloumns.
   * The result is a vector that indicates the number of 'x'
   * in each row (coloumn).
   */
  std::vector<unsigned int>
  zeroes(bool row=true);

  /**
   * Count, per row (or per coloumn when @p row is false), the cells marked as
   * selected/result entries.
   */
  std::vector<unsigned int>
  zeroes_result(bool row=true);

  /**
   * (Re)build the mask from the numeric matrix @p m: the mask is resized to the
   * larger dimension of @p m, non-zero cells become blank ' ' and zero/padded
   * cells become 'x'.
   */
  void
  initialize(std::vector<std::vector<unsigned int>> m);

  /**
   * Print the current state of the mask.
   */
  void
  status();

  /**
   * Return whether the markers currently cover every zero of the underlying
   * matrix (full rank reached).
   */
  bool
  rank();

  /**
   * Return the side of the (square) mask.
   */
   unsigned int
  get_size();

  /**
   * Pick one selected entry per row and per coloumn of the matrix @p m.
   *
   * The mask is re-initialised from @p m and, starting from the rows with the
   * fewest available cells, a single cell is marked as selected ('O') while the
   * others on the same row and coloumn are discarded ('.').
   */
  bool
  select_elements(std::vector<std::vector<unsigned int>> m);
  
  /**
   *  Overload of the output operator <<.
   *  It is possible to check the ouput of the matrix in the following way:
   *  @code{.cpp}
   *   Matrix p;
   *   ...
   *   std::cout << p;
   *  @endcode
   */
  friend std::ostream &operator<< (std::ostream &os, const Mask &dt);

protected:
  /**
   * Size of the mask.
   */
  unsigned int size;
};

#endif
