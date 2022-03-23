var atlas;
var xppq;

if (process.env.Q37_EPEIOS) {
  let epeiosPath = "";

  if (process.platform === 'win32')
    epeiosPath = "h:/hg/epeios/";
  else
    epeiosPath = process.env.Q37_EPEIOS;

  atlas = require(epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js");
  xppq = require('../XPPq.js');
} else {
  atlas = require('atlastk');
  xppq = require('./xppq-node/XPPq.js');
}

const stream = require("stream");

function getExampleScript(variableName) {
  return "editor.setValue(`" + eval(variableName).trim() + "`, -1);undefined;";
}

function streamToString(stream, cb) {
  const chunks = [];
  stream.on('data', (chunk) => {
    chunks.push(chunk.toString());
  });
  stream.on('end', () => {
    cb(chunks.join(''));
  });
}

class Context {
  constructor(dom) {
    this.dom = dom;
    this.out = "";
  }
  write(text) {
    this.out += text;
  }
}

function handleError(dom, error) {
  dom.inner("output", '<pre style="color: darkred;">' + error + "</pre>");
}

function callback(token, tag, attribute, value, context) {
  switch (token) {
    case xppq.tokens.ERROR:
      handleError(context.dom, value);
      break;
    case xppq.tokens.START_TAG:
      context.write(
        '<li><details open="true"><summary><fieldset><legend>Tag</legend><span class="Tag">' + tag + '</span></fieldset></summary><ul>');
      break;
    case xppq.tokens.ATTRIBUTE:
      context.write('<li><fieldset><legend>Attribute</legend><span class="AttributeName">' + attribute + '</span>: <span class="AttributeValue">' + value + '</span></fieldset></li>');
      break;
    case xppq.tokens.VALUE:
      context.write('<li><fieldset><legend>Value</legend><span class="Value">' + value + '</span></fieldset></li>');
      break;
    case xppq.tokens.END_TAG:
      context.write('</ul></details></li>');
      break;
    case xppq.tokens.DONE:
      context.dom.inner("output", "<pre><ul>" + context.out + "</ul></pre>");
      break;
    default:
      throw new Error("Unknown token !!!");
      break;
  }
}

class StringStream extends stream.Readable {
  constructor(text, options) {
    super(options);
    this.text = text;
  }
  _read() {
    if (!this.eos) {
      this.push(this.text);
      this.eos = true
    } else {
      this.push(null);
    }
  }
}

function escapeHtmlAndFormat(unsafe) {
  return "<pre class='lang-xml'>" + unsafe
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;") + "</pre>";
}

const BODY = `
<iframe style="width: 100%; border: none;" src="FaaSDesc.php?text=VGhpcyAqTm9kZS5qcyogYXBwbGljYXRpb24gdXNlcyBhbiBbKmFkZG9uKl0oaHR0cHM6Ly9ub2RlanMub3JnL2FwaS9hZGRvbnMuaHRtbCkgaW1wbGVtZW50aW5nIGFuICpYTUwqIHBhcnNlciBhbmQgcHJlcHJvY2Vzc29yLiBEZXZlbG9wZWQgaW4gKkMqLypDKyssKiB0aGlzIGFkZG9uIGlzIG11Y2ggbW9yZSBmYXN0ZXIsIHJlbGlhYmxlIGFuZCByZXNvdXJjZS1lZmZpY2llbnQsIGFuZCB0aGVyZWZvcmUgbW9yZSBlbnZpcm9ubWVudC1mcmllbmRseSwgdGhhbiBpZiBpdCBoYWQgYmVlbiBkZXZlbG9wZWQgaW4gKkphdmFTY3JpcHQqLiAgClRoZSAqUmF3KiBidXR0b24gZGlzcGxheXMgdGhlIGNvbnRlbnQgb2YgdGhlICpJbnB1dCogZmllbGQgaW4gdGhlICpPdXRwdXQqIGZpZWxkLiBUaGUgKlByZXByb2Nlc3NlZCogYnV0dG9uIGRpc3BsYXlzIHRoZSByZXN1bHQgb2YgdGhlIHByZXByb2Nlc3NpbmcuIFRoZSAqdy9vIHByZXByb2Nlc3NpbmcqIGFuZCAqd2l0aCBwcmVwcm9jZXNzaW5nKiBidXR0b25zIGZvcm1hdCB0aGUgKlhNTCogZGF0YSB1c2luZyB0aGUgcGFyc2VyLCByZXNwZWN0aXZlbHkgd2l0aG91dCBhbmQgd2l0aCBwcmVwcm9jZXNzaW5nLiBZb3UgY2FuIHBsYWNlIHlvdXIgb3duIGNvbnRlbnQgaW4gdGhlICpJbnB1dCogZmllbGQuICAKQXMgeW91IGNhbiBzZWUgaW4gdGhlIFsqV0VCLmpzKl0oaHR0cHM6Ly9xMzcuaW5mby9zLzd6d3R0M2g0KSBmaWxlLCB0aGlzIGFkZG9uIGlzIHVzZWQgbGlrZSBhbnkgb3RoZXIgKk5vZGUuanMqIG1vZHVsZS4gWW91IHdpbGwgYWxzbyBmaW5kIGluIHRoZSBzYW1lIHJlcG9zaXRvcnkgdGhlIGNvbXBsZXRlICpDKi8qQysrKiBjb2RlIGNvcnJlc3BvbmRpbmcgdG8gdGhpcyBhZGRvbi4="></iframe><fieldset>
  <fieldset>
    <legend>Input</legend>
    <div id="input" style="font-size: initial; resize: horizontal;"/>
  </fieldset>
  <fieldset style="display: flex;flex-direction: row;justify-content: space-evenly;">
    <legend>Actions</legend>
    <fieldset style="display: flex; align-items: center;">
      <legend>Examples</legend>
      <select xdh:onevent="Select">
        <option value="BASIC_EXAMPLE">Basic</option>
        <option value="IF_EXAMPLE">Using 'ifeq'</option>
        <option value="SUBSTITUTION_EXAMPLE">Substitution</option>
        <option value="USER_EXAMPLE">Yours</option>
      </select>
    </fieldset>
    <fieldset style="display: flex; flex-direction: column;">
      <legend>XML</legend>
      <button xdh:onevent="Raw">Raw</button>
      <button xdh:onevent="Preprocessed">Preprocessed</button>
    </fieldset>
    <fieldset style="display: flex;flex-direction: column;">
      <legend>Parsing</legend>
      <button xdh:onevent="Without">w/o preprocessing</button>
      <button xdh:onevent="With">with preprocessing</button>
    </fieldset>
  </fieldset>
  <fieldset>
    <legend>Output</legend>
      <output id="output" xdh:onevent="change|Test">
      <span style="font-style: oblique;">Enter XML code or select an example,<br/>andr click one of above buttons.</span>
      </output>
  </fieldset>
</fieldset>
`;

const INIT_SCRIPT = `
var editor = ace.edit("input",
  {
    theme: "ace/theme/monokai",
    mode: "ace/mode/xml",
    maxLines: 50,
    printMargin: false
  });
`

const callbacks = {
  "": (dom) => dom.inner("", BODY,
    () => dom.executeVoid(INIT_SCRIPT + getExampleScript('BASIC_EXAMPLE'),
      () => dom.end("output", "<hr/><pre>" + xppq.componentInfo() + "</pre><pre>" + xppq.wrapperInfo() + "</pre>"))),
  "Select": (dom, id) => dom.getValue(id,
    (exampleName) => dom.executeVoid(getExampleScript(exampleName))),
  "Raw": (dom) => dom.executeString("editor.getValue();",
    (value) => streamToString(new StringStream(value),
      (result) => dom.inner("output", escapeHtmlAndFormat(result),
        () => dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);")
      ))),
  "Preprocessed": (dom) => dom.executeString("editor.getValue();",
    (value) => streamToString(new StringStream(value).pipe(new xppq.Stream()).on("error", (e) => handleError(dom, e)),
      (result) => dom.inner("output", escapeHtmlAndFormat(result),
        () => dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);")))),
  "Without": (dom) => {
    dom.executeString("editor.getValue();",
      (value) => streamToString(new StringStream(value),
        (result) => xppq.parse(new StringStream(result), callback, new Context(dom))));
  },
  "With": (dom) => {
    dom.executeString("editor.getValue();",
      (value) => streamToString(new StringStream(value),
        (result) => xppq.parse(new StringStream(result).pipe(new xppq.Stream()).on("error", (e) => handleError(dom, e)), callback, new Context(dom))));
  },
  "Test": (dom) => dom.alert("Yo!"),
};

const HEAD = `
<title>Native XML preprocessor and parser with Node.js</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<!-- HTML Syntax highlighting -->
<link rel="stylesheet" href="//cdnjs.cloudflare.com/ajax/libs/highlight.js/10.1.2/styles/default.min.css">
<script src="//cdnjs.cloudflare.com/ajax/libs/highlight.js/10.1.2/highlight.min.js"></script>
<script>"document.querySelectorAll('pre').forEach((block) => {hljs.highlightBlock(block);});"</script>
<style>
  .Tag {
    font-weight: bold;
  }
  .AttributeName {
    font-weight: bold;
    color:darksalmon;
  }
  .AttributeValue {
    background-color:lightsalmon;
  }
  .Value {
    background-color:lightgrey;
  }
  ul {
    padding-left: 10px;
    list-style-type: none;
  }
  button {
    margin: 2px;
  }
  details summary {
    cursor: pointer;
  }
  details summary > * {
    display: inline;
  }
</style>
<script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.14/ace.js" integrity="sha512-6ts6Fu561/yzWvD6uwQp3XVYwiWNpWnZ0hdeQrETqtnQiGjTfOS06W76aUDnq51hl1SxXtJaqy7IsZ3oP/uZEg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
`;

atlas.launch(() => new atlas.DOM(), callbacks, HEAD);

const BASIC_EXAMPLE = `
<?xml version="1.0" encoding="UTF-8"?>
<SomeTag
  xmlns:xpp="http://q37.info/ns/xpp/"
  AnAttribute="SomeAttributeValue">
  <SomeOtherTag
   AnotherAttribute="AnotherAttributeValue">
    Tag value
  </SomeOtherTag>
  <xpp:define
   name="SomeMacro">
    <xpp:bloc>Some macro content !</xpp:bloc>
  </xpp:define>
  <YetAnotherTag
   YetAnotherAttribute="YetAnotherAttributeValue">
    <xpp:expand select="SomeMacro"/>
  </YetAnotherTag>
</SomeTag>
`;

const IF_EXAMPLE = `
<?xml version="1.0" encoding="UTF-8"?>
<RootTag xmlns:xpp="http://q37.info/ns/xpp/">
  <xpp:define
   name="MacroWithVar">
    <VarValueTest>
      <xpp:ifeq
       select="MyVar"
       value="VarFirstValue">
        <xpp:bloc>At first, 'MyVar' value is 'VarFirstValue'.</xpp:bloc>
      </xpp:ifeq>
      <xpp:ifeq
       select="MyVar"
       value="VarSecondValue">
        <xpp:bloc>Then 'MyVar' value is 'VarSecondValue'.</xpp:bloc>
      </xpp:ifeq>
    </VarValueTest>
  </xpp:define>
  <xpp:set
   name="MyVar"
   value="VarFirstValue"/>
  <xpp:expand
   select="MacroWithVar"/>
  <xpp:set
   name="MyVar"
   value="VarSecondValue"/>
  <xpp:expand
   select="MacroWithVar"/>
</RootTag>
`;

const SUBSTITUTION_EXAMPLE = `
<?xml version="1.0" encoding="UTF-8"?>
<Root xmlns:xpp="http://q37.info/ns/xpp/">
  <xpp:set
   name="FirstVar"
   value="FirstVarValue"/>
  <xpp:set
   name="SecondVar"
   value="SecondVarValue"/>
  <xpp:bloc marker="$">
    <Tag
     AttributeOne="Attribute for the $FirstVar$"
     AttributeTwo="Attribute for the $SecondVar$"/>
  </xpp:bloc>
</Root>
`;

const USER_EXAMPLE = `
<?xml version="1.0" encoding="UTF-8"?>
<Root xmlns:xpp="http://q37.info/ns/xpp/">
  Type your own XML code here.
</Root>
`;
