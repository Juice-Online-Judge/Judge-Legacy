#include "config.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

struct Impl {
  std::string filename;
  boost::property_tree::ptree root;
};

Config::Config() {
  self = new Impl;
}

Config::Config(const string& name) : Config() {
  self->filename = name;
}

void Config::load_config(const string& filename) {
  self->filename = filename;
  load_json();
}

const string& Config::get_filename() const {
  return self->filename;
}

void Config::load_json() {
}
