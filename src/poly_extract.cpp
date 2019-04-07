//
// Created by _as on 2019-04-06.
//
#include <iostream>
#include <unordered_map>
#include <vector>
#include <rapidjson/document.h>
#include "Polygon.hpp"
#include "algos.hpp"

constexpr auto segment = [](std::vector<Polygon>& polys, auto& ann){
  for (const auto& verts : ann["segmentation"].GetArray()){
    Polygon& poly = polys.emplace_back();
    const auto& v_arr = verts.GetArray();
    for (int i = 0; i < v_arr.Size(); i += 2) {
      auto& vert = poly.emplace_back();
      vert.position.x = v_arr[i].GetFloat();
      vert.position.y = v_arr[i + 1].GetFloat();
      //vert.color = sf::Color::Blue;
    }
  }
};

std::vector<Polygon> poly_extract(rapidjson::Document& doc, unsigned img_id){
  std::vector<Polygon> polys(32);

  // Search for annotations mentioning the image
  const auto& anns = doc["annotations"];

  for(const auto& ann : anns.GetArray()){
    if(!ann.HasMember("image_id"))
      continue;

    if(ann["image_id"].GetUint() != img_id)
      continue;

    switch(ann["iscrowd"].GetInt()){
      case 0:
        segment(polys, ann);
        break;
      case 1:
        break;
      default:
        std::cerr << "is_crowd flag is set to an unhandled value" << std::endl;
        std::exit(-1);
    }
  }
  return polys;
}

std::unordered_map<unsigned, std::vector<Polygon>> poly_extract(rapidjson::Document& doc, const std::vector<unsigned>& img_ids){
  std::unordered_map<unsigned, std::vector<Polygon>> ret{};
  ret.reserve(img_ids.size());
  std::for_each(img_ids.begin(), img_ids.end(), [&](unsigned u){ret.emplace(u, std::vector<Polygon>{});});

  // Search for annotations mentioning the image
  const auto& anns = doc["annotations"];

  for(const auto& ann : anns.GetArray()){
    if(!ann.HasMember("image_id"))
      continue;

    const unsigned curr_id = ann["image_id"].GetUint();

    if(!contains(img_ids, curr_id))
      continue;

    switch(ann["iscrowd"].GetInt()){
      case 0:
        segment(ret[curr_id], ann);
        break;
      case 1:
        break;
      default:
        std::cerr << "is_crowd flag is set to an unhandled value" << std::endl;
        std::exit(-1);
    }
  }
  return ret;
}