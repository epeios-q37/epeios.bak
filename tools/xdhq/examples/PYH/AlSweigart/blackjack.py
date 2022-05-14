"""Blackjack, by Al Sweigart al@inventwithpython.com
GUI by Claude SIMON http://q37.info/contact
The classic card game also known as 21. (This version doesn't have
splitting or insurance.)
Original code is available at https://nostarch.com/big-book-small-python-programming
Tags: large, game, card game"""


import contextlib
# The 'atlastk' module will be imported from the provided
# 'atlastk.zip' archive, to avoid having to install it.  
__import__("sys").path.append("./atlastk.zip")

import atlastk

import random, time, uuid, builtins
from xml.etree import ElementTree
from threading import Lock 

# Don't bother; developper's stuff.
with contextlib.suppress(AttributeError):
  __import__("builtins").open = atlastk.defaultBuiltinsFunction

NOTIFICATIONS = {
  # '0' to report no notification to display.
  1: "Waiting for bet.",
  2: "Hit, stand or double down.",
  3: "Hit or stand.",
  4: "Dealer is playing",
  5: "Dealer busts! You win {}!",
  6: "You won!",
  7: "It's a tie, the bet is returned to you.",
  8: "You lost {}!",
  9: "You're broke!"
}

def main():
  global lock

  lock = Lock()

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
  rootChildren = list(tree.getroot())
  SVG_DEFS = ElementTree.tostring(rootChildren[0],encoding="unicode")
  SVG_CARDS = [
      ElementTree.tostring(child, encoding="unicode")
      for child in list(rootChildren[1])
  ]


def newTurn():
  global deck, playerHand, dealerHand, bet

  deck = getDeck()
  dealerHand = [54]
  playerHand = [54]
  bet = 0   


def getDeck():
  deck = list(range(52))
  random.shuffle(deck)
  return deck


def getSVGCard(indice):
  return '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" height="243.137" width="167.575" viewBox="-.2 -236 167.575 243.137">' + SVG_CARDS[indice] + '</svg>'


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
  for _ in range(numberOfAces):
    # If another 10 can be added without busting, do so:
    if value <= 11:
      value += 10

  return value


def getSVGCards(cards):
  """Display all the cards in the cards list."""
  return "".join(getSVGCard(card) for card in reversed(cards))


def notify(dom, text, id="hint"):
  """ Displays a text with a little animation to attract attention."""
  dom.inner(id, '<output class="fade-in">{}</output>'.format(text))


def testAction(dom, action, playerHand, bet):
  """ Tests if an action is allowed and displays a message if not."""
  """ See 'ac…' functions for the meanong of 'action'."""
  """ if 'bet' < 0, the turn is over. """
  playerHandSize = len(playerHand)
  
  if money <= 0 and action != 'R':
    notify(dom,"You're broke! Hit 'Resume' for a new game!")
    return False;
  if bet <= 0 and playerHandSize >= 2:
    if action != 'R':
      notify(dom, "Hit 'Resume' for another turn!")
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


def displayHand(dom, host, hand, show):
  dom.setContent("{}_hand_value".format(host), getHandValue(hand) if show else "???")
  dom.inner("{}_cards".format(host), getSVGCards(hand if show else [54] + hand[1:]))


def letDealerPlay():
  global leaderId

  leaderId = str(uuid.uuid4())
  atlastk.broadcast_action("DealerMove", leaderId)


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


def acRefreshDisplay(dom, id):
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
    notify(dom, abs(bet), "bet")


def acDealerMove(dom, id):
  global leaderId, dealerHand, money, bet

  displayHand(dom, 'dealer', dealerHand, True)
  notify(dom, "Dealer plays.")

  with lock:
    leader = id == leaderId
    if leader:
      leaderId = str(uuid.uuid4())

  if leader:
    notification = 0
    dealerHandValue = getHandValue(dealerHand)
    playerHandValue = getHandValue(playerHand)
    
    if dealerHandValue > 21:
      money += bet
      notification = 5
    elif dealerHandValue > playerHandValue:
      money -= bet
      notification = 8 if money > 0 else 9
    elif dealerHandValue == playerHandValue and dealerHandValue>= 17:
        notification = 7

    if notification:
      bet = -bet # To report end of turn.
      atlastk.broadcast_action("RefreshDisplay", "M{}".format(notification))
    else:
      dealerHand.append(deck.pop())
      time.sleep(1)
      atlastk.broadcast_action("DealerMove", leaderId)


def acAdd(dom, id):
  global money, bet

  if not testAction(dom, '+', playerHand, bet):
    return

  value = int(dom.get_mark(id))

  if ( value + bet ) > money:
    notify(dom, "You can't bet that much!")
    return

  bet += value

  notify(dom, "Bet raised by {} to {}.".format(value, bet))
  atlastk.broadcast_action("RefreshDisplay", 'B0')


def acSub(dom, id):
  global money, bet

  if not testAction(dom, '-', playerHand, bet):
    return

  value = int(dom.get_mark(id))

  if value > bet:
    notify(dom, "You can't to remove that much!")
    return

  bet -= value

  notify(dom, "Bet down by {} to {}.".format(value, bet))
  atlastk.broadcast_action("RefreshDisplay", 'B0')


def acBet(dom):
  global dealerHand, playerHand

  if not testAction(dom, 'B', playerHand, bet):
    return

  dealerHand = [deck.pop(), deck.pop()]
  playerHand = [deck.pop(), deck.pop()]

  atlastk.broadcast_action("RefreshDisplay", 'dP2')

  if getHandValue(playerHand) == 21:
    letDealerPlay()  


def acHit(dom):
  global money, bet

  if not testAction(dom, 'H', playerHand, bet):
    return

  playerHand.append(deck.pop())

  handValue = getHandValue(playerHand)

  if handValue == 21:
    atlastk.broadcast_action("RefreshDisplay", 'P0')
    letDealerPlay()
  else:
    view = "P"
    if handValue > 21:
      money -= bet
      bet = -bet
      notification = 8 if money > 0 else 9
      view += "M"
    else: # handValue < 21
      notification = 3

    atlastk.broadcast_action("RefreshDisplay", '{}{}'.format(view, notification))


def acStand(dom):
  global money, bet

  if not testAction(dom, 'S', playerHand, bet):
    return

  letDealerPlay()


def acDoubleDown(dom):
  global money, bet, playerHand

  if not testAction(dom, 'D', playerHand, bet):
    return

  value = min(bet, money - bet)

  if value == 0:
    notify(dom, "Not enough money to double down!")
    return

  bet += value

  playerHand.append(deck.pop())

  if getHandValue(playerHand) > 21:
    money -= bet
    bet = -bet
    atlastk.broadcast_action("RefreshDisplay", 'MBP{}'.format(8 if money >= 0 else 9))
  else:
    atlastk.broadcast_action("RefreshDisplay", 'BP1')
    letDealerPlay()


def acResume(dom):
  if not testAction(dom, 'R', playerHand, bet):
    return

  if money > 0:
    newTurn()
  else:
    newGame()
  atlastk.broadcast_action("RefreshDisplay", 'dpMB1')


def acNew(dom):
  newGame()
  atlastk.broadcast_action("RefreshDisplay", 'dpMB1')


CALLBACKS = {
  "": acConnect,
  "RefreshDisplay": acRefreshDisplay,
  "DealerMove": acDealerMove,
  "+": acAdd,
  "-": acSub,
  "Bet": acBet,
  "Hit": acHit,
  "Stand": acStand,
  "DoubleDown": acDoubleDown,
  "Resume": acResume,
  "New": acNew
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
      <span id="money"></span>
    </fieldset>
    <fieldset>
      <legend>Bet</legend>
      <span id="bet"></span>
    </fieldset>
  </span>
</div>
"""

BODY = """
<details>
  <summary style="cursor: pointer;">About</summary>
  <fieldset style="width: 300px; background: aliceblue; position: absolute;">
Blackjack, by Al Sweigart
<a href="mailto:al@inventwithpython.com">al@inventwithpython.com</a><br/>
GUI by Claude SIMON
<a href="http://q37.info/contact">http://q37.info/contact</a><hr/>
Try to get as close to 21 without going over.<br/>
Kings, Queens, and Jacks are worth 10 points.
Aces are worth 1 or 11 points.
Cards 2 through 10 are worth their face value.<br/>
Hit to take another card.
Stand to stop taking cards.
On your first play, you can Double down to increase your bet
but must hit exactly one more time before standing.<br/>
In case of a tie, the bet is returned to the player.
  </fieldset>
</details>
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
  </span>
  <button style="display: flex; margin: 5px auto 0px;" xdh:onevent="New">New game</button>
</fieldset>
"""

HEAD = """
<style>
  @keyframes fadeIn {
    0% {opacity: 0;}
    100% {opacity: 1;}
  }
  .fade-in {
    animation: fadeIn 1s;
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