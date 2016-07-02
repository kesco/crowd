#include "file.hpp"
#include "error.hpp"

#include <fstream>

using namespace std;

namespace {
  inline ofstream _erase_file(const std::string &file_path) {
    ofstream ofs;
    ofs.open(file_path, std::ofstream::out | std::ofstream::trunc);
    if (ofs.fail()) {
      throw crowd::IOException("Can not write the file");
    }
    return ofs;
  }
}

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

  void erase_file(const std::string &file_path) {
    try {
      ofstream ofs = _erase_file(file_path);
      ofs.close();
    } catch (IOException &ex) {
      throw;
    }
  }

  void string_to_file(const std::string &path, const std::string &content,
                      bool erase) {
    if (erase) {
      try {
        ofstream ofs = _erase_file(path);
        ofs << content;
        ofs.close();
      } catch (IOException &ex) {
        throw;
      }
    } else {
      ofstream ofs(path);
      if (!ofs.is_open()) {
        throw crowd::IOException("Can not write the file");
      }
      ofs << content;
    }
  }
}


