# *Perl* version of the *Atlas* toolkit

![For Perl](https://q37.info/download/assets/Perl.png "Perl logo")

[![Run on Repl.it](https://repl.it/badge/github/epeios-q37/atlas-perl)](https://q37.info/s/h3h34zgq)
[![Version 0.11](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.11)](http://github.com/epeios-q37/atlas-perl/)
[![Stars](https://img.shields.io/github/stars/epeios-q37/atlas-perl.svg?style=social)](https://github.com/epeios-q37/atlas-perl/stargazers)
[![license: MIT](https://img.shields.io/github/license/epeios-q37/atlas-perl?color=yellow)](https://github.com/epeios-q37/atlas-perl/blob/master/LICENSE)
[![Homepage](https://img.shields.io/static/v1?label=homepage&message=atlastk.org&color=ff69b4)](https://atlastk.org)


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

and all this only with the help of a library of about 45 KB.

With the *Atlas* toolkit, your applications will be accessible from the entire internet on laptops, smartphones, tablets…, and this without having to deploy them on a remote server or to open an incoming port on your internet box. All you need is a local computer with a simple internet connection. 

The *Atlas* toolkit is also the fastest and easiest way to add a [graphical user interface](https://q37.info/s/hw9n3pjs) to all your programs.

## Live demonstrations

Before diving into source code, you can have a look on some live demonstrations. Simply go [here](https://q37.info/s/h3h34zgq), click on the green `run` button, select the demonstration you want to see, and then click (or scan with your smartphone) the then displayed [QR code](https://q37.info/s/3pktvrj7).

## *Hello, World!*

Here's how the [*Hello, World!*](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program) program made with the *Atlas* toolkit looks like:

![Little demonstration](https://q37.info/download/assets/Hello.gif "A basic example")

This example is part of the live demonstrations above, but you can launch it on your computer:

- `git clone https://github.com/epeios-q37/atlas-perl`
- `cd atlas-perl`
- `perl Hello/Hello.py`

For a live demonstration: <http://q37.info/s/zmzcxvm9> (other live demonstrations: <https://q37.info/s/sssznrb4>).

Source code:

```perl
use Atlas;

my $body = '
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text"
         xdh:onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button xdh:onevent="Submit">Submit</button>
   <button xdh:onevent="Clear">Clear</button>
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

## Content of the repository

The `atlastk` directory contains the *Perl* source code of the *Atlas* toolkit.

All other directories are examples.

To run an example, launch `perl <Name>/main.pl`, where `<Name>` is the name of the example (`Blank`, `Chatroom`…)


