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
<button data-xdh-onevent="btSubmit">A button</button>
""" ,

  "password":
"""
<input type="password" data-xdh-onevent="pwSubmit" placeholder="Password"/>
<span>Submitted password: <output id="pwOutput"/></span>
""",

  "checkbox":
"""
<div>
  <p>Specify the type of vehicle you own:</p>
  <input type="checkbox" id="cbBicycle" data-xdh-onevent="cbSelect">
  <label for="cbBicycle">Bicycle</label>
  <br/>
  <input type="checkbox" id="cbCar" data-xdh-onevent="cbSelect">
  <label for="cbCar">Car</label>
  <br/>
  <input type="checkbox" id="cbPirogue" data-xdh-onevent="cbSelect">
  <label for="cbPirogue">Pirogue</label>
</div>
<p />
<div>Last selected vehicle: <output id="cbOutput"/></div>
<p />
<div>
  <button data-xdh-onevent="cbSubmit">Submit</button>
</div>
""",

  "radio":
"""
<p>How to contact you:</p>
<div>
  <input type="radio" name="rdContact" id="rdEmail" data-xdh-onevent="rdSelect">
  <label for="rdEmail">Email</label>

  <input type="radio" name="rdContact" id="rdPhone" data-xdh-onevent="rdSelect">
  <label for="rdTelephone">Phone</label>

  <input type="radio" name="rdContact" id="rdMail" data-xdh-onevent="rdSelect">
  <label for="rdMail">Mail (post)</label>
</div>
<p />
<div>Selected contact method: <output id="rdOutput"/></div>
<p />
<div>
  <button data-xdh-onevent="rdSubmit">Submit</button>
</div>
""",

  "datalist":
"""
<span>
  <input list="dlFlavors" data-xdh-onevent="dlSubmit" placeholder="Select a flavor"/>
  <datalist id="dlFlavors">
      <option value="Caramel"/>
      <option value="Chocolate"/>
      <option value="Vanilla"/>
  </datalist>
</span>
<span>Selected flavor: <output id="dlOutput"/></span>
""",

  "date":
"""
<input type="date" data-xdh-onevent="dtSelect"/>
<span>Selected date: <output id="dtOutput"/></span>
""",

  "color":
"""
<input type="color" id="clColor" data-xdh-onevent="clSelect"/>
<span>Selected color: <output id="clOutput"/></span>
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
<select data-xdh-onevent="slSelect">
  <optgroup label="Théropods">
      <option value="tyrannosaurus">Tyrannosaurus</option>
      <option value="vélociraptor">Velociraptor</option>
      <option value="deinonychus">Deinonychus</option>
  </optgroup>
  <optgroup label="Sauropods"
      <option value="diplodocus">Diplodocus</option>
      <option value="saltasaurus">Saltasaurus</option>
      <option value="apatosaurus">Apatosaurus</option>
  </optgroup>
  <optgroup id="slOthers" label="Others"/>
</select>
<span>
  <label for="slOutput">Selected dinausor: </label>
  <output id="slOutput"/>
</span>
<p />
<div>
  <input id="slInput" data-xdh-onevent="slAdd" placeholder="Other dinausor"/>
  <label>
    <input type="checkbox" data-xdh-onevent="slToggle"/>
    <b>Deactivate</b>
  </label>
</div>
""",

  "CKEditor": 
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
  global current

  current = next(iter(WIDGETS))

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

def dl_shape(flavors):
  html = atlastk.create_HTML()

  for flavor in flavors:
    html.push_tag("option")
    html.put_attribute("value", flavor)
    html.pop_tag()

  return html

dl_flavors = ["Vanilla", "Chocolate", "Caramel"]  

def ac_dl_submit(dom, id):
  global dl_flavors

  flavor = dom.get_content(id)
  dom.set_content(id, "")
  if not flavor in dl_flavors:
    dl_flavors.append(flavor)
    dl_flavors.sort()
    dom.inner("dlFlavors", dl_shape(dl_flavors))
  dom.set_content("dlOutput", flavor)

def sl_embed(other):
  html = atlastk.create_HTML()

  html.push_tag("option")
  html.put_attribute("selected", "selected")
  html.put_value(other)
#  html.pop_tag()

  return html

def ac_sl_add(dom):
#  dom.prepend_layout("autres", "<option>" + dom.getContent("input") + "</option>")
  dom.prepend_layout("slOthers", sl_embed(dom.get_content("slInput")))
  dom.set_content("slInput", "")
  dom.focus("slInput")  

callbacks = {
  "": ac_connect,
  "Select": ac_select,

  "btSubmit": lambda dom: dom.alert("Click on button detected!"),

  "pwSubmit": lambda dom, id: dom.set_content("pwOutput", dom.get_content(id)),

  "cbSelect": lambda dom, id: dom.set_content("cbOutput", "{} ({})".format(id, dom.get_content(id))),
  "cbSubmit": lambda dom: dom.alert(str(dom.get_contents(["cbBicycle", "cbCar","cbPirogue"]))),

  "rdSelect": lambda dom, id: dom.set_content("rdOutput", id),
  "rdSubmit": lambda dom: dom.alert(str(dom.get_contents(["rdEmail", "rdPhone","rdMail"]))),

  "dlSubmit": ac_dl_submit,

  "dtSelect": lambda dom, id: dom.set_content("dtOutput", dom.get_content(id)),

  "clSelect": lambda dom, id: dom.set_content("clOutput", dom.get_content(id)),

  "cpSelect": lambda dom, id: dom.set_content("cpOutput", dom.get_content(id)),

  "rgSlide": lambda dom: dom.set_attribute("rgOutput", "value", (dom.getContent("rgVolume"))),

  "slSelect": lambda dom, id: dom.set_content("slOutput", dom.get_content(id)),
  "slAdd": ac_sl_add,
  "slToggle": lambda dom, id: dom.disable_element("slOthers") if dom.get_content(id) == 'true' else dom.enable_element("slOthers"),

  "ckSubmit": lambda dom, id: dom.set_content("ckOutput", dom.get_content("ckInput")),
}

atlastk.launch(callbacks, None, open("Head.html").read())
