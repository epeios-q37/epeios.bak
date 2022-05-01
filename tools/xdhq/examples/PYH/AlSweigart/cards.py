import atlastk
from xml.etree import ElementTree
import builtins

builtins.open = atlastk.defaultBuiltinsFunction

def main():
  global SVG_DEFS, SVG_CARDS
  ElementTree.register_namespace('','http://www.w3.org/2000/svg')
  ElementTree.register_namespace('xlink','http://www.w3.org/1999/xlink')
  tree = ElementTree.parse("./cards.svg")
  root = tree.getroot()
  children = root.getchildren() 
  SVG_DEFS = ElementTree.tostring(children[0],encoding="unicode")
  SVG_CARDS = children[1].getchildren()
  
  atlastk.launch(CALLBACKS, headContent=HEAD)

def getCardHTML(indice):
  print(indice)
  return '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" height="243.137" width="167.575" viewBox="-.2 -236 167.575 243.137">' + SVG_DEFS + ElementTree.tostring(SVG_CARDS[indice],encoding="unicode") + '</svg>'



def acConnect(dom):
  dom.inner("", BODY)
  dom.setContent("card_1", len(SVG_CARDS))
  for i in range(10,60):
    print(">>>")
    print(getCardHTML(i))
    print("------")
    dom.inner("card_{}".format(i), getCardHTML(i) )
    dom.scroll_to("card_{}".format(i))
  print("END!!!")
#  dom.inner("card2", getCardHTML(0) )
#  dom.inner("card3", getCardHTML(1) )
  #dom.inner("card1", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1">' + getCard() + '</svg>')
  #dom.inner("card2", '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1">' + getCard() + '</svg>')

CALLBACKS = {
  "": acConnect
}

BODY = """
<fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(14)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(14, 28)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(28, 42)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 13 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(42, 56)) + """
  </fieldset>
  <fieldset style="display: flex;">
""" + ( 3 * '<fieldset id="card_{}">yoyo</fieldset>' ).format(*range(56, 60)) + """
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