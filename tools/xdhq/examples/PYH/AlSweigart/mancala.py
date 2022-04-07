"""Mancala, by Al Sweigart al@inventwithpython.com
Fitted with a GUI using the Atlas toolkit by Claude SIMON http://q37.info/contact
The ancient seed-sowing game.
Original code is available at https://nostarch.com/big-book-small-python-programming
Tags: large, board game, game, two-player"""

import atlastk

# A tuple of the player's pits:
PLAYER_1_PITS = ('A', 'B', 'C', 'D', 'E', 'F')
PLAYER_2_PITS = ('G', 'H', 'I', 'J', 'K', 'L')

# A dictionary whose keys are pits and values are opposite pit:
OPPOSITE_PIT = {'A': 'G', 'B': 'H', 'C': 'I', 'D': 'J', 'E': 'K',
                   'F': 'L', 'G': 'A', 'H': 'B', 'I': 'C', 'J': 'D',
                   'K': 'E', 'L': 'F'}

# A dictionary whose keys are pits and values are the next pit in order:
NEXT_PIT = {'A': 'B', 'B': 'C', 'C': 'D', 'D': 'E', 'E': 'F', 'F': '1',
            '1': 'L', 'L': 'K', 'K': 'J', 'J': 'I', 'I': 'H', 'H': 'G',
            'G': '2', '2': 'A'}

# How many seeds are in each pit at the start of a new game:
STARTING_NUMBER_OF_SEEDS = 4  # (!) Try changing this to 1 or 10.


gameBoard = None
playerTurn = None

def main():
  newGame()
  atlastk.launch(CALLBACKS, headContent=HEAD)

def newGame():
  global gameBoard, playerTurn

  gameBoard = getNewBoard()
  playerTurn = '1'  # Player 1 goes first.

def getNewBoard():
  """Return a dictionary representing a Mancala board in the starting
  state: 4 seeds in each pit and 0 in the stores."""

  # Syntactic sugar - Use a shorter variable name:
  s = STARTING_NUMBER_OF_SEEDS

  # Create the data structure for the board, with 0 seeds in the
  # stores and the starting number of seeds in the pits:
  return {'1': 0, '2': 0, 'A': s, 'B': s, 'C': s, 'D': s, 'E': s,
          'F': s, 'G': s, 'H': s, 'I': s, 'J': s, 'K': s, 'L': s}

def updateBoard(dom, board):
  """Displays the game board as ASCII-art based on the board
  dictionary."""

  dom.set_values(board)

  # Check if the game ended and a player has won:
  winner = checkForWinner(gameBoard)
  if winner == '1' or winner == '2':
    dom.set_value("output", 'Player ' + winner + ' has won!')
  elif winner == 'tie':
    dom.set_value("output", 'There is a tie!')
  else:
    dom.set_value("output", "Player {}, click one of your pit".format(playerTurn))


def makeMove(board, playerTurn, pit):
    """Modify the board data structure so that the player 1 or 2 in
    turn selected pit as their pit to sow seeds from. Returns either
    '1' or '2' for whose turn it is next."""

    seedsToSow = board[pit]  # Get number of seeds from selected pit.
    board[pit] = 0  # Empty out the selected pit.

    while seedsToSow > 0:  # Continue sowing until we have no more seeds.
        pit = NEXT_PIT[pit]  # Move on to the next pit.
        if (playerTurn == '1' and pit == '2') or (
            playerTurn == '2' and pit == '1'
        ):
            continue  # Skip opponent's store.
        board[pit] += 1
        seedsToSow -= 1

    # If the last seed went into the player's store, they go again.
    if (pit == playerTurn == '1') or (pit == playerTurn == '2'):
        # The last seed landed in the player's store; take another turn.
        return playerTurn

    # Check if last seed was in an empty pit; take opposite pit's seeds.
    if playerTurn == '1' and pit in PLAYER_1_PITS and board[pit] == 1:
        oppositePit = OPPOSITE_PIT[pit]
        board['1'] += board[oppositePit]
        board[oppositePit] = 0
    elif playerTurn == '2' and pit in PLAYER_2_PITS and board[pit] == 1:
        oppositePit = OPPOSITE_PIT[pit]
        board['2'] += board[oppositePit]
        board[oppositePit] = 0

    # Return the other player as the next player:
    if playerTurn == '1':
        return '2'
    elif playerTurn == '2':
        return '1'

def checkForWinner(board):
    """Looks at board and returns either '1' or '2' if there is a
    winner or 'tie' or 'no winner' if there isn't. The game ends when a
    player's pits are all empty; the other player claims the remaining
    seeds for their store. The winner is whoever has the most seeds."""

    player1Total = board['A'] + board['B'] + board['C']
    player1Total += board['D'] + board['E'] + board['F']
    player2Total = board['G'] + board['H'] + board['I']
    player2Total += board['J'] + board['K'] + board['L']

    if player1Total == 0:
        # Player 2 gets all the remaining seeds on their side:
        board['2'] += player2Total
        for pit in PLAYER_2_PITS:
            board[pit] = 0  # Set all pits to 0.
    elif player2Total == 0:
        # Player 1 gets all the remaining seeds on their side:
        board['1'] += player1Total
        for pit in PLAYER_1_PITS:
            board[pit] = 0  # Set all pits to 0.
    else:
        return 'no winner'  # No one has won yet.

    # Game is over, find player with largest score.
    if board['1'] > board['2']:
        return '1'
    elif board['2'] > board['1']:
        return '2'
    else:
        return 'tie'



def acConnect(dom):
  dom.inner("", BODY)
  updateBoard(dom, gameBoard)

def acSubmit(dom, id):
  global gameBoard, playerTurn

  response = id

  # Make sure it is a valid pit to select:
  if (playerTurn == '1' and response not in PLAYER_1_PITS) or (
      playerTurn == '2' and response not in PLAYER_2_PITS
  ):
      dom.set_value('output', 'Please pick a pit on your side of the board.')
      return
  if gameBoard.get(response) == 0:
      dom.set_value('output', 'Please pick a non-empty pit.')
      return

  playerTurn = makeMove(gameBoard, playerTurn, response)

  atlastk.broadcast_action("Display")      
  

def acNew(dom):
  pass

HEAD = """
<style>
.store_fieldset {
  position: relative;
  align-items: center;
  display: flex;
}

.store_legend {
  background: aliceblue;
  position: absolute;
  writing-mode: vertical-rl;
  text-orientation: upright;  
}

.store_legend_1 {
  right: 0;
  transform: translate(75%);
}

.store_legend_2 {
  left: 0;
  transform: translate(-75%);
}

.pits_fieldset {
  position: relative;
  display: flex;
  padding: 15px;
}

.pits_legend {
  background: aliceblue;
  position: absolute;
}

.pits_legend_1 {
  bottom: 0;
  left: 50%;
  transform: translate(-25%, 50%);
  }

.pits_legend_2 {
  top: 0;
  transform: translate(25%, -50%);
  right: 50%;
}

button {
  width: 5ch;
  padding: 5px;
  margin: 5px;
}
</style>
"""

BODY = """
<details>
  <summary style="cursor: pointer;">About</summary>
  <fieldset style="z-index: 6; width: 400px; background: aliceblue; position: absolute;">
Mancala, by Al Sweigart
<a href="mailto:al@inventwithpython.com">al@inventwithpython.com</a><br/>
GUI by Claude SIMON
<a href="http://q37.info/contact">http://q37.info/contact</a><hr/>
The ancient two-player, seed-sowing game. Grab the seeds from a pit on
your side and place one in each following pit, going counterclockwise
and skipping your opponent's store. If your last seed lands in an empty
pit of yours, move the opposite pit's seeds into your store. The
goal is to get the most seeds in your store on the side of the board.
If the last placed seed is in your store, you get a free turn.
<p/>
The game ends when all of one player's pits are empty. The other player
claims the remaining seeds for their store, and the winner is the one
with the most seeds.
<p/>
More info at <a href="https://en.wikipedia.org/wiki/Mancala">https://en.wikipedia.org/wiki/Mancala</a>
</fieldset>
</details>
<fieldset style="display: flex; flex-direction: row">
  <fieldset class="store_fieldset">
    <legend class="store_legend store_legend_2">2 Store</legend>
    <fieldset id="2" style="width: 2ch; text-align: center;"></fieldset>
  </fieldset>
 <span>
<fieldset class="pits_fieldset">
  <legend class="pits_legend pits_legend_2">&lt;&lt;&lt;&lt;&lt; Player 2</legend>
""" + ( 6 * '<button id="{}" xdh:onevent="Submit"></button>' ).format(*list("GHIJKL")) + """
</fieldset>
<fieldset class="pits_fieldset">
  <legend class="pits_legend pits_legend_1">Player 1 &gt;&gt;&gt;&gt;&gt;</legend>
""" + ( 6 * '<button id="{}" xdh:onevent="Submit"></button>' ).format(*list("ABCDEF")) + """
</fieldset>
</span>
  <fieldset class="store_fieldset">
    <legend class="store_legend store_legend_1">1 Store</legend>
    <fieldset id="1" style="width: 2ch; text-align: center;"></fieldset>
  </fieldset>
</fieldset>
<fieldset id="output"></fieldset>
"""

CALLBACKS = {
  "": acConnect,
  "Display": lambda dom: updateBoard(dom, gameBoard),
  "Submit": acSubmit,
  "New": acNew
}

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()