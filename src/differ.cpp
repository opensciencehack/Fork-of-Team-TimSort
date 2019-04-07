#include <memory>
#include <opencv2/shape.hpp>
#include "Polygon.hpp"


float differ(const Polygon& a, const Polygon& b){
  auto ctx = cv::createShapeContextDistanceExtractor();
  std::vector<cv::Point2f> av;
  std::transform(a.begin(), a.end(), std::back_inserter(av), [](const auto& e) -> cv::Point2f {return {e.position.x, e.position.y};});
  std::vector<cv::Point2f> bv;
  std::transform(a.begin(), a.end(), std::back_inserter(bv), [](const auto& e) -> cv::Point2f {return {e.position.x, e.position.y};});
  cv::InputArray ia_a{av}, ia_b{bv};
  return ctx->computeDistance(ia_a, ia_b);
}