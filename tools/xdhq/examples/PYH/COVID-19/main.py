"""
MIT License

Copyright (c) 2020 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import sys
sys.path.append("./Atlas.zip")

import atlastk
import json, builtins, urllib.request

HEAD = """
<title>COVID-19</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
"""

BODY="""
<fieldset>
  <svg id="SVG" width="600" height="400" xmlns="http://www.w3.org/2000/svg"/>
</fieldset>
<div style="width: 100%; margin: 0 auto; text-align: center;">
  <span style="padding: 2px; border: 1px dotted;" id="Text"></span>
  <select id="Code" xdh:onevent="Redraw">
    <option value="FRA">France</option>
    <option value="WORLD">Monde</option>
  </select>
  <select id="Cible" xdh:onevent="Redraw">
    <option value="casConfirmes">Cas confirmés</option>
    <option value="deces">Décès</option>
  </select>
  <input type="checkbox" id="Relatif" xdh:onevent="Redraw">Relatif</input>
</div>
"""

with urllib.request.urlopen('https://raw.githubusercontent.com/opencovid19-fr/data/master/dist/chiffres-cles.json') as f:
  data = json.load(f)


def fill(data,code,key):
  set = []
  
  for item in data:
    if item["code"] == code:
      if key in item:
        set.append({"date": item["date"], "value": item[key]})
             
  return set


def draw_set(set,dom):
  seq = [item['value'] for item in set]
  max = builtins.max(seq)
  min = builtins.min(seq)
  
  height = int(dom.getAttribute("SVG", "height"))
  
  min = 0 if min > 0 else min
  
  svg = atlastk.createHTML()
  
  for i in range(len(set)):
    svg.push_tag("rect")
    svg.put_attribute("x", str(i*100/len(set)) + "%");
    svg.put_attribute("y", height-set[i]["value"]*height/max)
    svg.put_attribute("width", str(100/len(set)) + "%");
    svg.put_attribute("height", str(100*set[i]["value"]/max) + "%");
    svg.put_tag_and_value("title", set[i]["date"] + " : " + str(set[i]["value"]))
    svg.pop_tag();

  dom.inner("SVG", svg)
  
  dom.setValue("Text",set[0]["date"] + " - " + set[len(set)-1]["date"])
  
def get_relat(absol):
  relat = []
  
  for i in range(1,len(absol)):
    relat.append({"date": absol[i]["date"],"value": absol[i]["value"]-absol[i-1]["value"]})    
    
  return relat

def draw(dom):
  values = dom.getValues(["Code", "Cible", "Relatif"])
  
  absol = fill(data,values["Code"],values["Cible"])
  
  
  draw_set(get_relat(absol) if values["Relatif"] == "true" else absol,dom)


def a_connect(dom):
  dom.inner("", BODY)
  
  draw(dom)
  
callbacks = {
  "": a_connect,
  "Redraw": lambda dom : draw(dom)
}

atlastk.launch(callbacks, None, HEAD)
    
