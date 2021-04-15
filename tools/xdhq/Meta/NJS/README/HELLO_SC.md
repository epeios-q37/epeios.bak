```javascript
const atlas = require( 'atlastk' );

const body = `
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
 "": (dom, id) => dom.inner("", body,
  () => dom.focus("input")),
 "Submit": (dom, id) => dom.getValue("input",
  (name) => dom.setValue("Hello, " + name + "!",
   () => dom.set_value("Input", "",
    () => dom.focus("input")))),
};

atlas.launch(() => new atlas.DOM(), callbacks);
```
