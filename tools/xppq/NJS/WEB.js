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

const xppq = require('./XPPq.js');
const stream = require("stream");

const head = `
<title>XML preprocessor with Node.js</title>
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
    color:darkblue;
  }

  .AttributeValue {
    background-color:lightblue;
  }

  .Value {
    background-color:lightgrey;
  }

</style>
`;

const body = `<fieldset>
  <textarea name="" id="input" cols="30" rows="10"></textarea>
  <fieldset>
    <button xdh:onevent="Raw">Raw</button>
    <button xdh:onevent="Preprocessed">Preprocessed</button>
    <button xdh:onevent="Without">Parsing without preprocessing</button>
    <button xdh:onevent="With">Parsing with preprocessing</button>
    <fieldset>
       <output id="output">
        <span style="font-style: oblique;">Click one of above button.</span>
       </output>
    </fieldset>
  </fieldset>
</fieldset>
`;

const xml = `<?xml version="1.0" encoding="UTF-8"?>
<SomeTag xmlns:xpp="http://q37.info/ns/xpp/" AnAttribute="SomeAttributeValue">
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue (in a string)</SomeOtherTag>
 <xpp:define name="SomeMacro">
  <xpp:bloc>Some macro content !</xpp:bloc>
 </xpp:define>
 <YetAnotherTag YetAnotherAttribute="YetAnotherAttributeValue">
  <xpp:expand select="SomeMacro"/>
 </YetAnotherTag>
</SomeTag>
`;



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
    this.level = 0;
  }
  write(text) {
    this.out += text;
  }
  indent() {
    let level = this.level;

    while (level--)
      this.write(' ');
  }}

function callback(token, tag, attribute, value, context) {
  switch (token) {
    case xppq.tokens.ERROR:
      context.dom.inner("output", '<pre style="color: darkred;">' + value + "</pre>");
      break;
    case xppq.tokens.DONE:
      context.dom.inner("output", "<pre>" + context.out + "</pre>");
      break;
    case xppq.tokens.START_TAG:
      context.write("Start tag:");
      context.indent();
      context.write('<span class="Tag">' + tag + '</span></br>');
      context.level++;
      break;
    case xppq.tokens.ATTRIBUTE:
      context.write("Attribute:");
      context.indent();
      context.write('<span class="AttributeName">' + attribute + '</span> = <span class="AttributeValue">' + value + '</span></br>');
      break;
    case xppq.tokens.VALUE:
      context.write("Value    :");
      context.indent();
      context.write('<span class="Value">' + value.trim() + '</span></br>');
      break;
    case xppq.tokens.END_TAG:
      context.level--;
      context.write("End tag  :");
      context.indent();
      context.write('<span class="Tag">' + tag + '</span></br>');
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

function escapeHtml(unsafe)
{
    return "<pre class='lang-xml'>" + unsafe
         .replace(/&/g, "&amp;")
         .replace(/</g, "&lt;")
         .replace(/>/g, "&gt;")
         .replace(/"/g, "&quot;")
         .replace(/'/g, "&#039;") + "</pre>";
 }

const callbacks = {
  "": (dom) => dom.inner("", body,
     () => dom.setValue("input", xml,
      () => dom.end("output", "<pre>" + xppq.componentInfo() + "</pre><pre>" + xppq.wrapperInfo() + "</pre>"))),
  "Raw": (dom) => dom.getValue("input",
    (value) => streamToString(new StringStream(value),
      (result) => dom.inner("output", escapeHtml(result),
        () => dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);")
        ))),
  "Preprocessed": (dom) => dom.getValue("input",
    (value) => streamToString(new StringStream(value).pipe(new xppq.Stream()),
      (result) => dom.inner("output", escapeHtml(result),
      () => dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);")))),
  "Without": (dom) => {
    dom_ = dom;
    dom.getValue("input",
      (value) => streamToString(new StringStream(value),
        (result) => xppq.parse(new StringStream(result), callback, new Context(dom))));
    },
  "With": (dom) => {
    dom_ = dom;
    dom.getValue("input",
      (value) => streamToString(new StringStream(value),
        (result) => xppq.parse(new StringStream(result).pipe(new xppq.Stream()), callback, new Context(dom))));
    },
  };

atlas.launch(() => new atlas.DOM(), callbacks, head);