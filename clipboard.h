#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H

#define UNICODE
#define _UNICODE

#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iterator>
#include <windows.h>

class Clipboard
{
};

std::ostream &operator<<(std::ostream &out, const Clipboard &tgt);
std::istream &operator>>(std::istream &in, Clipboard &tgt);

#endif
