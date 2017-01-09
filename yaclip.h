#ifndef _YACLIP_H
#define _YACLIP_H

#include "clipboard.h"
#include <map>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/newline.hpp>
#include <boost/scoped_ptr.hpp>
#include <io.h>
#include <fcntl.h>
#include <getopt.h>

#define DEFAULT_ENCODING "UTF-8"
#define VERSION "0.2.0"

void usage();
void version();
void err_exit(std::string to, std::string from);

namespace io = boost::iostreams;

#endif
