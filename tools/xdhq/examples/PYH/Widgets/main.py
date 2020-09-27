"""
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import cgi
import atlastk

WIDGETS = {
  "button":
"""
<button data-xdh-onevent="btSubmit">Mon bouton!</button>
""" ,

  "checkbox":
"""
<p>Indiquez le type de véhicule que vous possédez :</p>
<input type="checkbox" id="cbBicyclette" data-xdh-onevent="cbSelect">
<label for="cbBicyclette">Bicyclette</label>
<br/>
<input type="checkbox" id="cbAutomobile" data-xdh-onevent="cbSelect">
<label for="cbAutomobile">Automobile</label>
<br/>
<input type="checkbox" id="cbPirogue" data-xdh-onevent="cbSelect">
<label for="cbPirogue">Pirogue</label>
<br>
<fieldset>
  <span>Dernier élément cliqué : </span>
  <output id="cbOutput"/>
</fieldset>
<div>
  <button data-xdh-onevent="cbSubmit">Envoyer</button>
</div>
""",

  "radio":
"""
<p>Veuillez choisir la meilleure méthode pour vous contacter :</p>
<div>
  <input type="radio" name="rdContact" id="rdEmail" data-xdh-onevent="rdSelect">
  <label for="rdEmail">Email</label>

  <input type="radio" name="rdContact" id="rdTelephone" data-xdh-onevent="rdSelect">
  <label for="rdTelephone">Téléphone</label>

  <input type="radio" name="rdContact" id="rdCourrier" data-xdh-onevent="rdSelect">
  <label for="rdCourrier">Courrier</label>
</div>
<fieldset>
  <span>Élément sélectionné : </span>
  <output id="rdOutput"/>
</fieldset>
<div>
  <button data-xdh-onevent="rdSubmit">Envoyer</button>
</div>
""",

  "datalist":
"""
<div>
  <span>Parfum sélectionné :</span>
  <output id="dlOutput"/>
</div>
<div>
  <label for="dlParfum">Indiquez un parfum :</label>
  <input list="dlParfums" id="dlParfum" data-xdh-onevent="dlSubmit"/>
  <datalist id="dlParfums">
      <option value="Caramel"/>
      <option value="Chocolat"/>
      <option value="Vanille"/>
  </datalist>
</div>
""",

  "color":
"""
<input type="color" id="clColor" "data-xdh-onevent="clSelect"/>
<output id="clOutput"/>
""",

  "range":
"""
<label for="rgVolume">Volume</label>
<input type="range" id="rgVolume" min="0" max="10" data-xdh-onevent="rgSlide">
<meter id="rgOutput"
  min="0" max="10"
  low="5" high="8" optimum="4"
  value="5">
</meter>
""",

  "select":
"""
<p>Veuillez choisir un dinosaure :</p>
<select id="slDinosaure" data-xdh-onevent="slSelect">
    <optgroup label="Théropodes">
        <option value="tyrannosaure">Tyrannosaure</option>
        <option value="vélociraptor">Vélociraptor</option>
        <option value="deinonychus">Deinonychus</option>
    </optgroup>
    <optgroup label="Sauropodes"
        <option value="diplodocus">Diplodocus</option>
        <option value="saltasaurus">Saltasaurus</option>
        <option value="apatosaurus">Apatosaurus</option>
    </optgroup>
    <optgroup id="slAutres" label="Autres"/>
</select>
<fieldset>
  <label for="slOutput">Élément sélectionné : </label>
  <output id="slOutput"/>
</fieldset>
<div>
  <button data-xdh-onevent="slSubmit">Envoyer</button>
</div>
<div>
  <input id="slInput" data-xdh-onevent="slAdd"/>
  <button data-xdh-onevent="slAdd">Ajouter</button>
  <label>
    <input type="checkbox" data-xdh-onevent="slToggle"/>
    <span><b>Désactiver</b></span>
  </label>
</div>
""",

  "ckEditor": 
"""
<textarea id="ckInput" data-xdh-widget="ckeditor|{entities: false, enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false}|val\(\)|"></textarea>
<button data-xdh-onevent="ckSubmit">Submit</button>
<fieldset>
  <output id="ckOutput"/>
</fieldset>
"""

}

current = next(iter(WIDGETS))

def handle(label,html):
  return (
  f"<option value={label}>{label}</option>",
  f"""
<fieldset class="hidden" id="{label}">
  <fieldset>{html}</fieldset>
  <pre class="lang-html">{cgi.escape(html)}</pre>
</fieldset>
  """)


def ac_connect(dom):
  dom.inner("", open("Main.html").read())

  list, widgets = "", ""

  for k, v in WIDGETS.items():
    html = handle(k, v) 
    list += html[0]
    widgets += html[1]

  dom.inner("List",list)
  dom.inner("Widgets", widgets)
  dom.execute_void("document.querySelectorAll('pre').forEach((block) => {hljs.highlightBlock(block);});")
  dom.remove_class(current, "hidden")

def ac_select(dom,id):
  global current

  dom.add_class(current,"hidden")
  current = dom.get_content(id)
  dom.remove_class(current, "hidden")

def dl_shape(parfums):
  html = atlastk.create_HTML()

  for parfum in parfums:
    html.push_tag("option")
    html.put_attribute("value", parfum)
    html.pop_tag()

  return html

dl_parfums = ["Vanille", "Chocolat", "Caramel"]  

def ac_dl_submit(dom):
  global dl_parfums

  parfum = dom.get_content("dlParfum")
  dom.set_content("dlParfum", "")
  if not parfum in dl_parfums:
    dl_parfums.append(parfum)
  dl_parfums.sort()
  dom.inner("dlParfums", dl_shape(dl_parfums))
  dom.set_content("dlOutput", parfum)

def sl_embed(other):
  html = atlastk.create_HTML()

  html.push_tag("option")
  html.put_value(other)
#  html.pop_tag()

  return html

def ac_sl_add(dom):
#  dom.prepend_layout("autres", "<option>" + dom.getContent("input") + "</option>")
  dom.prepend_layout("slAutres", sl_embed(dom.getContent("slInput")))
  dom.set_content("slInput", "")
  dom.focus("slInput")  

callbacks = {
  "": ac_connect,
  "Select": ac_select,

  "btSubmit": lambda dom: dom.alert("Click on button detected!"),

  "cbSelect": lambda dom, id: dom.set_content("cbOutput", "{} ({})".format(id, dom.get_content(id))),
  "cbSubmit": lambda dom: dom.alert(str(dom.get_contents(["cbBicyclette", "cbAutomobile","cbPirogue"]))),

  "rdSelect": lambda dom, id: dom.set_content("rdOutput", id),
  "rdSubmit": lambda dom: dom.alert(str(dom.get_contents(["rdEmail", "rdTelephone","rdCourrier"]))),

  "dlSubmit": ac_dl_submit,

  "clSelect": lambda dom, id: dom.set_content("clOutput", dom.get_content(id)),

  "rgSlide": lambda dom: dom.set_attribute("rgOutput", "value", (dom.getContent("rgVolume"))),

  "slSelect": lambda dom, id: dom.set_content("slOutput", dom.get_content(id)),
  "slSubmit": lambda dom: dom.alert(dom.get_content("slDinosaure")),
  "slAdd": ac_sl_add,
  "slToggle": lambda dom, id: dom.disable_element("slAutres") if dom.get_content(id) == 'true' else dom.enable_element("slAutres"),

  "ckSubmit": lambda dom, id: dom.set_content("ckOutput", dom.get_content("ckInput")),
}

atlastk.launch(callbacks, None, open("Head.html").read())
