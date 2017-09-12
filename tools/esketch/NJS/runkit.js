const xml = '\
<?xml version="1.0" encoding="UTF-8"?>\n\
<SomeTag xmlns:xpp="http://q37.info/ns/xpp/" AnAttribute="SomeAttributeValue">\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue (in a string)</SomeOtherTag>\n\
 <xpp:define name="SomeMacro">\n\
  <xpp:bloc>Some macro content !</xpp:bloc>\n\
 </xpp:define>\n\
 <YetAnotherTag YetAnotherAttribute="YetAnotherAttributeValue">\n\
  <xpp:expand select="SomeMacro"/>\n\
 </YetAnotherTag>\n\
</SomeTag>\
';

const stream = require('stream');
const xppq = require('xppq');
var indentLevel = 0;

var out = "";

class StringStream extends stream.Readable {
    constructor(text, options) {
        super(options);
        this.text = text;
    }
    _read() {
        if (!this.eos) {
            this.push(this.text);
            this.eos = true
        } else
            this.push(null);
    }
}

function write(text) {
    out = out + text;
}

function indent(level) {
    while (level--)
        write('.');
}

function callback(token, tag, attribute, value) {
    switch (token) {
        case xppq.tokens.ERROR:
            throw ("ERROR :'" + value + "'\n");
            break;
        case xppq.tokens.DONE:
            console.log(out);
            break;
        case xppq.tokens.START_TAG:
            write("Start tag");
            indent(indentLevel);
            write(": '" + tag + "'\n");
            indentLevel++;
            break;
        case xppq.tokens.ATTRIBUTE:
            write("Attribute");
            indent(indentLevel);
            write(": '" + attribute + "' = '" + value + "'\n");
            break;
        case xppq.tokens.VALUE:
            write("Value    ");
            indent(indentLevel);
            write(": '" + value.trim() + "'\n");
            break;
        case xppq.tokens.END_TAG:
            indentLevel--;
            write("End tag  ");
            indent(indentLevel);
            write(": '" + tag + "'\n");
            break;
        default:
            throw ("Unknown token !!!");
            break;
    }
}

const arg = process.argv[2];

if (arg != undefined)
    id = Number(arg);

function getStream() {
    return new StringStream(xml);
}

write(xppq.componentInfo() + "\n");
write(xppq.wrapperInfo() + "\n");

function test(id) {
    switch (id) {
        case 0:
            write("-----\nNo treatment ; to see the original XML data.\n\n");
            getStream().on('data', (chunk) => write(chunk)).on('error', (err) => console.error('\n>>> ERROR : ' + err + '\n')).on('end', () => console.log(out));
            break;
        case 1:
            write("-----\nUsing the preprocessing stream with a callback, which transforms to lower case.\n\n");
            new xppq.Stream(getStream()).on('data', (chunk) => write(chunk.toString().toLowerCase())).on('error', (err) => console.error('\n>>> ERROR : ' + err + '\n')).on('end', () => console.log(out));
            break;
        case 2:
            write("-----\nXML parsing WITHOUT preprocessing.\n\n");
            xppq.parse(getStream(), callback);
            break;
        case 3:
            write("-----\nXML parsing WITH preprocessing.\n\n");
            xppq.parse(new xppq.Stream(getStream()).on('error', (err) => console.error('>>> ERROR : ' + err)), callback);
            break;
        default:
            throw ("'" + id + "' is not a valid test id ; must be '0' to '3'.");
            break;
    }
}

test(3);  // 0 to 3.
"Click below on the left little triangle...";