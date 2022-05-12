"""Flooder, by Al Sweigart al@inventwithpython.com
GUI by Claude SIMON http://q37.info/contact
A colorful game where you try to fill the board with a single color.
Can be played by colorblind players.
Inspired by the "Flood It!" game.
Original code is available at https://nostarch.com/big-book-small-python-programming
Tags: large, game"""

# The 'atlastk' module will be imported from the provided
# 'atlastk.zip' archive, to avoid having to install it.  
__import__("sys").path.append("./atlastk.zip")

import atlastk

import random, sys

# Set up the constants:
BOARD_WIDTH = 16  # (!) Try changing this to 4 or 40.
BOARD_HEIGHT = 14  # (!) Try changing this to 4 or 20.
MOVES_PER_GAME = 20  # (!) Try changing this to 3 or 300.

# Constants for the different shapes used for colorblind:
HEART     = '♥'
DIAMOND   = '♦'
SPADE     = '♠'
CLUB      = '♣'
BALL      = '●'
TRIANGLE  = '▲'

# All the color/shape tiles used on the board:
TILE_TYPES = (0, 1, 2, 3, 4, 5)
COLORS_MAP = {0: 'cyan', 1: 'lightgreen', 2:'lightblue',
              3: 'yellow', 4:'salmon', 5:'burlywood'}
SHAPES_MAP = {0: HEART, 1: TRIANGLE, 2: DIAMOND,
              3: BALL, 4: CLUB, 5: SPADE}


def main():
  newGame()
  atlastk.launch(CALLBACKS, headContent=HEAD)  


def newGame():
  global gameBoard, movesLeft

  gameBoard = getNewBoard()   
  movesLeft = MOVES_PER_GAME


def getNewBoard():
  """Return a dictionary of a new Flood It board."""

  # Keys are (x, y) tuples, values are the tile at that position.
  board = {}

  # Create random colors for the board.
  for x in range(BOARD_WIDTH):
    for y in range(BOARD_HEIGHT):
      board[(x, y)] = random.choice(TILE_TYPES)

  # Make several tiles the same as their neighbor. This creates groups
  # of the same color/shape.
  for i in range(BOARD_WIDTH * BOARD_HEIGHT):
    x = random.randint(0, BOARD_WIDTH - 2)
    y = random.randint(0, BOARD_HEIGHT - 1)
    board[(x + 1, y)] = board[(x, y)]

  return board


def displayBoard(board, dom):
  """Display the board in the browser."""

  HTML = ""

  # Display each row:
  for y in range(BOARD_HEIGHT):
    HTML += "<tr>"

    # Display each tile in this row:
    for x in range(BOARD_WIDTH):
      tile = board[(x, y)]
      HTML += '<td xdh:onevent="Submit" data-tile="{}" style="background-color: {};">{}</td>'.format(tile, COLORS_MAP[tile],SHAPES_MAP[tile])

    HTML += "</tr>"

  dom.inner("board", HTML)

  if hasWon(board):
    notify(dom, "You have won!")
  elif movesLeft == 0:
    notify(dom, "You have run out of moves!")
  else:
    notify(dom, "Moves left: {}.".format(movesLeft))


def changeTile(tileType, board, x, y, charToChange=None):
  """Change the color/shape of a tile using the recursive flood fill
  algorithm."""
  if x == 0 and y == 0:
    charToChange = board[(x, y)]
    if tileType == charToChange:
      return  # Base Case: Already is the same tile.

  board[(x, y)] = tileType

  if x > 0 and board[(x - 1, y)] == charToChange:
    # Recursive Case: Change the left neighbor's tile:
    changeTile(tileType, board, x - 1, y, charToChange)
  if y > 0 and board[(x, y - 1)] == charToChange:
    # Recursive Case: Change the top neighbor's tile:
    changeTile(tileType, board, x, y - 1, charToChange)
  if x < BOARD_WIDTH - 1 and board[(x + 1, y)] == charToChange:
    # Recursive Case: Change the right neighbor's tile:
    changeTile(tileType, board, x + 1, y, charToChange)
  if y < BOARD_HEIGHT - 1 and board[(x, y + 1)] == charToChange:
    # Recursive Case: Change the bottom neighbor's tile:
    changeTile(tileType, board, x, y + 1, charToChange)    


def hasWon(board):
  """Return True if the entire board is one color/shape."""
  tile = board[(0, 0)]

  for x in range(BOARD_WIDTH):
    for y in range(BOARD_HEIGHT):
      if board[(x, y)] != tile:
        return False

  return True


def notify(dom, text):
  """ Displays a text with a little animation to attract attention."""
  dom.inner("output", '<output class="fade-in">{}</output>'.format(text))


def acConnect(dom):
  dom.inner("", BODY)
  displayBoard(gameBoard, dom)


def acSubmit(dom,id):
  global movesLeft

  if not hasWon(gameBoard) and movesLeft != 0:
    changeTile(int(dom.getAttribute(id, "data-tile")), gameBoard, 0, 0)
    movesLeft -= 1

  atlastk.broadcast_action("Display")


def acNew(dom):
  newGame()
  atlastk.broadcast_action("Display")


CALLBACKS = {
  "": acConnect,
  "Submit": acSubmit,
  "New": acNew,
  "Display": lambda dom: displayBoard(gameBoard, dom)
}

HEAD = """
<style>
  td {
    text-align: center;
    height: 3ex;
    font-size; x-large;
    cursor: pointer;
    width: 1em;
  }

  @keyframes fadeIn {
    0% {
      opacity: 0;
    }

    100% {
      opacity: 1;
    }
  }

  .fade-in {
    animation-name: fadeIn;
    animation-duration: 1s;
  }
</style>
"""

BODY = """
<details>
  <summary style="cursor: pointer;">About</summary>
  <fieldset style="width: 300px; background: aliceblue; position: absolute;">
Flooder, by Al Sweigart
<a href="mailto:al@inventwithpython.com">al@inventwithpython.com</a><br/>
GUI by Claude SIMON
<a href="http://q37.info/contact">http://q37.info/contact</a><hr/>
Set the upper left color/shape, which fills in all the
adjacent squares of that color/shape, by selecting a square
with the desired color/shape. Try to make the
entire board the same color/shape.
  </fieldset>
</details>
<fieldset>
  <fieldset>
    <table style="border-collapse: collapse;" id="board"></table>
  </fieldset>
  <fieldset id="output"></fieldset>
  </fieldset>
   <button style="display: flex; margin: 5px auto 0px;" xdh:onevent="New">New game</button>
</fieldset>
"""

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()