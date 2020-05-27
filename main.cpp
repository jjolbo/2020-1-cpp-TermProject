#include "main.h"
#include <iostream>
using namespace std;
#define LEVEL 11;

int main()
{
  // 0 : USER
  MODE command = init_game();

  if (command == USER)
  {
    cout << 1 << endl;
    // user_mode_game(LEVEL);
  }

  return 0;
}