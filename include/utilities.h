
/*
 * Authors: Luca Ponzoni and Mauro Bardelloni
 * date: 2015-11-23
 * time: 18:34:45
 */

#ifndef __utilities__
#define __utilities__

#include <vector>
#include <map>

namespace utilities
{
  /**
   *  Returns elapsed seconds past from the last call to timer rest.
   */
  double cclock();

  /**
   * Uniform the output of the message @p msg.
   * Version for a one line message.
   */
  void print_msg(std::string msg);

  /**
   * Uniform the output of the message @p msg.
   * Version for multi line message. Each line is an element of @p msg.
   */
  void print_msg(std::vector<std::string> msg);

  /**
   * Uniform the output of @p vector sequences.
   * @p space set the space reserved for each entry.
   */
  template <typename T>
  void print(std::vector<T> vec, unsigned int space = 3);

  // /**
  //  * Uniform the output of @p maps.
  //  * @p space set the space reserved for each entry.
  //  */
  // template <typename T,typename S>
  // void print_map(std::map<T,S> map, unsigned int space = 3);

  /**
   * Return the minimum value in a std::vector and the position
   * of the first entry with the value of the minimum.
   */
  std::pair<unsigned int, int>
  min(std::vector<unsigned int> vec,
      unsigned int lower_bound = 0,
      unsigned int upper_bound = 99);

  /**
   * Return the maximum value in a std::vector and the position
   * of the first entry with the value of the maximum.
   */
  std::pair<unsigned int, unsigned int>
  max(std::vector<unsigned int> vec);

  /**
   * Return the dimensions of @p mat as a pair: first is the number of rows,
   * second is the number of coloumns (taken from the first row).
   */
  std::pair<unsigned int, unsigned int>
  size_row_and_col(std::vector<std::vector<unsigned int>> mat);
}

#endif
