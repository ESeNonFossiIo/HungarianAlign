# sequence

Align two labelled sequences by modelling the problem as an **assignment
problem** and solving it with the **Hungarian algorithm**.

Each element of the two input sequences is a label. Every observed pairing of a
row label with a coloumn label increments the corresponding entry of a cost
matrix; solving the problem means finding the permutation of rows and coloumns
that maximises the trace of that matrix, i.e. the best overall alignment.

## How it works

- `SequenceProblem` reads the two sequences and fills a Hungarian cost matrix.
- `HMatrix` turns the maximise-trace problem into its minimise-cost form (the
  complement matrix), then reduces rows and coloumns and covers the zeros until
  an optimal assignment is reachable.
- `Mask`, `Permutation` / `LPermutation` and the `utilities` namespace provide
  the supporting bookkeeping (covering lines, label/index mapping, helpers).

The executable prints every intermediate step (original matrix, complement,
row/coloumn reductions) and the final optimal assignment.

## Requirements

- A C++11 compiler
- CMake (>= 3.10)
- Doxygen (optional, only to build the documentation)

## Build and run

```
mkdir build
cd build
cmake ..
make
make run
```

By default the program aligns the two example sequences in `data/`
(`example_Q6.dat` and `example_Q7.dat`).

## Test

```
mkdir build
cd build
cmake .. -DENABLE_TESTS=ON
make
ctest -V
```

### Run a specific test:

```
make run_**name-of-the-test**
```

## Documentation

```
mkdir build
cd build
make doc
open ../doc/html/index.html
```

## Input format

Each `.dat` file lists one entry per line as `index label`, for example:

```
1 1
2 1
3 1
```

The labels of the first and second file are paired entry by entry until one of
the two files is exhausted.
