#pragma once

#include <boost/filesystem.hpp>

#include <utility>

namespace crowd {

  /* 目前只支持build命令 */
  enum class COMMAND {
    BUILD, SERVER, INIT, UNSUPPORT
  };


  struct CLIApp {
    CLIApp();

    virtual ~CLIApp() { }

    void build(boost::filesystem::path &);

  private:
    boost::filesystem::path _temp_dir;
  };

  std::pair<COMMAND, boost::filesystem::path> parse_cli_option(int argc,
                                                               char **argv);
}
