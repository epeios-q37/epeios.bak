# *Perl* version of the *Atlas* toolkit

![For Perl](http://q37.info/download/assets/Perl.png "Perl logo")

[![Version 0.9.0](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.9.0)](http://q37.info/s/gei0veus)[![License](https://img.shields.io/pypi/l/atlastk.svg?style=plastic)](https://github.com/epeios-q37/atlas-perl/blob/master/LICENSE)

A fast and easy way to add a graphical user interface ([GUI](http://q37.info/s/hw9n3pjs)) to your *Perl* programs.

With the *Atlas* toolkit, both [front and back ends](http://q37.info/s/px7hhztd) are handled by the same code, and the programs will also be reachable from all over the internet.

Only basic knowledge of web technologies are required (no *JavaScript* knowledge required). And, with the *Atlas* toolkit, you can easily add a GUI to a programs originally designed to have a [CLI](https://q37.info/s/cnh9nrw9), so the users of this programs can choose which interface they want to use.

<!--
If you want to use the *Atlas* toolkit without installing the examples, simply install the [*atlastk* package from *PyPI*](http://q37.info/s/9srmskcm) (`pip install atlastk`). This package has no dependencies.

You can also use the *Atlas* toolkit on [*Repl.it*](http://q37.info/s/mxmgq3qm), an [online IDE](https://q37.info/s/zzkzbdw7), so you have nothing to install. You will find some examples in the next sections. **Important**: you have to ***fork*** this examples before launching them (by clicking the dedicated button or by modifying the source code), or they won't work properly! See <http://q37.info/s/zbgfjtp9> for more details.
-->

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type program made with the *Atlas* toolkit looks like:

![Little demonstration](http://q37.info/download/assets/Hello.gif "A basic example")

- `git clone http://github.com/epeios-q37/atlas-perl`
- `cd atlas-perl`
- `perl Hello/Hello.py`

<!-- You can also put below source code in a file and launch it after having installed the [*atlastk* package](http://q37.info/s/9srmskcm) (`pip install atlastk`), or, with absolutely no installation, paste the below code [here](http://q37.info/s/srnnb7hj), and open the displayed *URL* in a web browser.
-->

For a live demonstration: <http://q37.info/s/zmzcxvm9> (other live demonstrations: <https://q37.info/s/sssznrb4>).

Source code:

```perl
use Atlas;

my $body = '
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text"
         data-xdh-onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button data-xdh-onevent="Submit">Submit</button>
   <button data-xdh-onevent="Clear">Clear</button>
  </div>
 </fieldset>
</div>';

sub acConnect {
    my ($hello, $dom) = @_;

    $dom->setLayout("",$body);
    $dom->focus("input");
}

sub acSubmit {
    my ($hello, $dom) = @_;

    $dom->alert("Hello, " . $dom->getContent("input") . "!");
    $dom->focus("input");
}

sub acClear {
    my ($hello, $dom) = @_;

    if ( $dom->confirm("Are you sure?") ) {
        $dom->setContent("input", "");
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

<!--
## *TodoMVC*

And here's how the *Atlas* toolkit version of the [*TodoMVC*](http://todomvc.com/) application looks like:

![TodoMVC](http://q37.info/download/TodoMVC.gif "The TodoMVC application made with the Atlas toolkit")

For a live demonstration: <http://q37.info/s/n9nnwzcg>.
-->

## Content of the repository

The `atlastk` directory contains the *Perl* source code of the *Atlas* toolkit.

All other directories are examples.

To run an example, launch `perl <Name>/main.pl`, where `<Name>` is the name of the example (`Blank`, `Chatroom`…)

## Miscellaneous

The *Atlas* toolkit is also available for:

- *Java*: <http://github.com/epeios-q37/atlas-java>
- *Node.js*: <http://github.com/epeios-q37/atlas-node>
- *PHP*: <http://github.com/epeios-q37/atlas-php>
- *Python*: <http://github.com/epeios-q37/atlas-python>
- *Ruby*: <http://github.com/epeios-q37/atlas-ruby>

For more information about the *Atlas* toolkit, go to <http://atlastk.org/>.
