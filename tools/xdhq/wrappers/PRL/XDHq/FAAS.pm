=pod
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
=cut

use strict; use warnings;

package XDHq::FAAS;

use XDHq::SHRD;
use XDHq::FAAS::SHRD;
use XDHq::FAAS::Instance;
use IO::Socket::INET;
use threads;
use threads::shared;
use strict;

use constant {
    VERSION_ => "0.13",
    FAAS_PROTOCOL_LABEL_ => "4c837d30-2eb5-41af-9b3d-6c8bf01d8dbf",
    FAAS_PROTOCOL_VERSION_ => "0",
    MAIN_PROTOCOL_LABEL_ => "22bb5d73-924f-473f-a68a-14f41d8bfa83",
    MAIN_PROTOCOL_VERSION_ => "0",
    SCRIPTS_VERSION_ => "0",
};

my $pAddr = "faas.q37.info";
my $pPort = 53700;
my $wAddr = "";
my $wPort = "";
my $cgi = "xdh";

my $headContent: shared;
my $token = "";
my %instances;

sub trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };

sub getEnv {
    my ($name,$value) = @_;
    $value //= "";

    my $env= $ENV{$name};

    if (defined($env)) {
        return trim($env);
    } else {
        return return($value);
    }
}

sub _tokenIsEmpty {
    return ($token eq "") ||(substr($token,0,1) == "&");
}

sub _init {
    $token = "";
    my $atk = uc(getEnv("ATK"));

    if (($atk eq "") or ($atk eq "NONE" )) {}
    elsif ($atk eq "DEV") {
        $pAddr = "localhost";
        $wPort = "8080";
        CORE::say("\tDEV mode!");
    } elsif ($atk eq "TEST") {
        $cgi = "xdh_";
    } elsif ($atk ne "REPLIT") {
        die("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
    }

    $pAddr = getEnv("ATK_PADDR", $pAddr);
    $pPort = getEnv("ATK_PPORT", "$pPort");
    $wAddr = getEnv("ATK_WADDR", $wAddr);
    $wPort = getEnv("ATK_WPORT", $wPort);

    if ($wAddr eq "") {
        $wAddr = $pAddr;
    }

    if ($wPort ne "") {
        $wPort = ":" . $wPort;
    }

    if (_tokenIsEmpty()) {
        $token = getEnv("ATK_TOKEN");
    }

    if ($token ne "") {
        $token = "&" . $token;
    }

    # auto-flush on socket
    $| = 1;

    CORE::say("Connection to '${pAddr}:${pPort}'…");


    # create a connecting socket
    $XDHq::FAAS::SHRD::socket = new IO::Socket::INET (
        PeerHost => $pAddr,
        PeerPort => $pPort,
        Proto => 'tcp',
    );

    die("Error on connection to '${pAddr}:${pPort}': $! !!!\n") unless $XDHq::FAAS::SHRD::socket;

    CORE::say("Connected to '${pAddr}:${pPort}'.\n");
}

sub _handshakeFaaS {
    XDHq::FAAS::SHRD::writeString(FAAS_PROTOCOL_LABEL_);
    XDHq::FAAS::SHRD::writeString(FAAS_PROTOCOL_VERSION_);
    XDHq::FAAS::SHRD::writeString("PRL " . VERSION_);

    my $error = XDHq::FAAS::SHRD::getString();

    if ( $error ne "") {
        die($error);
    }

    my $notification = XDHq::FAAS::SHRD::getString();

    if ( $notification ne "") {
        CORE::say($notification);
    }
}


sub _handshakeMain {
    XDHq::FAAS::SHRD::writeString(MAIN_PROTOCOL_LABEL_);
    XDHq::FAAS::SHRD::writeString(MAIN_PROTOCOL_VERSION_);
    XDHq::FAAS::SHRD::writeString(SCRIPTS_VERSION_);

    my $error = XDHq::FAAS::SHRD::getString();

    if ( $error ne "") {
        die($error);
    }

    my $notification = XDHq::FAAS::SHRD::getString();

    if ( $notification ne "") {
        CORE::say($notification);
    }
}

sub _handshakes {
    _handshakeFaaS();
    _handshakeMain();
}

sub _ignition {
    XDHq::FAAS::SHRD::writeString($token);
    XDHq::FAAS::SHRD::writeString($main::headContent);
    XDHq::FAAS::SHRD::writeString($wAddr);
    XDHq::FAAS::SHRD::writeString("PRL");    

    $token = XDHq::FAAS::SHRD::getString();

    if ( $token eq "") {
        die(XDHq::FAAS::SHRD::getString());
    }

    if (not($wPort eq ":0")) {
        my $url = XDHq::FAAS::SHRD::getString();

        CORE::say($url);
        CORE::say("^" x length($url));
        CORE::say("Open above URL in a web browser (click, right click or copy/paste). Enjoy!\n");

        if ( XDHq::SHRD::isREPLit() ) {
            system("node", "-e",
            'require("http").createServer(function (req, res){res.end("<html><body><iframe style=\"border-style: none; width: 100%;height: 100%\" src=\"https://atlastk.org/repl_it.php?url=' . $url . '\"></iframe></body></html>");process.exit();}).listen(8080)');
        } elsif (not uc(getEnv("ATK")) eq "NONE") {
            XDHq::SHRD::open($url);
        }
    }
}

use constant {
    FORBIDDEN_ID_ => -1,
    CREATION_ID_ => -2,
    CLOSING_ID_ => -3,
};

sub _dismiss {
    # Lock scope.
    lock($XDHq::FAAS::SHRD::writeLock);
    XDHq::FAAS::SHRD::writeSInt(shift);
    XDHq::FAAS::SHRD::writeString("#Dismiss_1");
}  

sub _report {
    # Lock scope.
    lock($XDHq::FAAS::SHRD::writeLock);
    XDHq::FAAS::SHRD::writeSInt(-1);
    XDHq::FAAS::SHRD::writeString("#Inform_1_");
    XDHq::FAAS::SHRD::writeString(shift);
}

sub _serve {
    my ($callback, $userCallback, $callbacks) = @_;

    while(XDHq::SHRD::TRUE) {
        my $id = XDHq::FAAS::SHRD::getSInt();

        if ( $id eq FORBIDDEN_ID_ ) {   # Should never happen.
            die("Received unexpected undefined command id!");
        } elsif ( $id eq CREATION_ID_ ) {   # Value reporting a new front-end.
            $id = XDHq::FAAS::SHRD::getSInt();    # The id of the new front-end.

            if ( $instances{$id} ) {
                _report("Instance of id '${id}' exists but should not !")
            }

            my $instance : shared = XDHq::FAAS::Instance->new($id);

            $callback->($userCallback, $callbacks, $instance);

            $instances{$id}=$instance;
        } elsif ( $id eq CLOSING_ID_ ) {
            $id = XDHq::FAAS::SHRD::getSInt();

            if ( not($instances{$id})) {
                _report("Instance of id id '${id}' not available for destruction!")  ;
            } else {
                my $instance = $instances{$id};

                $instance->{quit} = XDHq::SHRD::TRUE;
                $instance->dataAvailable();
                XDHq::FAAS::SHRD::waitForInstance();
            }

            delete $instances{$id};
        } elsif ( not($instances{$id})) {
            _report("Unknown instance of id '${id}'!");
            _dismiss($id);
        } else {
            my $instance = $instances{$id};

            if ( !defined($instance->{language}) ) {
                $instance->{language} = XDHq::FAAS::SHRD::getString();
            } else {
                $instance->dataAvailable();
                XDHq::FAAS::SHRD::waitForInstance();
            } 
        }
    }
}
        
sub launch {
    my ($callback, $userCallback, $callbacks, $headContent) = @_;

    $main::headContent = $headContent;

    _init();
    _handshakes();
    _ignition();
    _serve($callback, $userCallback, $callbacks);
}

sub broadcastAction {
    {   # Lock scope;
        lock($XDHq::FAAS::SHRD::writeLock);
        XDHq::FAAS::SHRD::writeSInt(-3);
        XDHq::FAAS::SHRD::writeString(shift);
        XDHq::FAAS::SHRD::writeString(shift);
    }    
}

return XDHq::SHRD::TRUE;
