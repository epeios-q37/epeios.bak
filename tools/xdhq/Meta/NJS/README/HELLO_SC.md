```javascript
const atlastk = require('atlastk');

const BODY = `
<fieldset>
 <input id="Input" data-xdh-onevent="Submit" value="World"/>
 <button data-xdh-onevent="Submit">Hello</button>
 <hr/>
 <fieldset>
  <output id="Output">Greetings displayed here!</output>
 </fieldset>
</fieldset>
`;

const callbacks = {
 "": (dom, id) => dom.inner("", BODY,
  () => dom.focus("Input")),
 "Submit": (dom, id) => dom.getValue("Input",
  (name) => dom.setValue("Output", "Hello, " + name + "!",
   () => dom.setValue("Input", "",
    () => dom.focus("Input")))),
};

atlastk.launch(() => new atlastk.DOM(), callbacks);
```