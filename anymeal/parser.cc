#include <iostream>
#include <fstream>
#include <sstream>
#include "mealmaster.hh"
#include "partition.hh"


using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Syntax: " << argv[0] << " [filename] ..." << endl;
    return 1;
  };
  cout << "Processing " << argc << " files\r\n";
  for (int i=1; i<argc; i++) {
    ifstream f(argv[i]);
    auto lst = recipes(f);
    cout << "File " << argv[i] << " has " << lst.size() << " recipes\r\n";
    for (auto recipe=lst.begin(); recipe!=lst.end(); recipe++) {
      istringstream s(*recipe);
      try {
        auto result = parse_mealmaster(s);
        cout << "Parsed recipe: " << result.title() << "\r\n";
      } catch (parse_exception &e) {
        cout << "Rejected recipe" << "\r\n";
        cout << e.what() << endl;
        cout << *recipe;
      };
    };
  };
  return 0;
}
