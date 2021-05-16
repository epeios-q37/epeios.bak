```perl
use Atlas;
 
my $BODY = '
<fieldset>
 <input id="Input" data-xdh-onevent="Submit" value="World"/>
 <button data-xdh-onevent="Submit">Hello</button>
 <hr/>
 <fieldset>
  <output id="Output">Greetings displayed here!</output>
 </fieldset>
</fieldset>
';
 
sub acConnect {
 my ($hello, $dom) = @_;
 
 $dom->inner("", $BODY);
 $dom->focus("Input");
}
 
sub acSubmit {
 my ($hello, $dom) = @_;
 my $name = $dom->getValue("Input");
 
 $dom->begin("Output", "<div>Hello, $name!</div>");
 $dom->setValue("Input", "");
 $dom->focus("Input");
}
 
my %CALLBACKS = (
 "" => \&acConnect,
 "Submit" => \&acSubmit
);
 
Atlas::launch(\%CALLBACKS);
```