
/*
 * Entry point: load two labelled sequences from the example data files, build
 * the assignment problem and solve it with the Hungarian algorithm, printing
 * the intermediate steps and the optimal alignment.
 *
 * Author: Luca Ponzoni and Mauro Bardelloni
 * date: 2015-11-23
 * time: 18:34:45
 */

#include <utility>
#include "sequence_problem.h"

int main(int argc, char *argv[])
{
  SequenceProblem pb;
  pb.fill("../data/example_Q6.dat","../data/example_Q7.dat");
  pb.solve();
  return 0;
}
