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
  for (int i=1; i<argc; i++) {
    ifstream f(argv[i]);
    auto lst = recipes(f);
    for (auto recipe=lst.begin(); recipe!=lst.end(); recipe++) {
      istringstream s(*recipe);
      try {
        auto result = parse_mealmaster(s);
        cout << "Good recipe: " << result.title() << endl;
      } catch (parse_exception &e) {
        cout << e.what() << endl;
        cout << *recipe;
      };
    };
  };
  return 0;
}
