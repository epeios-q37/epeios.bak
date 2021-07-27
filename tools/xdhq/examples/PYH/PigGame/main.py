#!/usr/bin/env python3
"""
MIT License

Copyright (c) 2021 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.extend(["../../atlastk", "."])

import atlastk, random, threading, time, urllib, uuid

DEBUG = True  # Uncomment for debug mode.

LIMIT = 100

lock = threading.Lock()
games = {}

class Game:
  def __init__(self):
    self.current = 1 # 1 or 2

    self.available = 1 # 0 (no more player available), 1 or 2

    self.scores = {
      1: 0,
      2: 0
    }

    self.turn = 0  
    self.dice = 0


class User:
  def __init__(self):
    self._player = 0
    self._game = None 
    self.token = None # if None, opponent is the computer.

  def __del__(self):
    if self.token:
      remove_game(self.token, self._player)

  def init(self, token = None):
    if self.token:
      remove_game(self.token)

    self.token = token
    self._game = None if token else Game()  
    self.player_ = 0

  def get_game(self):
    if self.token:
      return get_game(self.token)
    else:
      return self._game

  def get_raw_player(self):
    return self._player

  def get_player(self):
    if self._player == 0:
      if self.token:
        self._player = get_game_available_player(self.token)
      else:
        self._player = get_available_player(self._game)
      
    return self._player


def create_game(token):
  global lock, games
  assert token, token not in games

  game = Game()  

  with lock:
    games[token] = game

  print("Create: ", token)

  return game
  

def remove_game(token, player):
  global lock, games
  assert token

  with lock:
    if token in games:
      game = games[token]

      if game.available == 1 or player != 0:
        del games[token]
        print("Remove: ", token)


def get_game(token):
  global lock

  with lock:
    if token in games:
      return games[token]
    else:
      return None:


def get_available_player(game):
  player = 0

  if game.available:
    player = game.available

  if game.available == 1:
    game.available = 2
  else:
    game.available = 0

  return player


def get_game_available_player(token):
  global lock

  with lock:
    if token in games:
      return get_available_player(games[token])
    else:
      return 0


def debug():
  try:
    return DEBUG
  except:
    return False


def fade(dom, element):
  dom.remove_class(element, "fade-in")
  dom.flush()
  dom.add_class(element, "fade-in")


def update_meter(dom, ab, score, turn):
  if score + turn > LIMIT:
    turn = LIMIT - current

  fade(dom, f"TurnMeter{ab}")
  dom.set_attribute(f"TurnMeter{ab}", "style", f"width: {turn}%;")

  dom.set_attribute(f"ScoreMeter{ab}", "style", f"width: {score}%;")

  dom.set_content(f"ScoreText{ab}", score)


def disable_play_buttons(dom):
  dom.disable_elements(["Roll", "Hold"])


def enable_play_buttons(dom):
  dom.enable_elements(["Roll", "Hold"])


def get_opponent(player_ab):
  if player_ab == 'A':
    return 'B'
  elif player_ab == 'B':
    return 'A'
  elif player_ab == 1:
    return 2
  else:
    return 1


def mark_player(dom, ab):
  if ab == 'B':
    dom.disable_element("DisplayMarkerA")
  else:
    dom.enable_element("DisplayMarkerA")


def display_dice(dom, value):
  fade(dom, "dice")

  if value <= 6:
    filename = f"dice-{value}.svg" if value != 0 else "Pig.svg"
    dom.inner("dice", open(filename).read())


def is_my_turn(game, player):
  if player != 0:
    return player == game.current
  elif game.available != 0:
    return game.available == game.current
  else:
    return None


def update_meters(dom, game, player):
  my_turn = is_my_turn(game, player)

  if my_turn is None:
    a, b = 1, 2
    turn_A = game.turn if game.current == 1 else 0
    turn_B = game.turn if game.current == 2 else 0
  else:  
    a = game.current if my_turn else get_opponent(game.current)
    b = get_opponent(a)
    turn_A = game.turn if my_turn else 0
    turn_B =  0 if my_turn else game.turn

  update_meter(dom, 'A', game.scores[a], turn_A)
  update_meter(dom, 'B', game.scores[b], turn_B)


def update_markers(dom, game, player):
  my_turn = is_my_turn(game, player)

  if my_turn is None:
    mark_player(dom, 'A' if game.current == 1 else 'B')
  elif my_turn:
    mark_player(dom, 'A')
  else:
    mark_player(dom, 'B')


def update_play_buttons(dom, game, player, winner):
  my_turn = is_my_turn(game, player)

  if my_turn is None or not my_turn or winner != 0:
    disable_play_buttons(dom)
  else:
    enable_play_buttons(dom)


def display_turn(dom, element, value):
    fade(dom, element)
    dom.set_content(element, value)


def update_dice(dom, game, winner):
  if winner != 0 or game.turn != 0 or game.dice == 0:
      display_dice(dom, game.dice)


def update_turn(dom, game, winner):
  if winner == 0:
    display_turn(dom, "Cumul", game.turn if game.turn != -1 else 0)
    display_turn(dom, "Total", game.scores[game.current] + (game.turn if game.turn != -1 else 0))


def report_winner(dom, player, winner):
  if winner == player or player == 0 and winner == 1:
    ab = 'A'
  else:
    ab = 'B'

  dom.set_content(f"ScoreMeter{ab}", "Winner!")


def update_layout(dom, game, player):
  if game.scores[1] >= LIMIT:
    winner = 1
  elif game. scores[2] >= LIMIT:
    winner = 2
  else:
    winner = 0

  update_play_buttons(dom, game, player, winner)
  update_dice(dom, game, winner)
  update_turn(dom, game, winner)
  update_meters(dom, game, player)
  update_markers(dom, game, player)

  if winner != 0:
    report_winner(dom, player, winner)


def display(dom, game, player):
  if game is None:
    disable_play_buttons(dom)
    dom.alert("Game aborted!")
    return

  if game.available == 0 and player == 0 :
    dom.disable_element("PlayerView")
  update_layout(dom, game, player)


def ac_connect(user, dom, id):
  dom.inner("", open("Main.html").read())
  user.init(id)

  if debug():
    dom.remove_class("debug", "removed")

  display(dom, user.get_game(), user.get_raw_player())


def get_player(user, dom):
  player = user.get_player()

  if player == 0:
    dom.alert("Game has already two players!\nReverting to default game mode.")
    user.init()
    player = 0
    display(dom, user.get_game(), player)
   
  return player


def computer_turn(game, dom):
  game.current = 2

  time.sleep(1)

  while True:
    game.dice = random.randint(1, 6)

    if game.dice == 1:
      game.turn = -1
      game.current = 1
      display(dom, game, 1)
      break;

    game.turn += game.dice

    if game.scores[2] + game.turn >= LIMIT:
      game.scores[2] = LIMIT
      game.turn = 0
      display(dom, game, 1)
      break

    display(dom, game, 1)
    time.sleep(2.5)    

    if random.randrange(3) == 0:
      game.scores[2] += game.turn
      game.turn = 0
      game.current = 1
      display(dom, game, 1)
      break;


def ac_roll(user, dom):
  disable_play_buttons(dom)

  player = get_player(user, dom)

  if player == 0:
    return


  game = user.get_game()

  if game is None:
    disable_play_buttons(dom)
    dom.alert("Game aborted!")
    return

  game.dice = random.randint(1, 6)

  if debug():
    debug_dice = dom.get_content("debug")
    if debug_dice:
      game.dice = int(debug_dice)

  if game.dice == 1:
    game.current = get_opponent(game.current)
    game.turn = -1 # To force the displaying of the dice.

    if not user.token:
      display(dom, game, 1)
      computer_turn(game, dom)
  else:
    game.turn += game.dice

    if game.scores[game.current] + game.turn >= LIMIT:
      game.scores[game.current] = LIMIT
      game.turn = 0

  if user.token:
    atlastk.broadcast_action("Display", user.token)
  else:
    display(dom, game, 1)



def ac_hold(user, dom):
  disable_play_buttons(dom)

  player = get_player(user, dom)

  if player == 0:
    return

  game = user.get_game()

  if game is None:
    disable_play_buttons(dom)
    dom.alert("Game aborted!")
    return

  game.scores[player] += game.turn
  game.current = get_opponent(game.current)
  game.turn = 0

  if user.token:
    atlastk.broadcast_action("Display", user.token)
  else:
    display(dom, game, 1)
    computer_turn(game, dom)


def new_between_humans(user, dom):
  global games

  if debug():
    token="demo"
  else:
    token = str(uuid.uuid4())

  create_game(token)

  url = atlastk.get_app_url(token)
  dom.inner("qrcode", f'<a href="{url}" title="{url}" target="_blank"><img style="margin: auto; width:100%;" src="https://api.qrserver.com/v1/create-qr-code/?size=200x200&data={urllib.parse.quote(url)}"/></a>')
  dom.disable_element("HideHHLinkSection")
  user.init(token)


def new_against_computer(user, dom):
  dom.enable_element("HideHHLinkSection")
  user.init()

  game = user.get_game()

  game.current = 1
  game.available = 0


def ac_new(user, dom):
  mode = dom.get_content("Mode")

  print(mode)

  dom.inner("", open("Main.html").read())
  dom.enable_element("PlayerView")

  if mode == "HC":
    new_against_computer(user, dom)
  else:
    new_between_humans(user, dom)

  display(dom, user.get_game(), 1)


def ac_display(user, dom, id):

  if id != user.token:
    return

  game = user.get_game()

  if game is None:
    disable_play_buttons(dom)
    dom.alert("Game aborted!")
    return

  if game.current == 1 and game.available == 1:
    dom.inner("", open("Main.html").read())
    if debug():
      dom.remove_class("debug", "removed")
    user.player = 0
  display(dom, user.get_game(), user.get_raw_player())


CALLBACKS = {
  "": ac_connect,
  "Roll": ac_roll,
  "Hold": ac_hold,
  "New": ac_new,
  "Display": ac_display
}

atlastk.launch(CALLBACKS, User, open("Head.html").read())
