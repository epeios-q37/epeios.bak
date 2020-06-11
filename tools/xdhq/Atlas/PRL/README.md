# *Perl* version of the *Atlas* toolkit

![For Perl](https://q37.info/download/assets/Perl.png "Perl logo")

[![Run on Repl.it](https://repl.it/badge/github/epeios-q37/atlas-java)](https://q37.info/s/h3h34zgq) [![Version 0.11](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.11)](https://q37.info/s/gei0veus)

*NOTA*: this toolkit is also available for:
- *Java*: <http://github.com/epeios-q37/atlas-java>,
- *Node.js*: <http://github.com/epeios-q37/atlas-node>,
- *Python*: <http://github.com/epeios-q37/atlas-python>,
- *Ruby*: <http://github.com/epeios-q37/atlas-ruby>.

---

With the [*Atlas* toolkit](http://atlastk.org/), it has never been easier to create your own modern web application ([*SPA*](https://q37.info/s/7sbmxd3j)):
- no *Javascript* to write; only *HTML* and *Perl*,
- no [front and back end architecture](https://q37.info/s/px7hhztd) to bother with,
- no [web server](https://q37.info/s/n3hpwsht) (*Apache*, *Nginx*…) to install,

and all this only with the help of a library of about 35 KB.

With the *Atlas* toolkit, your applications will be accessible from the entire internet on laptops, smartphones, tablets…, and this without having to deploy them on a remote server or to open an incoming port on your internet box. All you need is a local computer with a simple internet connection. 

The *Atlas* toolkit is also the fastest and easiest way to add a [graphical user interface](https://q37.info/s/hw9n3pjs) to all your programs.

## *Hello, World!*

Here's how a [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) type program made with the *Atlas* toolkit looks like:

![Little demonstration](https://q37.info/download/assets/Hello.gif "A basic example")

- `git clone https://github.com/epeios-q37/atlas-perl`
- `cd atlas-perl`
- `perl Hello/Hello.py`

<!-- You can also put below source code in a file and launch it after having installed the [*atlastk* package](http://q37.info/s/9srmskcm) (`pip install atlastk`), or, with absolutely no installation, paste the below code [here](http://q37.info/s/srnnb7hj), and open the displayed *URL* in a web browser.
-->

For a live demonstration: <http://q37.info/s/zmzcxvm9> (other live demonstrations: <https://q37.info/s/sssznrb4>).

Source code:

```perl
use Atlas;
# For the sake of simplification, the following HTML content has been
# embedded in the source code, but it can be easily stored in a file. 
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


