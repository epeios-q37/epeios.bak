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
`;

const body = `<fieldset>
  <textarea name="" id="input" cols="30" rows="10"></textarea>
  <fieldset>
    <button xdh:onevent="Raw">Raw</button>
    <button xdh:onevent="Preprocessed">Preprocessed</button>
    <button xdh:onevent="With">Parsing without preprocessing</button>
    <button xdh:onevent="Without">Parsing with preprocessing</button>
    <fieldset>
       <output id="output">Output</output>
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

var dom_;
var out = "";
var indentLevel = 0;

function write(text) {
  out = out + text;
}

function indent(level) {
  while (level--)
    write(' ');
}

function callback(token, tag, attribute, value) {
  switch (token) {
    case xppq.tokens.ERROR:
      throw new Error("ERROR :'" + value + "'\n");
      break;
    case xppq.tokens.DONE:
      out = "coucou";
      dom_.setValue("output", out, () => dom_.setValue("input", "toto"));
      //dom_.inner("output", "<div>" + out + "</div>");
      console.log("Hey!");
      break;
    case xppq.tokens.START_TAG:
      write("Start tag:");
      indent(indentLevel);
      write("'" + tag + "'\n");
      indentLevel++;
      break;
    case xppq.tokens.ATTRIBUTE:
      write("Attribute:");
      indent(indentLevel);
      write("'" + attribute + "' = '" + value + "'\n");
      break;
    case xppq.tokens.VALUE:
      write("Value    :");
      indent(indentLevel);
      write("'" + value.trim() + "'\n");
      break;
    case xppq.tokens.END_TAG:
      indentLevel--;
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
    return "<div>" + unsafe
         .replace(/&/g, "&amp;")
         .replace(/</g, "&lt;")
         .replace(/>/g, "&gt;")
         .replace(/"/g, "&quot;")
         .replace(/'/g, "&#039;") + "</div>";
 }

function acRaw(dom) {
  dom.getValue("input", (value) => streamToString(new StringStream(value).pipe(new xppq.Stream()),(result) => dom.inner("output", escapeHtml(result))), true);
// dom.getValue("input", (value) =>  dom.inner("output", escapeHtml(value)));
}


const callbacks = {
  "": (dom) => dom.inner("", body,
     () => dom.setValue("input", xml)),
  "Raw": (dom) => dom.getValue("input",
    (value) => streamToString(new StringStream(value),
      (result) => dom.inner("output", escapeHtml(result))), true),
  "Preprocessed": (dom) => dom.getValue("input",
    (value) => streamToString(new StringStream(value).pipe(new xppq.Stream()),
      (result) => dom.inner("output", escapeHtml(result))), true),
  "With": (dom) => {
    dom_ = dom;
    dom.getValue("input",
      (value) => streamToString(new StringStream(value),
        (result) => xppq.parse(new StringStream(result), callback)), true);
    },
  };

atlas.launch(() => new atlas.DOM(), callbacks, head);