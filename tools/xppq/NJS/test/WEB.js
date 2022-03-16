/*
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
*/

/*
  This is the one-file version, in which the content of
  the 'Head.html' and the 'Main.html' files are embedded.
*/

var atlas;

if (process.env.Q37_EPEIOS) {
  let epeiosPath = "";

  if (process.platform === 'win32')
    epeiosPath = "h:/hg/epeios/";
  else
    epeiosPath = process.env.Q37_EPEIOS;

  atlas = require(epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js");
} else {
  atlas = require('atlastk');
}

const xppq = require('../XPPq.js');
const stream = require("stream");

const head = `
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
</style>
<script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.14/ace.js" integrity="sha512-6ts6Fu561/yzWvD6uwQp3XVYwiWNpWnZ0hdeQrETqtnQiGjTfOS06W76aUDnq51hl1SxXtJaqy7IsZ3oP/uZEg==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
`;

const body = `
<fieldset>
  <fieldset>
    <legend>Input</legend>
    <div id="input" style="font-size: initial; resize: horizontal;"/>
  </fieldset>
  <fieldset style="display: flex;flex-direction: row;justify-content: space-evenly;">
    <legend>Actions</legend>
    <fieldset style="display: flex; align-items: center;">
      <legend>Examples</legend>
      <select xdh:onevent="Select">
        <option value="basicExample">Basic</option>
        <option value="ifExample">Using 'ifeq'</option>
        <option value="substitutionExample">Substitution</option>
        <option value="userExample">Yours</option>
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
      <output id="output">
      <span style="font-style: oblique;">Enter XML code or select an exemple,<br/>and/or click one of above button.</span>
      </output>
  </fieldset>
</fieldset>
`;

const basicExample = `
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

const ifExample = `
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

const substitutionExample = `
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

const userExample = `
<?xml version="1.0" encoding="UTF-8"?>
<Root xmlns:xpp="http://q37.info/ns/xpp/">
  Type your own XML code here.
</Root>
`;

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

function handleError(dom, error)
{
  dom.inner("output", '<pre style="color: darkred;">' + error + "</pre>");
}

let initScript = `
var editor = ace.edit("input",
  {
    theme: "ace/theme/monokai",
    mode: "ace/mode/xml",
    maxLines: 50,
    printMargin: false
  });
`

function callback(token, tag, attribute, value, context) {
  switch (token) {
    case xppq.tokens.ERROR:
      handleError(context.dom, value);
      break;
    case xppq.tokens.DONE:
      context.dom.inner("output", "<pre><ul>" + context.out + "</ul></pre>");
      break;
    case xppq.tokens.START_TAG:
      context.write('<fieldset style="border: 5px ridge; margin: 2px;"><li><fieldset><legend>Tag</legend><span class="Tag">' + tag + '</span></fieldset><ul>');
      break;
    case xppq.tokens.ATTRIBUTE:
      context.write('<li><fieldset><legend>Attribute</legend><span class="AttributeName">' + attribute + '</span>: <span class="AttributeValue">' + value + '</span></fieldset></li>');
      break;
    case xppq.tokens.VALUE:
      context.write('<li><fieldset><legend>Value</legend><span class="Value">' + value + '</span></fieldset></li>');
      break;
    case xppq.tokens.END_TAG:
      context.write('</ul></li></fieldset>');
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

const callbacks = {
  "": (dom) => dom.inner("", body,
    () => dom.executeVoid(initScript + getExampleScript('basicExample'),
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
};

atlas.launch(() => new atlas.DOM(), callbacks, head);