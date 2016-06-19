#include "file.hpp"
#include "error.hpp"

using namespace std;
namespace crowd {
  string string_from_file(const string &file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
      throw IOException("Can not read the file.");
    }
    string ret;
    string line;
    while (getline(file, line)) {
      ret += line;
      ret.push_back('\n');
    }
    return ret;
  }
}


