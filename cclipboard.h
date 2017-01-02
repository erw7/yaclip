#ifndef _CCLIPBOARD_H
#define _CCLIPBOARD_H

#define UNICODE
#define _UNICODE

#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iterator>
#include <windows.h>

class Cclipboard
{
};

std::ostream &operator<<(std::ostream &out, const Cclipboard &tgt);
std::istream &operator>>(std::istream &in, Cclipboard &tgt);

#endif
