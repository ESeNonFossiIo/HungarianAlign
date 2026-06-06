/*
 * Description
 * Author: Luca Ponzoni and Mauro Bardelloni
 * date: 2015-11-23
 * time: 18:34:45
 * test: matrix, output and tracs
 */

#include "matrix.h"
#include "utilities.h"

int main(int argc, char *argv[])
{
  using namespace utilities;
  print_msg("-> Create a permutation of 4 elements:");
  Permutation id(4);
  std::cout << id
            << std::endl;

  print_msg("-> Create a matrix 2x2 and two elements in position (2,1) and one in position (4,4):");
  Matrix<> m(2,2);
  std::cout << m;
  m.status();

  m.add_one(1,1);
  m.add_one(1,1);
  m.add_one(3,3);

  std::cout << m;
  m.status();

  print_msg("-> Add one in position (1,4) and 2 in (3,4):");
  m.add_one(0,3);
  m.add_one(2,3);
  m.add_one(2,3);

  std::cout << m;
  m.status();
}
