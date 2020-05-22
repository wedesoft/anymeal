#include <iostream>
#include <fstream>
#include <sstream>
#include "partition.hh"
#include "mealmaster.hh"
#include "recode.hh"
#include "database.hh"


using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Syntax: " << argv[0] << " [filename] ..." << endl;
    return 1;
  };
  try {
    Recoder recoder("latin1..utf8");
    Database database;
    database.open("anymeal.db");
    for (int i=1; i<argc; i++) {
      database.begin();
      ifstream f(argv[i]);
      auto lst = recipes(f);
      for (auto recipe=lst.begin(); recipe!=lst.end(); recipe++) {
        istringstream s(*recipe);
        try {
          auto result = parse_mealmaster(s);
          cerr << "Parsed recipe: " << result.title() << endl;
          auto recoded = recoder.process_recipe(result);
          database.insert_recipe(recoded);
        } catch (parse_exception &e) {
          cout << "Rejected recipe: " << e.what() << "\r\n";
          cout << *recipe;
        };
      };
      database.commit();
    };
  } catch (exception &e) {
    cerr << "An error occurred: " << e.what() << endl;
  };
  return 0;
}
