//
// Created by _as on 2019-04-06.
//

// Wrapper code

#include <fstream>
#include <memory>
#include <string>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/istreamwrapper.h>
#include "py_lnk.hpp"

using namespace boost::python;

class Wrapper {
  std::shared_ptr<rapidjson::Document> doc{};
  std::vector<Polygon> vec;
public:
  Wrapper(){
    doc = std::make_shared<rapidjson::Document>();
  }
  Wrapper(const Wrapper&) = default;
  Wrapper(Wrapper&&) = default;
  void ld_file(const char* fpath){
    std::ifstream in_file{fpath};
    rapidjson::IStreamWrapper isw(in_file);
    doc->ParseStream(isw);
  }
  void extract(unsigned u){
    vec = poly_extract(*doc, u);
    vec.erase(std::remove(vec.begin(), vec.end(), std::vector<Vertex>{}));
  }
  float diff(unsigned a, unsigned b){
    return differ(vec[a], vec[b]);
  }
};



BOOST_PYTHON_MODULE(pyExDiff){
  class_<sf::Vertex>("Vertex")
      .def("x", +[](sf::Vertex* v){return v->position.x;})
      .def("x", +[](sf::Vertex* v, float val){v->position.x = val;})
      .def("y", +[](sf::Vertex* v){return v->position.y;})
      .def("y", +[](sf::Vertex* v, float val){v->position.y = val;});
  class_<Polygon>("Polygon")
      .def(vector_indexing_suite<std::vector<float>>());
  class_<std::vector<Polygon>>("PolygonVec")
      .def(vector_indexing_suite<std::vector<Polygon>>());
  class_<Wrapper>("Wrap")
      .def("ldfile", &Wrapper::ld_file)
      .def("extract", &Wrapper::extract)
      .def("diff", &Wrapper::diff);
}