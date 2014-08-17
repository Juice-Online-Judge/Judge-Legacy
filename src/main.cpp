#include <iostream>

#include <boost/program_options.hpp>

#include "logger.hpp"
#include "execute.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  namespace po = boost::program_options;
  int sec, mem;
  string ques, path;
  po::options_description desc("Options");
  loggerInit(argv);
  desc.add_options()
    ("help,h", "Show help messages")
    ("time,t", po::value<int>(&sec)->required(), "Time limit(Sec)")
    ("memory,m", po::value<int>(&mem)->required(), "Memory Limit(MB)")
    ("question,q", po::value<string>()->required(), "Question Name")
    ("exec-file,e", po::value<string>()->required(), "Execution file");
  po::positional_options_description posDesc;
  posDesc.add("question", 1);
  posDesc.add("exec-file", 2);
  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).positional(posDesc).run(), vm);
    if(vm.count("help")) {
      cout << desc << endl;
      return 0;
    }
    po::notify(vm);
  }
  catch(po::required_option& e) {
    cerr << e.what() << endl;
    cerr << desc << endl;
    return 2;
  }
  catch(po::error& e) {
    cerr << e.what() << endl;
    cerr << desc << endl;
    return 2;
  }
  ques = vm["question"].as<string>();
  path = vm["exec-file"].as<string>();
  return execute(ques, path, sec, mem);
}
