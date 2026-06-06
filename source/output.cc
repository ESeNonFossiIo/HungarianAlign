#include <iostream>     // std::cout, std::endl
#include <iomanip>      // std::setw
#include <algorithm>    // std::find
#include <set>
#include <vector>
#include <cmath>

#include "matrix.h"
#include "hungarian_matrix.h"
#include "permutation.h"
#include "utilities.h"

template <typename type>
std::ostream &
operator<<(std::ostream &os, const Matrix<type> &m)
{
  // How to start each row:
  std::string row_init = "    ";
  // Length of each coloum:
  unsigned int spaces  = 5;

  os << std::endl;
  for (unsigned int r = 0; r < m.n_row; ++r)
    {
      os << row_init << "+" << "-----";
      for (unsigned int c = 1; c < m.n_col; ++c)
        os << "+" << "-----";
      os << "+" << std::endl
         << row_init << "|";
      for (unsigned int c = 0; c< m.n_col; ++c)
        os << std::setw(spaces) << m.mat[r][c] << "|";
      os << std::endl;
    }
  os << row_init;
  for (unsigned int c = 0; c < m.n_col; ++c)
    os << "+" << "-----";
  os << "+" << std::endl;

  return os;
}

// Explicit instantiations so the operator is emitted in this translation unit
// and can be linked from the tests and other sources.
template std::ostream &operator<<(std::ostream &, const Matrix<unsigned int> &);
template std::ostream &operator<<(std::ostream &, const Matrix<char> &);

std::ostream &
operator<<(std::ostream &os, const HMatrix &m)
{
  // How to start each row:
  std::string row_init = "    ";
  // Length of each coloum:
  unsigned int spaces  = 5;

  os << std::endl;
  for (unsigned int r = 0; r < m.size; ++r)
    {
      os << row_init << "+" << "-----";
      for (unsigned int c = 1; c < m.size; ++c)
        os << "+" << "-----";
      os << "+" << std::endl
         << row_init << "|";
      for (unsigned int c = 0; c< m.size; ++c)
        os << std::setw(spaces) << m.res[r][c] << "|";
      os << std::endl;
    }
  os << row_init;
  for (unsigned int c = 0; c < m.size; ++c)
    os << "+" << "-----";
  os << "+" << std::endl;

  return os;
}

std::ostream &
operator<<(std::ostream &os, LPermutation &p)
{
  // Length of each entries:
  unsigned int spaces = 3;

  os << "Sequence:  ";
  os << "<";
  for (unsigned int i = 0; i<p.tot-1; ++i)
    os << std::setw(spaces) << p[i] <<  ",";
  os << std::setw(spaces) << p[p.tot-1] << ">" << std::endl;
  os << "Labels:    ";
  os << "<";
  for (unsigned int i = 0; i<p.tot-1; ++i)
    os << std::setw(spaces) << p.from_int_to_char[p[i]] <<  ",";
  os << std::setw(spaces) << p.from_int_to_char[p[p.tot-1]] << ">" << std::endl;
  return os;
}


std::ostream &
operator<<(std::ostream &os, Mask &mask)
{
  std::string row_init = "    ";
  unsigned int spaces  = 2;
  os << std::endl;
  
  for (unsigned int r = 0; r < mask.get_size(); ++r)
    {
      os << row_init << "+" << "---";
      for (unsigned int c = 1; c < mask.get_size(); ++c)
        os << "+" << "---";
      os << "+" << std::endl
                << row_init << "|";
      for (unsigned int c = 0; c< mask.get_size(); ++c)
        os << std::setw(spaces) << mask(r,c) << " |";
      os << std::endl;
    }
  os << row_init;
  for (unsigned int c = 0; c < mask.get_size(); ++c)
    os << "+" << "---";
  os << "+" << std::endl;
  return os;
}