#include "config.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

Config::Config() {
  ;
}

Config::Config(string& name) {
  this->fileName = name;
}
