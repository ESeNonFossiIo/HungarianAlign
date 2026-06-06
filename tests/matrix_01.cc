/*
 * Description
 * Author: Luca Ponzoni and Mauro Bardelloni
 * date: 2015-11-23
 * time: 18:34:45
 * test: copy operator
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

  print_msg("-> Create a matrix 2x2:");
  Matrix<> m(2,2);
  m.add_one(1,1);
  m.add_one(1,1);
  m.add_one(0,1);
  m.add_one(1,1);
  m.add_one(0,0);
  m.add_one(0,0);
  m.add_one(0,0);
  m.add_one(0,0);

  std::cout << m;
  m.status();

  print_msg("-> Copy operator:");
  Matrix<> n(2,2);
  std::cout << n;
  n.status();
  n=m;
  std::cout << n;
  n.status();

  print_msg("-> Modify the first matrix:");
  m.add_one(0,0);
  m.add_one(0,0);
  std::cout << m;
  m.status();
  std::cout << n;
  n.status();


}
