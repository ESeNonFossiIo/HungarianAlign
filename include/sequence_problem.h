#ifndef __sequence_problem_h__
#define __sequence_problem_h__

#include <fstream>

#include "hungarian_matrix.h"

/**
 *  SequenceProblem models the alignment of two labelled sequences as an
 *  assignment problem.
 *
 *  Each pair of labels coming from the two input sequences increments the
 *  corresponding entry of an internal cost matrix (@p problem_matrix). Solving
 *  the problem amounts to finding the permutation of rows and coloumns that
 *  maximises the trace of that matrix, which is computed through the Hungarian
 *  algorithm implemented in HMatrix.
 */
class SequenceProblem
{
public:
  /**
   * Build an empty problem with a zero-sized cost matrix. Entries are added
   * later through one of the fill() overloads.
   */
  SequenceProblem();

  /**
   * Fill the cost matrix reading two data files.
   *
   * Both files are scanned in parallel, one entry at a time, where every entry
   * is a pair "index label" (an unsigned int followed by a char). For each
   * step the row label read from @p file1 and the coloumn label read from
   * @p file2 are registered into the cost matrix. Scanning stops as soon as one
   * of the two files is exhausted.
   */
  void
  fill(std::string file1, std::string file2);

  /**
   * Fill the cost matrix from an explicit list of label pairs.
   *
   * For every pair in @p sequence, @c first is used as the row label and
   * @c second as the coloumn label, incrementing the matching cost entry.
   */
  void
  fill(std::vector<std::pair<char, char> > sequence);

  /**
   * Solve the assignment problem with the Hungarian algorithm and print the
   * intermediate steps (original matrix, complement, row/coloumn reductions)
   * together with the final optimal assignment.
   */
  void
  solve();

private:
  /**
   * Hungarian cost matrix holding the pairing counts between the two
   * sequences; it carries out the actual optimisation in solve().
   */
  HMatrix problem_matrix;
};

#endif
