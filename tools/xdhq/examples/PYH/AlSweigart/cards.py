import atlastk
from xml.etree import ElementTree
import builtins

builtins.open = atlastk.defaultBuiltinsFunction

def main():
  initSVGCards()
  Head = HEAD
  atlastk.launch(CALLBACKS, headContent=Head)


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


def acConnect(dom):
  dom.inner("", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="0" height="0">' + SVG_DEFS + '</svg>' + BODY)
  dom.setContent("card_1", len(SVG_CARDS))
  for i in range(55):
    dom.inner("card_{}".format(i), getCardHTML(i) )
    dom.scroll_to("card_{}".format(i))
  print("END!!!")


CALLBACKS = {
  "": acConnect
}

BODY = """
<fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(13)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(13, 26)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(26, 39)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(39, 52)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 3 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(52, 55)) + """
  </fieldset>
</fieldset>
"""

HEAD = """
<style>
.hidden {
  display: none;
}
</style>
"""


# If the program is run (instead of imported), run the game:
if __name__ == '__main__':
  main()