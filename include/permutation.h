#ifndef __permutation_h__
#define __permutation_h__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>

/**
 * Permutation of @p num elements, stored as a sequence of indices that starts
 * as the identity (0, 1, ..., num-1) and is rearranged through
 * switch_elements().
 */
class Permutation
{
public:
  /**
   * Build the identity permutation over @p num elements.
   */
  Permutation(unsigned int num);

  /**
   * Build a permutation of @p num elements from an explicit @p sequence of
   * indices.
   */
  Permutation( unsigned int num,
               std::vector<unsigned int> sequence );

  /**
   * Copy constructor.
   */
  Permutation(const Permutation &p);

  /**
   * Add an element to the permutation.
   */
  void new_element();

  /**
   * Switch element @p i and element @p j.
   */
  void switch_elements(unsigned int i, unsigned int j);

  /**
   *  Status of the matrix:
   *  - Number of elements
   */
  void
  status();

  /**
   *  Overload of the output operator <<.
   *  It is possible to check the ouput of the permutations in the following way:
   *  @code{.cpp}
   *   Permutation p;
   *   ...
   *   std::cout << p;
   *  @endcode
   */
  friend std::ostream &operator<< (std::ostream &os, Permutation &dt);

  /**
   * Access to the element @p i.
   */
  unsigned int &operator()(const unsigned int i);
  unsigned int &operator[](const unsigned int i);

protected:
  /**
   * Total number of elements.
   */
  unsigned int tot;

  /**
   * Current permutation.
   */
  std::vector<unsigned int> sequence;
};

/**
 * LPermutation stands for labeled permutation. It adds to
 * the Permutation class a map of labels.
 */
class LPermutation : public Permutation
{
public:
  /**
   * Build an empty labeled permutation (no elements, no labels).
   */
  LPermutation();

  /**
   * Copy constructor.
   */
  LPermutation(const LPermutation &p);

  /**
   * Add an element @p element of type char to the sequence.
   * If this label exists the function return false, otherwise
   * true.
   */
  bool add_an_element(char element, bool output=false);

  /**
   *  Overload of the output operator <<.
   *  It is possible to check the ouput of the permutations in the following way:
   *  @code{.cpp}
   *   LPermutation p;
   *   ...
   *   std::cout << p;
   *  @endcode
   */
  friend std::ostream &operator<< (std::ostream &os,
                                   LPermutation &dt);

  /**
   * Return the label associated with index @p i.
   */
  char
  get_char(unsigned int i);

  /**
   * Return the index associated with label @p c (inverse of get_char()).
   */
  unsigned int
  get_int(char c);

private:
  /**
   * Associate to each number of the sequence an element.
   */
  std::map<unsigned int, char> from_int_to_char;

  /**
   * Associate to each element the linked number.
   * This map is a kind of inverse of @p sequence_map.
   */
  std::map<char, unsigned int> from_char_to_int;
};

#endif
