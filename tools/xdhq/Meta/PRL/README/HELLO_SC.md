```perl
use Atlas;

my $body = '
<fieldset>
 <input id="input" maxlength="20" placeholder="Enter a name here" type="text"
         data-xdh-onevent="Submit" value="World"/>
 <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
  <button data-xdh-onevent="Submit">Submit</button>
  <button data-xdh-onevent="Clear">Clear</button>
 </div>
</fieldset>
';

sub acConnect {
 my ($hello, $dom) = @_;

 $dom->inner("",$body);
 $dom->focus("input");
}

sub acSubmit {
 my ($hello, $dom) = @_;

 $dom->alert("Hello, " . $dom->getValue("input") . "!");
 $dom->focus("input");
}

sub acClear {
 my ($hello, $dom) = @_;

 if ( $dom->confirm("Are you sure?") ) {
  $dom->setValue("input", "");
 }

 $dom->focus("input");
}

my %callbacks = (
 "" => \&acConnect,
 "Submit" => \&acSubmit,
 "Clear" => \&acClear,
);

Atlas::launch(\%callbacks);
```