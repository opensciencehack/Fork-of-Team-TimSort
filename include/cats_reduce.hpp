#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Polygon.hpp"


Polygon cat_reduce(std::vector<unsigned> ids, std::unordered_map<unsigned, std::vector<Polygon>> polys);
std::unordered_map<std::string, Polygon> cats_reduce(std::unordered_map<std::string, std::vector<unsigned>> cats, std::unordered_map<unsigned, std::vector<Polygon>> polys);