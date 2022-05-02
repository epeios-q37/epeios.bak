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
  global deck, playerHand, dealerHand

  deck = getDeck()
  dealerHand = [deck.pop(), deck.pop()]
  playerHand = [deck.pop(), deck.pop()]


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

    html += ""

    dom.inner(id, html)


def notify(dom, text):
  """ Displays a text with a little animation to attract attention."""
  dom.inner("output", '<output class="fade-in">{}</output>'.format(text))


def acConnect(dom):
  dom.inner("", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="0" height="0">' + SVG_DEFS + '</svg>' + BODY)
  displayHands(dom, dealerHand, playerHand, False)


def acHit(dom):
  playerHand.append(deck.pop())
  atlastk.broadcast_action("Display")


CALLBACKS = {
  "": acConnect,
  "Display": lambda dom: displayHands(dom, dealerHand, playerHand, False),
  "Hit": acHit,
}

BODY = """
<fieldset>
  <span style="display: flex; justify-content: space-around">
    <button xdh:onevent="Hit">Hit</button>
    <button xdh:onevent="Stand">Stand</button>
    <button>Double down</button>
    <span></span>
    <button>New game</button>
  </span>
  <fieldset id="output"></fieldset>
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
  #dealer_cards, #player_cards {
    display: flex;
    flex-direction: column;
  }
</style>
"""

# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()