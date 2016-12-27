#include "clipboard.h"
#include <io.h>
#include <fcntl.h>
#include <getopt.h>
#include <map>

const char CR = '\r';
const char LF = '\n';

void usage();
void crlf2lf(std::string &targetStr);


int main(int argc, char * const argv[])
{
  std::map<char, bool> optflag;
  optflag['i'] = false;
  optflag['o'] = false;
  optflag['l'] = false;
  optflag['c'] = false;

  struct option longopts[] = {
    {"lf",   no_argument, NULL, 'l'},
    {"crlf", no_argument, NULL, 'c'},
    {0,      0,           0,     0 },
  };

  int opt;
  int longindex;
  while ((opt = getopt_long(argc, argv, "io", longopts, &longindex)) != -1) {
    switch (opt) {
      case 'i':
        optflag['i'] = true;
        break;
      case 'o':
        optflag['o'] = true;
        break;
      case 'l':
        optflag['l'] = true;
        break;
      case 'c':
        optflag['c'] = true;
        break;
      default:
        usage();
        return 1;
    }
  }

  if ((optflag['i'] && optflag['o'])
      || (!optflag['i'] && !optflag['o'])
      || (optflag['i'] && optflag['l'])
      || (optflag['o'] && optflag['c'])) {
    usage();
    return 1;
  }

  Clipboard clipboard;
  std::string str;
  std::stringstream ss;

  if (optflag['o']) {
    ss << clipboard;
    str = ss.str();
    if (optflag['l']) {
      crlf2lf(str);
    }
    _setmode(_fileno(stdout), _O_BINARY);
    std::cout.write(str.c_str(), str.length());
  }
  else {
    bool init = true;

    if (optflag['c']) {
      ss << CR;
    }
    ss << LF;
    clipboard.setNewLine(ss.str());
    ss.str("");

    while (std::getline(std::cin, str)) {
      if (!init) {
        ss << "\n";
      }
      ss << str;
      init = false;
    }
    str = ss.str();
    std::stringstream(str) >> clipboard;
  }

  return 0;
}

void usage() {
  std::cerr << "win32yank" << std::endl << std::endl
    << "Usage:" << std::endl
    << "    win32yank -o [--lf]" << std::endl
    << "    win32yank -i [--crlf]" << std::endl << std::endl
    << "Options:" << std::endl
    << "    -o         Print clipboard content to stdout" << std::endl
    << "    -i         Set clipboard from stdin" << std::endl
    << "    --lf       Replace CRLF with  LF before printing to stdout" << std::endl
    << "    --crlf     Replace lone LF bytes with CRLF before setting the clipboard" << std::endl;
}

void crlf2lf(std::string &targetStr) {
  std::string destStr;
  for (std::string::const_iterator it = targetStr.begin();
      it != targetStr.end(); ++it) {
    try {
      if (*it == CR && *(it + 1) == LF) {
        ++it;
      }
    }
    catch (std::out_of_range) {
    }
    destStr += *it;
  }
  targetStr = destStr;
}
