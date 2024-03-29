import info.q37.xppq.*;

import info.q37.atlas.*;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

public class WEB extends Atlas {
  private static String parse(InputStream stream) {
    XPPqData data = new XPPqData();
    XPPqParser parser = new XPPqParser(stream);
    StringBuilder result = new StringBuilder();

    int token = parser.parse(data);

    while (token != XPPqParser.DONE) {
      switch (token) {
        case XPPqParser.START_TAG:
          result.append(
              "<li><details open=\"true\"><summary><fieldset><legend>Tag</legend><span class=\"Tag\">" + data.tagName + "</span></fieldset></summary><ul>");
          break;
        case XPPqParser.ATTRIBUTE:
          result.append("<li><fieldset><legend>Attribute</legend><span class=\"AttributeName\">" + data.attributeName + "</span>: <span class=\"AttributeValue\">" + data.value + "</span></fieldset></li>");
          break;
        case XPPqParser.VALUE:
          result.append(
              "<li><fieldset><legend>Value</legend><span class=\"Value\">" + data.value + "</span></fieldset></li>");
          break;
        case XPPqParser.END_TAG:
          result.append("</ul></details></li>");
          break;
        default:
          System.err.println("Unknown token!");
          System.exit(1);
      }

      token = parser.parse(data);
    }

    return "<pre><ul>" + result.toString() + "</ul></pre>";
  }

  private static InputStream stringToStream(String string) {
    return new ByteArrayInputStream(string.getBytes());
  }

  private static String streamToString(InputStream stream) throws IOException {
    int c = 0;
    StringBuilder result = new StringBuilder();

    while ((c = stream.read()) != -1) {
      result.append((char) c);
    }

    return result.toString();
  }

  private static String escapeHtml(String s) {
    StringBuilder out = new StringBuilder(Math.max(16, s.length()));

    for (int i = 0; i < s.length(); i++) {
      char c = s.charAt(i);
      if (c > 127 || c == '"' || c == '\'' || c == '<' || c == '>' || c == '&') {
        out.append("&#");
        out.append((int) c);
        out.append(';');
      } else {
        out.append(c);
      }
    }
    return out.toString();
  }

  private String escapeHtmlAndFormat(String unsafe) {
    return "<pre class='lang-xml'>" + escapeHtml(unsafe) + "</pre>";
  }

  private void acConnect(DOM dom) {
    dom.inner("", BODY);
    dom.executeVoid(INIT_SCRIPT + "editor.setValue(`" + BASIC_EXAMPLE + "`, -1);undefined;");
    dom.end("output", "<hr/><pre>" + XPPq.componentInfo() + "</pre><pre>" + XPPq.wrapperInfo() + "</pre>");
  }

  private void acSelect(DOM dom, String id) {
    String xml = "";

    switch (dom.getContent(id)) {
      case "basicExample":
        xml = BASIC_EXAMPLE;
        break;
      case "ifExample":
        xml = IF_EXAMPLE;
        break;
      case "substitutionExample":
        xml = SUBSTITUTION_EXAMPLE;
        break;
      case "userExample":
        xml = USER_EXAMPLE;
        break;
    }

    dom.executeVoid("editor.setValue(`" + xml.trim() + "`, -1);undefined;");
  }

  private void acRaw(DOM dom) throws IOException {
    dom.inner("output", escapeHtmlAndFormat(streamToString(stringToStream(dom.executeString("editor.getValue();")))));
    dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);");
  }

  private void acPreprocessed(DOM dom) throws IOException {
    dom.inner("output", escapeHtmlAndFormat(streamToString(new XPPqPreprocessor(stringToStream(dom.executeString("editor.getValue();"))))));
    dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);");
  }

  private void acWithout(DOM dom) {
    dom.inner("output", parse(stringToStream(dom.executeString("editor.getValue();"))));
  }

  private void acWith(DOM dom) {
    dom.inner("output", parse(new XPPqPreprocessor(stringToStream(dom.executeString("editor.getValue();")))));
  }

  @Override
  public void handle(String action, String id) {
    try {
      switch (action) {
        case "":
          acConnect(dom);
          break;
        case "Select":
          acSelect(dom, id);
          break;
        case "Raw":
          acRaw(dom);
          break;
        case "Preprocessed":
          acPreprocessed(dom);
          break;
        case "Without":
          acWithout(dom);
          break;
        case "With":
          acWith(dom);
          break;
        default:
          System.err.println("No or unknown action !");
          System.exit(1);
      }
    } catch (Exception e) {
      dom.inner("output", "<pre style=\"color: darkred;\">" + e.getMessage() + "</pre>");
    }
  }

  public static void main(String[] args) {
    launch(WEB::new, HEAD);
  }

  /* XML examples */

  private static final String BASIC_EXAMPLE =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
"<SomeTag\n" +
"  AnAttribute=\"SomeAttributeValue\">\n" +
"  <SomeOtherTag\n" +
"   AnotherAttribute=\"AnotherAttributeValue\">\n" +
"    Tag value\n" +
"  </SomeOtherTag>\n" +
"  <xpp:define\n" +
"   name=\"SomeMacro\">\n" +
"    <xpp:bloc>Some macro content !</xpp:bloc>\n" +
"  </xpp:define>\n" +
"  <YetAnotherTag\n" +
"   YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" +
"    <xpp:expand select=\"SomeMacro\"/>\n" +
"  </YetAnotherTag>\n" +
"</SomeTag>";

  private static final String IF_EXAMPLE =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
"<RootTag xmlns:xpp=\"http://q37.info/ns/xpp/\">\n" +
"  <xpp:define\n" +
"   name=\"MacroWithVar\">\n" +
"    <VarValueTest>\n" +
"      <xpp:ifeq\n" +
"       select=\"MyVar\"\n" +
"       value=\"VarFirstValue\">\n" +
"        <xpp:bloc>At first, 'MyVar' value is 'VarFirstValue'.</xpp:bloc>\n" +
"      </xpp:ifeq>\n" +
"      <xpp:ifeq\n" +
"       select=\"MyVar\"\n" +
"       value=\"VarSecondValue\">\n" +
"        <xpp:bloc>Then 'MyVar' value is 'VarSecondValue'.</xpp:bloc>\n" +
"      </xpp:ifeq>\n" +
"    </VarValueTest>\n" +
"  </xpp:define>\n" +
"  <xpp:set\n" +
"   name=\"MyVar\"\n" +
"   value=\"VarFirstValue\"/>\n" +
"  <xpp:expand\n" +
"   select=\"MacroWithVar\"/>\n" +
"  <xpp:set\n" +
"   name=\"MyVar\"\n" +
"   value=\"VarSecondValue\"/>\n" +
"  <xpp:expand\n" +
"   select=\"MacroWithVar\"/>\n" +
"</RootTag>";

  private static final String SUBSTITUTION_EXAMPLE =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
"<Root xmlns:xpp=\"http://q37.info/ns/xpp/\">\n" +
"  <xpp:set\n" +
"    name=\"FirstVar\"\n" +
"    value=\"FirstVarValue\"/>\n" +
"  <xpp:set\n" +
"    name=\"SecondVar\"\n" +
"    value=\"SecondVarValue\"/>\n" +
"  <xpp:bloc marker=\"$\">\n" +
"    <Tag\n" +
"      AttributeOne=\"Attribute for the $FirstVar$\"\n" +
"      AttributeTwo=\"Attribute for the $SecondVar$\"/>\n" +
"  </xpp:bloc>\n" +
"</Root>";

  private static final String USER_EXAMPLE =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
"<Root xmlns:xpp=\"http://q37.info/ns/xpp/\">\n" +
"  Type your own XML code here.\n" +
"</Root>";

  /* GUI related */

  private static final String HEAD =
"<title>Native XML preprocessor and parser with Java</title>" +
"<link rel=\"icon\" type=\"image/png\" href=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==\" />" +
"<!-- HTML Syntax highlighting -->" +
"<link rel=\"stylesheet\" href=\"//cdnjs.cloudflare.com/ajax/libs/highlight.js/10.1.2/styles/default.min.css\">" +
"<script src=\"//cdnjs.cloudflare.com/ajax/libs/highlight.js/10.1.2/highlight.min.js\"></script>" +
"<script>\"document.querySelectorAll('pre').forEach((block) => {hljs.highlightBlock(block);});\"</script>" +
"<style>" +
"  .Tag {" +
"    font-weight: bold;" +
"  }" +
"  .AttributeName {" +
"    font-weight: bold;" +
"    color:darksalmon;" +
"  }" +
"  .AttributeValue {" +
"    background-color:lightsalmon;" +
"  }" +
"  .Value {" +
"    background-color:lightgrey;" +
"  }" +
"  ul {" +
"    padding-left: 10px;" +
"    list-style-type: none;" +
"  }" +
"  button {" +
"    margin: 2px;" +
"  }" +
"  details summary { " +
"    cursor: pointer;" +
"  }" +
"  details summary > * {" +
"    display: inline;"+
"  }" +
"</style>" +
"<script src=\"https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.14/ace.js\" integrity=\"sha512-6ts6Fu561/yzWvD6uwQp3XVYwiWNpWnZ0hdeQrETqtnQiGjTfOS06W76aUDnq51hl1SxXtJaqy7IsZ3oP/uZEg==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>";

  private static final String BODY =
"<iframe style=\"width: 100%; border: none;\" src=\"FaaSDesc.php?text=VGhpcyAqSmF2YSogYXBwbGljYXRpb24gdXNlcyBhIFsqSk5JKl0oaHR0cHM6Ly9lbi53aWtpcGVkaWEub3JnL3dpa2kvSmF2YV9OYXRpdmVfSW50ZXJmYWNlKSBiYXNlZCBwYWNrYWdlIGltcGxlbWVudGluZyBhbiAqWE1MKiBwYXJzZXIgYW5kIHByZXByb2Nlc3Nvci4gRGV2ZWxvcGVkIGluICpDKi8qQysrKiwgdGhpcyBwYWNrYWdlIGlzIG11Y2ggbW9yZSBmYXN0ZXIsIHJlbGlhYmxlIGFuZCByZXNvdXJjZS1lZmZpY2llbnQsIGFuZCB0aGVyZWZvcmUgbW9yZSBlbnZpcm9ubWVudC1mcmllbmRseSwgdGhhbiBpZiBpdCBoYWQgYmVlbiBkZXZlbG9wZWQgaW4gKkphdmEqLiAgClRoZSAqUmF3KiBidXR0b24gZGlzcGxheXMgdGhlIGNvbnRlbnQgb2YgdGhlICpJbnB1dCogZmllbGQgaW4gdGhlICpPdXRwdXQqIGZpZWxkLiBUaGUgKlByZXByb2Nlc3NlZCogYnV0dG9uIGRpc3BsYXlzIHRoZSByZXN1bHQgb2YgdGhlIHByZXByb2Nlc3NpbmcuIFRoZSAqdy9vIHByZXByb2Nlc3NpbmcqIGFuZCAqd2l0aCBwcmVwcm9jZXNzaW5nKiBidXR0b25zIGZvcm1hdCB0aGUgKlhNTCogZGF0YSB1c2luZyB0aGUgcGFyc2VyLCByZXNwZWN0aXZlbHkgd2l0aG91dCBhbmQgd2l0aCBwcmVwcm9jZXNzaW5nLiBZb3UgY2FuIHBsYWNlIHlvdXIgb3duIGNvbnRlbnQgaW4gdGhlICpJbnB1dCogZmllbGQuICAKQXMgeW91IGNhbiBzZWUgaW4gdGhlIFsqV0VCLmphdmEqXShodHRwczovL3EzNy5pbmZvL3MvcnBweGdkZjQpIGZpbGUsIHRoaXMgcGFja2FnZSBpcyB1c2VkIGxpa2UgYW55IG90aGVyICpKYXZhKiBwYWNrYWdlLiBZb3Ugd2lsbCBhbHNvIGZpbmQgaW4gdGhlIHNhbWUgcmVwb3NpdG9yeSB0aGUgY29tcGxldGUgKkMqLypDKysqIGNvZGUgY29ycmVzcG9uZGluZyB0byB0aGlzIHBhY2thZ2Uu\"></iframe>" +
"<fieldset>" +
"  <legend>Input</legend>" +
"  <fieldset>" +
"    <div id=\"input\" style=\"font-size: initial; resize: horizontal;\"/>" +
"  </fieldset>" +
"  <fieldset style=\"display: flex;flex-direction: row;justify-content: space-evenly;\">" +
"  <legend>Actions</legend>" +
"    <fieldset style=\"display: flex; align-items: center;\">" +
"      <legend>Examples</legend>" +
"      <select xdh:onevent=\"Select\">" +
"        <option value=\"basicExample\">Basic</option>" +
"        <option value=\"ifExample\">Using 'ifeq'</option>" +
"        <option value=\"substitutionExample\">Substitution</option>" +
"        <option value=\"userExample\">Yours</option>" +
"      </select>" +
"    </fieldset>" +
"    <fieldset style=\"display: flex; flex-direction: column;\">" +
"      <legend>XML</legend>" +
"      <button xdh:onevent=\"Raw\">Raw</button>" +
"      <button xdh:onevent=\"Preprocessed\">Preprocessed</button>" +
"    </fieldset>" +
"    <fieldset style=\"display: flex;flex-direction: column;\">" +
"      <legend>Parsing</legend>" +
"      <button xdh:onevent=\"Without\">w/o preprocessing</button>" +
"      <button xdh:onevent=\"With\">with preprocessing</button>" +
"    </fieldset>" +
"  </fieldset>" +
"  <fieldset>" +
"  <legend>Output</legend>" +
"      <output id=\"output\">" +
"      <span style=\"font-style: oblique;\">Enter XML code or select an example,<br/>and click one of above buttons.</span>" +
"      </output>" +
"  </fieldset>" +
"</fieldset>";

  private static final String INIT_SCRIPT =
"var editor = ace.edit(\"input\"," +
"{" +
"  theme: \"ace/theme/monokai\"," +
"  mode: \"ace/mode/xml\"," +
"  maxLines: 50," +
"  printMargin: false" +
"});";
}
