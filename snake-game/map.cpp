#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <sstream>
#include <string>
#include "map.h"

using namespace std;

std::string format_string(const std::string fmt, ...)
{
  int size = ((int)fmt.size()) * 2;
  std::string buffer;
  va_list ap;
  while (1)
  {
    buffer.resize(size);
    va_start(ap, fmt);
    int n = vsnprintf((char *)buffer.data(), size, fmt.c_str(), ap);
    va_end(ap);
    if (n > -1 && n < size)
    {
      buffer.resize(n);
      return buffer;
    }
    if (n > -1)
      size = n + 1;
    else
      size *= 2;
  }
  return buffer;
}

void mapArray::set_map(int map_arr[][21])
{
  ifstream inStream;
  string line;
  string filename = format_string("./map.txt");
  inStream.open(filename);

  getline(inStream, line);
  istringstream is(line);

  is >> maxwidth >> maxheight;

  for (int i = 0; getline(inStream, line); i++)
  {
    istringstream is(line);
    int n;

    int c = 0;
    while (is >> n)
    {
      map_arr[i][c++] = n;
    }
  }
  // for (int i = 0; i < 21; i++)
  // {
  //   for (int j = 0; j < 21; j++)
  //   {
  //     cout << map_arr[i][j] << ' ';
  //     // if (map_arr[i][j] == 1)
  //     // {
  //     //   cout << 1 << endl;
  //     // }
  //   }
  //   cout << endl;
  // }
}
