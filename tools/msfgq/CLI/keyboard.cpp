/*
	Copyright (C) 2021 Claude SIMON (http://q37.info/contact/).

	This file is part of the 'mscfdraftq' tool.

    'mscfdraftq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'mscfdraftq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'mscfdraftq'.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "keyboard.h"

# include "stsfsm.h"

#ifdef CPE_S_WIN
# include <conio.h>
#elif defined( CPE_S_POSIX )
# include <termios.h>
#else
# error "Unhandled operating system!"
#endif

using namespace keyboard;

namespace {
#ifdef CPE_S_WIN
  int getch_( void ) {
      return _getch();
  }
#elif defined(CPE_S_POSIX)
  int getch_(void)
  {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
  }
#else
# error
#endif
}
namespace {
  namespace {
    stsfsm::wAutomat Automat_;
    stsfsm::sParser Parser_;

    namespace seq_ {
      const char Up[] = {91, 65, 0};
      const char Down[] = {91, 66, 0};
      const char Right[] = {91, 67, 0};
      const char Left[] = {91, 68, 0};
      const char End[] = {91, 70, 0};
      const char Begin[] = {91, 72, 0};
      const char Delete[] = {91, 51, 126, 0};
      const char PageUp[] = {91, 53, 126, 0};
      const char PageDown[] = {91, 54, 126, 0};
    }

#define K(name) {c##name, seq_::name}

    struct {
      int Id;
      const char *Tag;
    } Keys_ [] = {
      K(Up),
      K(Down),
      K(Right),
      K(Left),
      K(End),
      K(Begin),
      K(Delete),
      K(PageUp),
      K(PageDown),
    };
#undef K
  }

  void SetAutomat_(void) {
    Automat_.Init();

    for ( int i = 0; i < sizeof(Keys_) / sizeof(Keys_[0] ); i++)
      stsfsm::Add(Keys_[i].Tag, Keys_[i].Id, Automat_);

    Parser_.Init(Automat_);
  }

  stsfsm::eStatus GetStatus_(int C) {
    return Parser_.Handle(C);
  }

  int GetCode_(void) {
    return Parser_.GetId();
  }

  void Reset_(void) {
    Parser_.Reset();
  }
}

bso::sUInt keyboard::GetCode(void)
{
  bso::sUInt KeyCode = getch_();
#ifdef CPE_S_WINDOWS
  if ( KeyCode == 0 || KeyCode == 224 )
    KeyCode = 256 + getch_();
#elif defined(CPE_S_POSIX)
  if ( KeyCode == 127 )
    KeyCode = cBack;
  else if ( KeyCode == 27 ) {
    stsfsm::eStatus Status = stsfsm::s_Undefined;

    while ( ( Status = GetStatus_(getch_() ) ) == stsfsm::sPending );

    if ( Status == stsfsm::sMatch )
      KeyCode = GetCode_();
    else
      KeyCode = 0;

    Reset_();
  }
#endif // defined

  return KeyCode;
}

qGCTOR(keyboard) {
  SetAutomat_();
}

