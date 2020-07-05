```java
import info.q37.atlas.*;

class Hello extends Atlas {

 private static String body =
  "<div style=\"display: table; margin: 50px auto auto auto;\">" +
  " <fieldset>" +
  "   <input id=\"input\" maxlength=\"20\" placeholder=\"Enter a name here\"'" +
  "          type=\"text\" data-xdh-onevent=\"Submit\" value=\"World\"/>" +
  "   <div style=\"display: flex; justify-content: space-around; margin: 5px auto auto auto;\">" +
  "    <button data-xdh-onevent=\"Submit\">Submit</button>" +
  "    <button data-xdh-onevent=\"Clear\">Clear</button>" +
  "  </div>" +
  " </fieldset>" +
  "</div>";

 @Override
 public void handle(String action, String id)
 {
  switch(action) {
  case "": // Action label corresponding to a new session.
   dom.setLayout("", body);
   break;
  case "Submit":
   dom.alert("Hello, " + dom.getContent("input") + "!" );
   break;
  case "Clear":
   if ( dom.confirm("Are you sure ?") )
    dom.setContent("input", "");
   break;
  }

  dom.focus("input");
 }

 public static void main(String[] args)
 {
  launch(() -> new Hello());
 }
}
```