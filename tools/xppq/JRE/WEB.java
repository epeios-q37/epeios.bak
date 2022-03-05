import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.stream.Stream;

import info.q37.atlas.*;

public class WEB  extends Atlas {
  private static final String head =
"<title>Native XML preprocessor and parser with Node.js</title>" +
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
"</style>" +
"<script src=\"https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.14/ace.js\" integrity=\"sha512-6ts6Fu561/yzWvD6uwQp3XVYwiWNpWnZ0hdeQrETqtnQiGjTfOS06W76aUDnq51hl1SxXtJaqy7IsZ3oP/uZEg==\" crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>";

  private static final String initScript = 
"var editor = ace.edit(\"input\"," + 
"{" + 
"  theme: \"ace/theme/monokai\"," + 
"  mode: \"ace/mode/xml\"," + 
"  maxLines: 50," + 
"  printMargin: false" + 
"});";

  private static final String body =
"<fieldset>" +
"  <fieldset>" +
"    <div id=\"input\" style=\"font-size: initial; resize: horizontal;\"/>" +
"  </fieldset>" +
"  <fieldset style=\"display: flex;flex-direction: row;justify-content: space-evenly;\">" +
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
"      <output id=\"output\">" +
"      <span style=\"font-style: oblique;\">Enter XML code or select an exemple,<br/>and/or click one of above button.</span>" +
"      </output>" +
"  </fieldset>" +
"</fieldset>";

  private static final String basicExample = 
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

  private static InputStream stringToStream (String string) {
    return new ByteArrayInputStream(string.getBytes());
  }

  private static String streamToString(InputStream stream) throws IOException {
    int c = 0;
    String result = "";

    while ((c = stream.read()) != -1) {
      result = result + ((char) c);
    }

    return result;
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
    dom.inner("", body);
    dom.executeVoid(initScript + "editor.setValue(`" + basicExample + "`, -1);undefined;");
    dom.end("output", "<hr/><pre>" + XPPq.componentInfo() + "</pre><pre>" + XPPq.wrapperInfo() + "</pre>");
  }

  private void acRaw(DOM dom) throws IOException {
    dom.inner("output", escapeHtmlAndFormat(streamToString(stringToStream(dom.executeString("editor.getValue();")))));
    dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);");
  }

  private void acPreprocessed(DOM dom) throws IOException {
    dom.inner("output", escapeHtmlAndFormat(streamToString(new XPPqPreprocessor(stringToStream(dom.executeString("editor.getValue();"))))));
    dom.executeVoid("hljs.highlightBlock(document.getElementById('output').firstChild);");
  }

	@Override
	public void handle(String action, String id ) {
    try {
      if ( action.equals( "" ) ) {
        acConnect(dom);
      } else if ( action.equals( "Raw" ) ) {
        acRaw(dom);
      } else if ( action.equals( "Preprocessed" ) ) {
        acPreprocessed(dom);
      } else {
        System.err.println("No or unknown action !");
        System.exit(1);
      }
	  } catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
  }

	public static void main(String[] args) {
		launch(WEB::new, head);
	}
}
