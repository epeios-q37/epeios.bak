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
COLORS_MAP = {0: 'red', 1: 'green', 2:'lightblue',
              3: 'yellow', 4:'cyan', 5:'purple'}
SHAPES_MAP = {0: HEART, 1: TRIANGLE, 2: DIAMOND,
              3: BALL, 4: CLUB, 5: SPADE}

def main():
  newGame()
  atlastk.launch(CALLBACKS, headContent=HEAD)  

def newGame():
  global gameBoard
  gameBoard = getNewBoard()              

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
    """Display the board on the screen."""

    HTML = "<div>"

    # Display each row:
    for y in range(BOARD_HEIGHT):

        # Display each tile in this row:
        for x in range(BOARD_WIDTH):
                tile = board[(x, y)]
                HTML += '<span style="background-color: {};">{}</span>'.format(COLORS_MAP[tile],SHAPES_MAP[tile])

        HTML += "<br/>"

    HTML += "</div>"

    dom.inner("board", HTML)

def acConnect(dom):
  dom.inner("", BODY)
  displayBoard(gameBoard, dom)

CALLBACKS = {
  "": acConnect
}

HEAD = """
<style>
  #board {
    font-family: "courier";
    font-size: xx-large;
    /*background-color: black;*/
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
adjacent squares of that color/shape. Try to make the
entire board the same color/shape.
  </fieldset>
</details><fieldset id="board">
</fieldset>
"""

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()