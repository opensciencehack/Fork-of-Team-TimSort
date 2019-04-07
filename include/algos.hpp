#pragma once

#include <algorithm>

template <class T, class Container>
bool contains(Container c, T v){
  return std::find(c.begin(), c.end(), v) != c.end();
}