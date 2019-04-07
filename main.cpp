#include <iostream>
#include <fstream>
#include <filesystem>
#include <opencv/cv.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "BBox.hpp"
#include "Polygon.hpp"


void downloadFile(const std::string& Host, const std::string& RemoteFile, const std::string& LocalFile, unsigned short Port = 0){
  sf::Http http;
  sf::Http::Request request;
  unsigned int file_size = 0;

  http.setHost(Host, Port);
  request.setMethod(sf::Http::Request::Get);
  request.setUri(RemoteFile);

  sf::Http::Response Page = http.sendRequest(request);//Get the data

  file_size = Page.getBody().size();//Get the size that was returned

  const auto& fileContainer = Page.getBody();
  std::ofstream file(LocalFile.c_str(), std::ios::out | std::ios::binary);
  file.write(fileContainer.c_str(), file_size); //Write the file
  file.close();
}


int main(int argc, char** argv) {
  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " filename img_id\n";
    return -1;
  }

  const unsigned img_id = std::atoi(argv[2]);

  rapidjson::Document doc{};
  std::ifstream in_file{argv[1]};
  rapidjson::IStreamWrapper isw(in_file);
  doc.ParseStream(isw);

#if 0
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);

  std::cout << buffer.GetString() << std::endl;
#endif

  // Find if image id exists

  assert(doc.HasMember("images"));

  const auto& imgs = doc["images"].GetArray();

  const auto it = std::find_if(imgs.Begin(), imgs.End(), [=](auto& e){
    return (e["id"].GetInt() == img_id);
  });

  sf::Texture texture;

  if(it == imgs.End()){
    sf::String img_filename{(*it)["file_name"].GetString()};
    //texture.loadFromFile("/media/_as/SIMON USB/test_20k/" + img_filename);
    texture.loadFromFile(img_filename);
  } else {
    std::cerr << "Could not find image id in file\n" << std::flush;
    std::string url = (*it)["coco_url"].GetString();
    std::cout << "Attempting to download: " << url << std::endl;
    unsigned endBase = url.find('/', 7); //Jump over the "http://" and search for another "/"
    std::string baseURL = url.substr(0, endBase);
    std::string page = url.substr(endBase + 1);
    downloadFile(baseURL, page, "temp.jpg", 80);
    texture.loadFromFile("temp.jpg");
  }

  // Search for annotations mentioning the image

  assert(doc.HasMember("annotations"));

  const auto& anns = doc["annotations"];

  std::vector<BBox> boxes(128);
  std::vector<Polygon> polys(128);


  for(const auto& ann : anns.GetArray()){
    if(!ann.HasMember("image_id"))
      continue;

    if(ann["image_id"].GetUint() != img_id)
      continue;

    const auto& box = ann["bbox"].GetArray();

    boxes.push_back(BBox{static_cast<unsigned int>(box[0].GetFloat()), static_cast<unsigned int>(box[1].GetFloat()), static_cast<unsigned int>(box[2].GetFloat()), static_cast<uint>(box[3].GetFloat())});

    switch(ann["iscrowd"].GetInt()){
      case 0:
        for (const auto& verts : ann["segmentation"].GetArray()){
          Polygon& poly = polys.emplace_back();
          const auto& v_arr = verts.GetArray();
          for (int i = 0; i < v_arr.Size(); i += 2) {
            auto& vert = poly.emplace_back();
            vert.position.x = v_arr[i].GetFloat();
            vert.position.y = v_arr[i + 1].GetFloat();
            vert.color = sf::Color::Blue;
          }
        }
        break;
      case 1:
        break;
      default:
        std::cerr << "is_crowd flag is set to an unhandled value" << std::endl;
        return -1;
    }
  }

  const auto img_size = texture.getSize();
  sf::RenderWindow window(sf::VideoMode{img_size.x, img_size.y}, "Demo");
  sf::Sprite sprite;
  sprite.setTexture(texture, true);

  window.draw(sprite);

  {
    auto rect = sf::RectangleShape{};
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(3);
    for(const auto& box : boxes){
      rect.setSize(sf::Vector2f{static_cast<float>(box.w), static_cast<float>(box.h)});
      rect.setPosition(box.x, box.y);
      window.draw(rect);
    }
  }

  for(const auto& poly : polys)
    window.draw(poly.data(), poly.size(), sf::LineStrip);

  window.display();
  std::cin.get(); // avoids closing everything

  window.close();
  //std::filesystem::remove("temp.jpg");
  //std::system("rm temp.jpg");
  return 0;
}
