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
  rootChildren = list(tree.getroot())
  SVG_DEFS = ElementTree.tostring(rootChildren[0],encoding="unicode")
  SVG_CARDS = [
      ElementTree.tostring(child, encoding="unicode")
      for child in list(rootChildren[1])
  ]


def getSVGCard(indice):
  return '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" height="243.137" width="167.575" viewBox="-.2 -236 167.575 243.137">' + SVG_CARDS[indice] + '</svg>'


def acConnect(dom):
  dom.inner("", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="0" height="0">' + SVG_DEFS + '</svg>' + BODY)
  dom.setValue("card_54", len(SVG_CARDS))
  for i in range(55):
    dom.inner("card_{}".format(i), getSVGCard(i) )
    dom.scrollTo("card_{}".format(i))
    dom.flush()


CALLBACKS = {
  "": acConnect
}

BODY = """
<fieldset>
""" + ( 4 * ( """
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}"></fieldset>' ) + """
  </fieldset>
""" ) + """
  <fieldset style="display: flex;">
""" + ( 3 * '<fieldset id="card_{}"></fieldset>' ) + """
  </fieldset>
</fieldset>
""" ).format(*range(55))

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