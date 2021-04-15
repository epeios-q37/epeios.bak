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
 dom.inner("", $body)
 dom.focus("Input")
end

def acSubmit(userObject, dom, id)
 name = dom.getValue("Input")
 dom.set_value("Hello, " + name + "!")
 dom.set_value("Input", "")
 dom.focus("Input")
end

callbacks = {
 "" => method(:acConnect),  # This key is the action label for a new connection.
 "Submit" => method(:acSubmit)
}

Atlas.launch(callbacks)
```