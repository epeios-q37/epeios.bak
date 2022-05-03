"""Blackjack, by Al Sweigart al@inventwithpython.com
GUI by Claude SIMON http://q37.info/contact
The classic card game also known as 21. (This version doesn't have
splitting or insurance.)
Original code is available at https://nostarch.com/big-book-small-python-programming
Tags: large, game, card game"""

# The 'atlastk' module will be imported from the provided
# 'atlastk.zip' archive, to avoid having to install it.  
__import__("sys").path.append("./atlastk.zip")

import atlastk

import random, time, threading, uuid, builtins
from xml.etree import ElementTree

builtins.open = atlastk.defaultBuiltinsFunction

NOTIFICATIONS = {
  # '0' to report no notification to display.
  1: "Waiting your bet.",
  2: "Hit, stand or double bet.",
  3: "Hit or stand.",
  4: "Dealer is playing",
  5: "Dealer busts! You win ${}!",
  6: "You won!",
  7: "It's a tie, the bet is returned to you.",
  8: "You lost ${}!"
}

def main():
  global lock
  lock = threading.Lock()

  newGame()
  initSVGCards()
  atlastk.launch(CALLBACKS, headContent=HEAD)


def newGame():
  global money

  money = 5000
  newTurn()


def initSVGCards():
  global SVG_DEFS, SVG_CARDS
  ElementTree.register_namespace('','http://www.w3.org/2000/svg')
  ElementTree.register_namespace('xlink','http://www.w3.org/1999/xlink')
  tree = ElementTree.parse("./cards.svg")
  rootChildren = tree.getroot().getchildren()
  SVG_DEFS = ElementTree.tostring(rootChildren[0],encoding="unicode")
  SVG_CARDS = [
      ElementTree.tostring(child, encoding="unicode")
      for child in rootChildren[1].getchildren()
  ]


def getSVGCard(indice):
  return '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" height="243.137" width="167.575" viewBox="-.2 -236 167.575 243.137">' + SVG_CARDS[indice] + '</svg>'


def getDeck():
  deck = list(range(52))
  random.shuffle(deck)
  return deck


def newTurn():
  global deck, playerHand, dealerHand, bet

  deck = getDeck()
  dealerHand = [54]
  playerHand = [54]
  bet = 0   


def getHandValue(cards):
    """Returns the value of the cards. Face cards are worth 10, aces are
    worth 11 or 1 (this function picks the most suitable ace value)."""
    value = 0
    numberOfAces = 0

    # Add the value for the non-ace cards:
    for card in cards:
        rank = 1 + card % 13
        if rank == 1:
            numberOfAces += 1
        elif rank in (11, 12, 13):  # Face cards are worth 10 points.
            value += 10
        else:
            value += rank  # Numbered cards are worth their number.

    # Add the value for the aces:
    value += numberOfAces  # Add 1 per ace.
    for i in range(numberOfAces):
        # If another 10 can be added without busting, do so:
        if value + 10 <= 21:
            value += 10

    return value


def displayCards(dom, id, cards):
    """Display all the cards in the cards list."""
    html = ""

    for card in reversed(cards):
      html += getSVGCard(card)

    dom.inner(id, html)


def notify(dom, text, id="hint"):
  """ Displays a text with a little animation to attract attention."""
  dom.inner(id, '<output class="fade-in">{}</output>'.format(text))


def testAction(dom, action, dealerHand, playerHand, bet):
  playerHandSize = len(playerHand)
  
  if bet <= 0 and playerHandSize >= 2:
    if action != 'R':
      notify(dom, "Hit resume for another turn!")
      return False
  elif playerHandSize > 2:
    if action not in ['H', 'S']:
      notify(dom, "You can only hit or stand!")
      return False
  elif playerHandSize == 2:
    if action not in ['H', 'S', 'D']:
      notify(dom, "You can only hit, stand or double down!")
      return False
  elif action not in ['+', '-', 'B']:
    notify(dom, "You can only bet!")
    return False
  elif action == 'B' and bet <= 0:
    notify(dom, "Your bet is empty!")
    return False

  return True


def dealerTurn(dealerHand, playerHand):
  while getHandValue(dealerHand) < 17:
    atlastk.broadcast_action("RefreshDisplay", '4')
    time.sleep(1)
    dealerHand.append(deck.pop())
    atlastk.broadcast_action("RefreshDisplay", 'D0')

  time.sleep(1)

  playerHandValue = getHandValue(playerHand)
  dealerHandValue = getHandValue(dealerHand)

  if dealerHandValue > 21:
    notification = 5
    status = 1
  elif dealerHandValue < playerHandValue:
    notification = 6
    status = 1
  elif dealerHandValue == playerHandValue:
    notification = 7
    status = 0
  else:
    notification = 8
    status = -1

  atlastk.broadcast_action("RefreshDisplay", "D{}".format(notification))

  return status


def acConnect(dom):
  dom.inner("", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="0" height="0">' + SVG_DEFS + '</svg>' + BODY)
  turnInProgress = bet > 0 or len(playerHand) >= 2
  displayHand(dom, 'dealer', dealerHand, False )
  displayHand(dom, 'player', playerHand, turnInProgress )
  notify(dom, money, "money")
  notify(dom, abs(bet), "bet")

  if turnInProgress:
    notify(dom, "Turn in progress…")
  else:
    notify(dom, "Waiting your bet.")


def displayHand(dom, host, hand, show):
  dom.setContent("{}_hand_value".format(host), getHandValue(hand) if show else "???")
  displayCards(dom, "{}_cards".format(host), hand if show else [54] + hand[1:])


def acRefreshDisplay(dom, id):
  print(id, bet)

  notification = int(id[-1])

  if notification != 0:
    notify(dom, NOTIFICATIONS[notification].format(abs(bet)))

  if 'd' in id.lower():
    displayHand(dom, 'dealer', dealerHand, 'D' in id)

  if 'p' in id.lower():
    displayHand(dom, 'player', playerHand, 'P' in id)

  if 'M' in id:
    notify(dom, money, "money")

  if 'B' in id:
    notify(dom, bet, "bet")


def acAdd(dom, id):
  global money, bet

  if not testAction(dom, '+', dealerHand, playerHand, bet):
    return

  value = int(dom.get_mark(id))

  if ( value + bet ) > money:
    notify(dom, "You can't bet that much!")
    return

  bet += value

  atlastk.broadcast_action("RefreshDisplay", 'B1')


def acSub(dom, id):
  global money, bet

  if not testAction(dom, '-', dealerHand, playerHand, bet):
    return

  value = int(dom.get_mark(id))

  if value > bet:
    notify(dom, "You can't to remove that much!")
    return

  bet -= value

  atlastk.broadcast_action("RefreshDisplay", 'B1')


def acBet(dom):
  global dealerHand, playerHand

  if not testAction(dom, 'B', dealerHand, playerHand, bet):
    return

  dealerHand = [deck.pop(), deck.pop()]
  playerHand = [deck.pop(), deck.pop()]

  atlastk.broadcast_action("RefreshDisplay", 'dP2')


def acHit(dom):
  global money, bet, lockId

  if not testAction(dom, 'H', dealerHand, playerHand, bet):
    return

  playerHand.append(deck.pop())

  handValue = getHandValue(playerHand)

  if handValue > 21:
    money -= bet
    bet = -bet
    notification = 8
  elif handValue < 21:
    notification = 3
  else:
    lockId = str(uuid.uuid4())
    atlastk.broadcast_action("RefreshDisplay", 'P0')
    atlastk.broadcast_action("DealerMove", lockId)
    return

  atlastk.broadcast_action("RefreshDisplay", 'P{}'.format(notification))


def acStand(dom):
  global money, bet, lockId

  if not testAction(dom, 'S', dealerHand, playerHand, bet):
    return

  lockId = str(uuid.uuid4())
  atlastk.broadcast_action("DealerMove", lockId)


def acDoubleDown(dom):
  global money, bet, playerHand, lockId

  if not testAction(dom, 'D', dealerHand, playerHand, bet):
    return

  value = min(bet, money - bet)

  if value == 0:
    notify(dom, "Not enough money to double down!")
    return

  bet += value

  playerHand.append(deck.pop())
  atlastk.broadcast_action("RefreshDisplay", 'BP0')

  lockId = str(uuid.uuid4())
  atlastk.broadcast_action("DealerMove", lockId)


def acDealerMove(dom, id):
  global lockId, dealerHand, money, bet
  chosenOne = False

  displayHand(dom, 'dealer', dealerHand, True)
  notify(dom, "Dealer plays.")

  with lock:
    chosenOne = id == lockId
    if chosenOne:
      lockId = str(uuid.uuid4())

  if chosenOne:
    dealerHandValue = getHandValue(dealerHand)
    playerHandValue = getHandValue(playerHand)
    
    if dealerHandValue < playerHandValue or ( dealerHandValue == playerHandValue and dealerHandValue < 17 ):
      dealerHand.append(deck.pop())
      time.sleep(1)
      atlastk.broadcast_action("DealerMove", lockId)
    else:
      if dealerHandValue > 21:
        notification = 5
        money += bet
      elif dealerHandValue == playerHandValue:
        notification = 7
      else:
        notification = 8
        money -= bet
      bet = -bet # To report end of turn.

      atlastk.broadcast_action("RefreshDisplay", "M{}".format(notification))


def acResume(dom):
  if not testAction(dom, 'R', dealerHand, playerHand, bet):
    return

  newTurn()
  atlastk.broadcast_action("RefreshDisplay", 'dpB1')


CALLBACKS = {
  "": acConnect,
  "RefreshDisplay": acRefreshDisplay,
  "+": acAdd,
  "-": acSub,
  "Bet": acBet,
  "Hit": acHit,
  "Stand": acStand,
  "DoubleDown": acDoubleDown,
  "DealerMove": acDealerMove,
  "Resume": acResume
}

CHIP_VALUES = [1, 5, 10, 50, 100, 500]

CHIP = """
<span xdh:mark="{}" class="chip">
  <button xdh:onevent="+">+</button>
  <fieldset class="chip-value">{}</fieldset>
  <button xdh:onevent="-">-</button>
</span>
"""

CHIPS = """
<div class="chips">
""" + len(CHIP_VALUES) * CHIP + """
  <span>
    <fieldset>
      <legend>Money</legend>
      <span id="money">5000</span>
    </fieldset>
    <fieldset>
      <legend>Bet</legend>
      <span id="bet">500</span>
    </fieldset>
  </span>
</div>
"""

BODY = """
<fieldset>
  <div style="display: flex; justify-content: space-around; padding-bottom: 10px;">
    <button xdh:onevent="Bet">Bet</button>
    <button xdh:onevent="Hit">Hit</button>
    <button xdh:onevent="Stand">Stand</button>
    <button xdh:onevent="DoubleDown">Double down</button>
    <span></span>
    <button xdh:onevent="Resume">Resume</button>
  </div>
  <fieldset id="hint"></fieldset>
""" + CHIPS.format(*[v for v in CHIP_VALUES for _ in range(2) ]) + """
  <span style="display: flex">
    <fieldset>
      <legend>
        <span>Dealer: </span>
        <output id="dealer_hand_value"/>
      </legend>
      <span id="dealer_cards"/>
    </fieldset>
    <fieldset id="player">
      <legend>
        <span>Player: </span>
        <output id="player_hand_value"/>
      </legend>
      <span id="player_cards"/>
    </fieldset>
  </fieldset>
</fieldset>
"""

HEAD = """
<style>
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
  #dealer_cards, #player_cards {
    display: flex;
    flex-direction: column;
  }
  .chip {
    display: flex;
    flex-direction: column;
  }
  .chip-value {
    width: 3ch;
    justify-content: center;
    display: flex;
  }
  .chips {
    display: flex;
    justify-content: space-evenly;
    align-items: center;
    padding: 5px;
  }
  .chips span fieldset {
    padding: 0px;
  }
  .chips span fieldset span {
    justify-content: right;
    display: flex;
  }
  .chips span button {
    font-size: large;
  }
</style>
"""

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()