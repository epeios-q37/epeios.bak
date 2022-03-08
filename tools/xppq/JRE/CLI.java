import java.util.logging.Level;
import java.util.logging.Logger;

class CLI {
  private static void displayBytecodeBuildTimestamp() throws Exception {
    System.out.println("Bytecode build : " + new java.util.Date(new java.io.File(XPPqTest.class.getClassLoader()
        .getResource(XPPqTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()));
  }

  private static void dump(java.io.InputStream stream) throws Exception {
    int c = 0;
    String result = "";

    while ((c = stream.read()) != -1) {
      result = result + ((char) c);
    }

    System.out.println(result);
  }

  private static void test0(java.io.InputStream stream) throws Exception {
    System.out.println("No treatment ; to see the original file :");
    System.out.println("-----------------------------------------");
    dump(stream);
  }

  private static void test1(java.io.InputStream stream) throws Exception {
    System.out.println("Preprocessing the file :");
    System.out.println("------------------------");
    dump(new XPPqPreprocessor(stream));
  }

  private static void parse(java.io.InputStream stream) throws Exception {
    XPPqData data = new XPPqData();
    XPPqParser parser = new XPPqParser(stream);

    int token = parser.parse(data);

    while (token != XPPqParser.PROCESSED) {
      switch (token) {
        case XPPqParser.START_TAG:
          System.out.print("Start tag: '" + data.tagName + "'\n");
          break;
        case XPPqParser.ATTRIBUTE:
          System.out.print("Attribute: '" + data.attributeName + "' = '" + data.value.trim() + "'\n");
          break;
        case XPPqParser.VALUE:
          System.out.print("Value:     '" + data.value.trim() + "'\n");
          break;
        case XPPqParser.END_TAG:
          System.out.print("End tag:   '" + data.tagName + "'\n");
          break;
        default:
          throw new Exception("Unknown token !!!");
      }

      token = parser.parse(data);
    }
  }

  private static void test2(java.io.InputStream stream) throws Exception {
    System.out.println("XML parsing WITHOUT preprocessing :");
    System.out.println("-----------------------------------");
    parse(stream);
  }

  private static void test3(java.io.InputStream stream) throws Exception {
    System.out.println("XML parsing WITH preprocessing :");
    System.out.println("--------------------------------");
    // parse( new XPPqPreprocessor( new java.io.FileInputStream( fileName ) ) );
    parse(new XPPqPreprocessor(stream));
  }

  private static String xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
      "<SomeTag xmlns:xpp=\"http://q37.info/ns/xpp/\" AnAttribute=\"SomeAttributeValue\">\n" +
      " <SomeOtherTag AnotherAttribute=\"AnotherAttributeValue\">TagValue</SomeOtherTag>\n" +
      " <xpp:define name=\"SomeMacro\">\n" +
      "  <xpp:bloc>Some macro content from string !</xpp:bloc>\n" +
      " </xpp:define>\n" +
      " <YetAnotherTag YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" +
      "  <xpp:expand select=\"SomeMacro\"/>\n" +
      " </YetAnotherTag>\n" +
      "</SomeTag>\n";

  private static java.io.InputStream getStream() throws Exception {
    if (true)
      return new java.io.FileInputStream("demo.xml");
    else
      return new java.io.ByteArrayInputStream(xml.getBytes());
  }

  public static void main(String[] args) throws Exception {
    XPPq.Init();
    System.out.println(XPPq.wrapperInfo());
    System.out.println(XPPq.componentInfo());
    displayBytecodeBuildTimestamp();
    System.out.println();

    int test = 3;

    if (args.length > 0) {
      try {
        test = Integer.parseInt(args[0]);
      } catch (NumberFormatException e) {
        System.err.println("'" + args[0] + "' is not a valid test id ; must be '0' to '3'.");
        System.exit(1);
      }
    }

    String fileName = new String("demo.xml");

    switch (test) {
      case 0:
        test0(getStream());
        break;
      case 1:
        test1(getStream());
        break;
      case 2:
        test2(getStream());
        break;
      case 3:
        test3(getStream());
        break;
      default:
        System.err.println("'" + test + "' is not a valid test id ; must be '0' to '3'.");
        System.exit(1);
        break;
    }
  }
}
