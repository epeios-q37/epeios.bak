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

import random, builtins
from xml.etree import ElementTree

builtins.open = atlastk.defaultBuiltinsFunction

def main():
  newGame()
  initSVGCards()
  atlastk.launch(CALLBACKS, headContent=HEAD)


def newGame():
  global deck, playerHand, dealerHand, money, bet

  deck = getDeck()
  dealerHand = [54]
  playerHand = [54]
  money = 5000
  bet = 0


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


def displayHands(dom, dealerHand, playerHand, showDealerHand):
    """Show the player's and dealer's cards. Hide the dealer's first
    card if showDealerHand is False."""
    if showDealerHand:
        dom.setContent("dealer_hand_value", getHandValue(dealerHand))
        displayCards(dom, "dealer_cards", dealerHand)
    else:
        dom.setContent("dealer_hand_value", "???")
        # Hide the dealer's first card:
        displayCards(dom, "dealer_cards", [54] + dealerHand[1:])

    # Show the player's cards:
    dom.setContent("player_hand_value", getHandValue(playerHand))
    displayCards(dom, "player_cards", playerHand)

    if len(playerHand) >= 2:
      notify(dom, "Please hit or stand.")    
    else:
      notify(dom, "Please bet.")


def displayMoneyAndBet(dom, money, bet):
  dom.inner("money", '<output class="fade-in">{}</output>'.format(money))
  dom.inner("bet", '<output class="fade-in">{}</output>'.format(bet))

  if bet == 0:
    notify(dom, "Please make a bet!")
  elif len(playerHand) <= 2:
    notify(dom, "Please bet, hit or double down.")
  else:
    notify(dom, "Please hit or stand.")


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


def notify(dom, text):
  """ Displays a text with a little animation to attract attention."""
  dom.inner("output", '<output class="fade-in">{}</output>'.format(text))


def testAction(dom, action):
  playerHandSize = len(playerHand)

  if playerHandSize > 2:
    if action not in ['H', 'S']:
      notify(dom, "You van only hit or stand!")
      return False
  elif playerHandSize == 2:
    if action not in ['H', 'S', 'D']:
      notify(dom, "You can only hit, stand or double down!")
      return False
  elif action not in ['+', '-', 'B']:
    notify(dom, "Yu can only bet!")
    return False

  return True


def acConnect(dom):
  dom.inner("", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="0" height="0">' + SVG_DEFS + '</svg>' + BODY)
  displayHands(dom, dealerHand, playerHand, False)
  displayMoneyAndBet(dom, money, bet)


def acAdd(dom, id):
  global money, bet

  if not testAction(dom, '+'):
    return

  value = int(dom.get_mark(id))

  if value > money:
    notify(dom, "You can't bet that much!")
    return

  bet += value
  money -= value

  atlastk.broadcast_action("DisplayMoneyAndBet")


def acSub(dom, id):
  global money, bet

  if not testAction(dom, '-'):
    return

  value = int(dom.get_mark(id))

  if value > bet:
    notify(dom, "You can't to remove that much!")
    return

  bet -= value
  money += value

  atlastk.broadcast_action("DisplayMoneyAndBet")


def acBet(dom):
  global dealerHand, playerHand

  if not testAction(dom, 'B'):
    return

  dealerHand = [deck.pop(), deck.pop()]
  playerHand = [deck.pop(), deck.pop()]
  atlastk.broadcast_action("DisplayIntermediateHands")


def acHit(dom):
  if not testAction(dom, 'H'):
    return

  playerHand.append(deck.pop())
  atlastk.broadcast_action("DisplayIntermediateHands")


def acStand(dom):
  if not testAction(dom, 'S'):
    return


def acDoubleDown(dom):
  global money, bet, playerHand

  if not testAction(dom, 'D'):
    return

  if money == 0:
    notify(dom, "Not enough money to double down!")
    return

  value = min(bet, money)
  money -= value
  bet += value

  playerHand.append(deck.pop())
  atlastk.broadcast_action("DisplayFinalHands")  
  atlastk.broadcast_action("DisplayMoneyAndBet")


CALLBACKS = {
  "": acConnect,
  "DisplayIntermediateHands": lambda dom: displayHands(dom, dealerHand, playerHand, False),
  "DisplayFinalHands": lambda dom: displayHands(dom, dealerHand, playerHand, True),
  "DisplayMoneyAndBet": lambda dom: displayMoneyAndBet(dom, money, bet),
  "+": acAdd,
  "-": acSub,
  "Bet": acBet,
  "Hit": acHit,
  "Stand": acStand,
  "DoubleDown": acDoubleDown
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
    <button>New game</button>
  </div>
  <fieldset id="output"></fieldset>
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