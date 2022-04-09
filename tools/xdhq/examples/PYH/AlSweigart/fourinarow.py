"""Four in a Row, by Al Sweigart al@inventwithpython.com
Fitted with a WEB GUI by Claude SIMON http://q37.info/contact
A tile-dropping game to get four in a row, similar to Connect Four.
Original code is available at https://nostarch.com/big-book-small-python-programming
Tags: large, game, board game, two-player"""

import atlastk

# Constants used for displaying the board:
EMPTY_SPACE = '.'  # A period is easier to count than a space.
PLAYER_X = 'X'
PLAYER_O = 'O'

BOARD_WIDTH = 7
BOARD_HEIGHT = 6

gameBoard = None
playerTurn = None

def main():
  newGame()
  atlastk.launch(CALLBACKS, headContent=HEAD)

def newGame():
  global gameBoard, playerTurn

  gameBoard = getNewBoard()
  playerTurn = PLAYER_X

def getNewBoard():
  """Returns a dictionary that represents a Four in a Row board.

  The keys are (columnIndex, rowIndex) tuples of two integers, and the
  values are one of the 'X', 'O' or '.' (empty space) strings."""
  board = {}
  for columnIndex in range(BOARD_WIDTH):
    for rowIndex in range(BOARD_HEIGHT):
      board[(columnIndex, rowIndex)] = EMPTY_SPACE
  return board

def displayBoard(dom, board, playerTile):
  """Display the board and its tiles on the screen."""

  '''Prepare a list to pass to the format() string method for the
  board template. The list holds all of the board's tiles (and empty
  spaces) going left to right, top to bottom:'''
  gameOver = True
  winner = getWinner(board)

  # Check for a win or tie:
  if winner is not None:
    output(dom, 'Player {} has won!'.format(winner))
  elif isFull(board):
    output(dom, 'There is a tie!')
  else:
    output(dom, "Player {}, click a column button.".format(playerTile))
    gameOver = False

  tileChars = []
  for rowIndex in range(BOARD_HEIGHT):
    for columnIndex in range(BOARD_WIDTH):
      tileChars.append(board[(columnIndex, rowIndex)])

  htmlFeatures = []
  for columnIndex in range(BOARD_WIDTH):
    if gameOver or gameBoard[(columnIndex, 0)] != EMPTY_SPACE:
      htmlFeatures.append('disabled="disabled"')
    else:
      htmlFeatures.append('')
    htmlFeatures.extend([columnIndex, columnIndex+1])

  # Display the board:
  dom.inner("board",BOARD.format(*htmlFeatures, *tileChars))

def isFull(board):
  """Returns True if the `board` has no empty spaces, otherwise
  returns False."""

  for rowIndex in range(BOARD_HEIGHT):
    for columnIndex in range(BOARD_WIDTH):
      if board[(columnIndex, rowIndex)] == EMPTY_SPACE:
        return False  # Found an empty space, so return False.
  return True  # All spaces are full.


def getWinner(board):
  """Returns the player who has four tiles in a row on `board`,
  otherwise returns None."""

  # Go through the entire board, checking for four-in-a-row:
  for columnIndex in range(BOARD_WIDTH - 3):
    for rowIndex in range(BOARD_HEIGHT):
      # Check for horizontal four-in-a-row going right:
      tile1 = board[(columnIndex, rowIndex)]
      tile2 = board[(columnIndex + 1, rowIndex)]
      tile3 = board[(columnIndex + 2, rowIndex)]
      tile4 = board[(columnIndex + 3, rowIndex)]
      if tile1 == tile2 == tile3 == tile4 != EMPTY_SPACE:
        return tile1

  for columnIndex in range(BOARD_WIDTH):
    for rowIndex in range(BOARD_HEIGHT - 3):
      # Check for vertical four-in-a-row going down:
      tile1 = board[(columnIndex, rowIndex)]
      tile2 = board[(columnIndex, rowIndex + 1)]
      tile3 = board[(columnIndex, rowIndex + 2)]
      tile4 = board[(columnIndex, rowIndex + 3)]
      if tile1 == tile2 == tile3 == tile4 != EMPTY_SPACE:
        return tile1

  for columnIndex in range(BOARD_WIDTH - 3):
    for rowIndex in range(BOARD_HEIGHT - 3):
      # Check for four-in-a-row going right-down diagonal:
      tile1 = board[(columnIndex, rowIndex)]
      tile2 = board[(columnIndex + 1, rowIndex + 1)]
      tile3 = board[(columnIndex + 2, rowIndex + 2)]
      tile4 = board[(columnIndex + 3, rowIndex + 3)]
      if tile1 == tile2 == tile3 == tile4 != EMPTY_SPACE:
        return tile1

      # Check for four-in-a-row going left-down diagonal:
      tile1 = board[(columnIndex + 3, rowIndex)]
      tile2 = board[(columnIndex + 2, rowIndex + 1)]
      tile3 = board[(columnIndex + 1, rowIndex + 2)]
      tile4 = board[(columnIndex, rowIndex + 3)]
      if tile1 == tile2 == tile3 == tile4 != EMPTY_SPACE:
        return tile1
  return None

def output(dom, text):
  dom.inner("output", '<output class="fade-in">{}</output>'.format(text))

def acConnect(dom):
  dom.inner("", BODY)
  displayBoard(dom, gameBoard, playerTurn)

def acSubmit(dom, id):
  global gameBoard, playerTurn
  
  columnIndex = int(id)

  # If the column is full, ask for a move again:
  if gameBoard[(columnIndex, 0)] != EMPTY_SPACE:
    output(dom, 'That column is full, select another one.')
    return

  # Starting from the bottom, find the first empty space.
  for rowIndex in range(BOARD_HEIGHT - 1, -1, -1):
    if gameBoard[(columnIndex, rowIndex)] == EMPTY_SPACE:
      playerMove = (columnIndex, rowIndex)
      break

  gameBoard[playerMove] = playerTurn

  # Switch turns to other player:
  if playerTurn == PLAYER_X:
    playerTurn = PLAYER_O
  elif playerTurn == PLAYER_O:
    playerTurn = PLAYER_X

  atlastk.broadcast_action("Display")

def acNew(dom):
  newGame()
  atlastk.broadcast_action("Display")

CALLBACKS = {
  "": acConnect,
  "Display": lambda dom: displayBoard(dom, gameBoard, playerTurn),
  "Submit": acSubmit,
  "New": acNew
}

HEAD = """
<title>Four in a Row</title>
<style>
  td {
    text-align: center;
    width: 1ic;
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
Four in a Row, by Al Sweigart
<a href="mailto:al@inventwithpython.com">al@inventwithpython.com</a><br/>
GUI by Claude SIMON
<a href="http://q37.info/contact">http://q37.info/contact</a><hr/>
Two players take turns dropping tiles into one of seven columns, trying
to make four in a row horizontally, vertically, or diagonally.
  </fieldset>
</details>
<fieldset>
  <fieldset id="board" style="font-size: xx-large;"></fieldset>
  <fieldset id="output"></fieldset>
  <button style="display: flex;	margin: 5px auto 0px;" id="New" xdh:onevent="New">New game</button>
</fieldset>
"""

BOARD = '<table style="margin: auto; width: 100%;"><tr>' +\
  BOARD_WIDTH * '<th><button xdh:onevent="Submit" {} id="{}">{}</button></th>' +\
  "</tr>" + BOARD_HEIGHT * ("<tr>" + BOARD_WIDTH * "<td>{}</td>" +
  "</tr>") + "</table>"

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()