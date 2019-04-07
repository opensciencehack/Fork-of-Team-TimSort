
#include <fstream>
#include <unordered_map>
#include <vector>
#include <rapidjson/istreamwrapper.h>
#include "cats_reduce.hpp"
#include "py_lnk.hpp"

int main(int argc, char** argv){

  rapidjson::Document doc{};
  std::ifstream in_file{argv[1]};
  rapidjson::IStreamWrapper isw(in_file);
  doc.ParseStream(isw);

  std::vector<unsigned> ids;
  std::unordered_map<unsigned, std::vector<Polygon>> polys = poly_extract(doc, ids);
  //std::unordered_map<std::string, Polygon> cat_poly = cats_reduce();
  return 0;
}