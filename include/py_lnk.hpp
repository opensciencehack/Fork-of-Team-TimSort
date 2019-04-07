//
// Created by _as on 2019-04-07.
//
#pragma once

#include <vector>
#include <rapidjson/document.h>
#include "Polygon.hpp"

std::vector<Polygon> poly_extract(rapidjson::Document& doc, unsigned img_id);
std::unordered_map<unsigned, std::vector<Polygon>> poly_extract(rapidjson::Document& doc, const std::vector<unsigned>& img_ids);

float differ(const Polygon& a, const Polygon& b);