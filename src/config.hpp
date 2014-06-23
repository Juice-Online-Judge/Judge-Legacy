#ifndef CONFIG_HPP_INCLUDE
#define CONFIG_HPP_INCLUDE

#include <string>
#include <boost/property_tree/ptree.hpp>

class Config {
  public:
    Config();
    Config(std::string&);
    template <typename T>
    T get(std::string&);
    std::string get(std::string&);
    void load(std::string&);
  private:
    std::string fileName;
    boost::property_tree::ptree root;
};

#endif /* end of include guard: CONFIG_HPP_INCLUDE */

