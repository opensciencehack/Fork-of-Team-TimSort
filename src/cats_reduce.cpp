#include <numeric>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "Polygon.hpp"

Polygon cat_reduce(std::vector<unsigned> ids, std::unordered_map<unsigned, std::vector<Polygon>> polys){
  using BoostPoly = boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<float>>;
  BoostPoly a, b;
  std::vector<float> out;
  for (const auto& id : ids){
    const auto& poly_vec = polys[id];
    for (const auto& poly : poly_vec) {
      for (int i = 0; i < poly.size(); i += 2) {
        boost::geometry::append(a.inners(), boost::geometry::model::d2::point_xy{poly[i], poly[i + 1]});
      }
    }
    boost::geometry::intersection(a, b, out);
  }
  //std::accumulate(, , [](){boost::geometry::intersection(, , out);});
}

std::unordered_map<std::string, Polygon> cats_reduce(std::unordered_map<std::string, std::vector<unsigned>> cats, std::unordered_map<unsigned, std::vector<Polygon>> polys){
  std::unordered_map<std::string, Polygon> ret;
  std::transform(cats.begin(), cats.end(), std::inserter(ret, ret.begin()), [](const auto& e){
    const auto& [name, ids] = e;
    return std::make_pair(name, cat_reduce(ids));
  });
  return ret;
}