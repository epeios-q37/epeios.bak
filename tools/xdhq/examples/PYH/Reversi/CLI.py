#!/usr/bin/env python3
"""
MIT License

Copyright (c) 2017,2020 Hajime Nakagami <nakagami@gmail.com>
Copyright (c) 2019,2021 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from core import *

BLACK_MARK = 'X'
WHITE_MARK = 'O'


def print_board(reversi):
  print('\n   a b c d e f g h \n  +-+-+-+-+-+-+-+-+')
  for i, row in enumerate(reversi.board):
    print(' %d|' % (i + 1), end='')
    for r in row:
      print('{}|'.format({
        EMPTY: ' ',
        BLACK: BLACK_MARK,
        WHITE: WHITE_MARK
      }[r]),
          end='')
    print('\n  +-+-+-+-+-+-+-+-+')
  print()


def input_position(player):
  while True:
    s = input('{}? [a-h][1-8]'.format(BLACK_MARK if player ==
                      BLACK else WHITE_MARK))
    if s == '' or (len(s) == 2 and s[0] in list('abcdefgh')
             and s[1] in list('12345678')):
      break
  if s == '':
    return None

  y, x = ord(s[0]) - 97, ord(s[1]) - 49
  #print('input_position=', x, y)
  return x, y


def print_position(player, xy):
  if xy is None:
    print('{}: skip'.format(BLACK_MARK if player == BLACK else WHITE_MARK))
  else:
    print('{}: {}{}'.format(BLACK_MARK if player == BLACK else WHITE_MARK,
                chr(xy[1] + 97), chr(xy[0] + 49)))


reversi = None
humans = []

def humanTurn(player):
  xy = input_position(player)
  while xy and not reversi.put(xy[0], xy[1], player):
      xy = input_position(player)

def computerTurn(player):
  xy = reversi.find_best_position(player)
  if xy:
    reversi.put(xy[0], xy[1], player)
  print_position(player, xy)

def turn(player):
  if player in humans:
    humanTurn(player)
  else:
    computerTurn(player)
  print_board(reversi)
  if not humans:
    input("Hit Enter key to continue.")

TYPES = {
  'a': [BLACK],
  'b': [WHITE],
  'c': [BLACK, WHITE],
  'd': []
}  

def getHumans():
  global humans
  answer = ""

  while answer.lower() not in TYPES:
    print("a: 'X' human, 'O' computer")
    print("b: 'O' computer, 'X' human")
    print("c: 'X' human, 'O' human")
    print("d: 'X' computer, 'O' computer")
    answer = input("Please choose ('X' begins): ")

  humans = TYPES[answer]

def start_game():
  global reversi
  reversi = Reversi(1)

  getHumans()

  print_board(reversi)

  while not (reversi.count(EMPTY) == 0 or reversi.count(BLACK) == 0
         or reversi.count(WHITE) == 0):
    turn(BLACK)
    turn(WHITE)

  print_board(reversi)
  print(f"'X': {reversi.count(BLACK)}, 'O': {reversi.count(WHITE)}")
  if reversi.count(BLACK) > reversi.count(WHITE):
    print("'X' wins!")
  elif reversi.count(BLACK) < reversi.count(WHITE):
    print("'Y' wins!")
  else:
    print('Equality)')

if __name__ == "__main__":
  start_game()
