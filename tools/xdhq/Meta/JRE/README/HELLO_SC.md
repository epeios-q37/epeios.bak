```java
import info.q37.atlas.*;

class Hello extends Atlas {
 private static String BODY =
  "<fieldset>" +
  " <input id=\"Input\" data-xdh-onevent=\"Submit\" value=\"World\"/>" +
  " <button data-xdh-onevent=\"Submit\">Hello</button>" +
  " <hr/>" +
  " <fieldset>" +
  "  <output id=\"Output\">Greetings displayed here!</output>" +
  " </fieldset>" +
  "</fieldset>";

  @Override
  public void handle(String action, String id)
  {
    switch(action) {
    case "":
      dom.inner("", BODY);
      break;
    case "Submit":
      String name = dom.getValue("Input");
      dom.setValue("Output", "Hello, " + name + "!");
      dom.setValue("Input", "" );
      break;
    }
    dom.focus("Input");
  }

  public static void main(String[] args)
  {
    launch(() -> new Hello());
  }
}
```