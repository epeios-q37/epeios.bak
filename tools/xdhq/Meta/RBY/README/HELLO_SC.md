```ruby
require 'Atlas'

$BODY =
<<~HEREDOC
<fieldset>
 <input id="Input" data-xdh-onevent="Submit" value="World"/>
 <button data-xdh-onevent="Submit">Hello</button>
 <hr/>
 <fieldset>
  <output id="Output">Greetings displayed here!</output>
 </fieldset>
</fieldset>
HEREDOC

def acConnect(userObject, dom, id)
 dom.inner("", $BODY)
 dom.focus("Input")
end

def acSubmit(userObject, dom, id)
 name = dom.getValue("Input")
 dom.setValue("Output", "Hello, " + name + "!")
 dom.setValue("Input", "")
 dom.focus("Input")
end

CALLBACKS = {
 "" => method(:acConnect),
 "Submit" => method(:acSubmit)
}

Atlas.launch(CALLBACKS)
```