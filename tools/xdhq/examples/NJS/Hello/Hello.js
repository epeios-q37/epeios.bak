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

const head = `
<title>"Hello, World !" example</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
`;

const body = `
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text" xdh:onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button xdh:onevent="Submit">Submit</button>
   <button xdh:onevent="Clear">Clear</button>
   <button xdh:onevent="Test">Test</button>
  </div>
 </fieldset>
</div>
`;

function acTest(dom) {
  console.log("!!!!!!!!!!! Test");
  dom.setValue("input", "Test", () => console.log("callback!"));
  dom.drop();
}

const callbacks = {
  "": (dom) => dom.inner("", body,
    () => dom.focus("input")),
  "Submit": (dom) => dom.getValue("input",
    (name) => dom.alert("Hello, " + name + "!",
      () => dom.focus("input"))),
  "Clear": (dom) => dom.confirm("Are you sure ?",
    (answer) => {
      if (answer) {
        dom.setValue("input", "",
          () => dom.hold());
      }
      dom.focus("input");
    }),
  "Test": acTest,
};

atlas.launch(() => new atlas.DOM(), callbacks, head);
