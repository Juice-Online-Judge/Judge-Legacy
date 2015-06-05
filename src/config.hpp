#ifndef CONFIG_HPP_INCLUDE
#define CONFIG_HPP_INCLUDE

#include <string>

struct Impl;

class Config {
public:
  Config();
  Config(const std::string&);
  template <typename T>
  T get(const std::string&);
  std::string get(const std::string&);
  void load_config(const std::string&);
  const std::string& get_filename() const;
private:
  void load_json(void);
  Impl *self;
};

#endif /* end of include guard: CONFIG_HPP_INCLUDE */


