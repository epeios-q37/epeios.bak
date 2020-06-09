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

my $FaaSProtocolLabel = "7b4b6bea-2432-4584-950b-e595c9e391e1";
my $FaaSProtocolVersion = "0";
my $mainProtocolLabel = "8d2b7b52-6681-48d6-8974-6e0127a4ca7e";
my $mainProtocolVersion = "0";

my $pAddr = "faas1.q37.info";
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
    my $atk = getEnv("ATK");

    if ($atk eq "" ) {}
    elsif ($atk eq "DEV") {
        $pAddr = "localhost";
        $wPort = "8080";
        CORE::say("\tDEV mode!");
    } elsif ($atk eq "TEST") {
        $cgi = "xdh_";
    } else  {
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

sub _handshake {

    XDHq::FAAS::SHRD::writeString($FaaSProtocolLabel);
    XDHq::FAAS::SHRD::writeString($FaaSProtocolVersion);

    my $error = XDHq::FAAS::SHRD::getString();

    if ( $error ne "") {
        die($error);
    }

    my $notification = XDHq::FAAS::SHRD::getString();

    if ( $notification ne "") {
        CORE::say($notification);
    }
}

sub _ignition {
    XDHq::FAAS::SHRD::writeString($token);
    XDHq::FAAS::SHRD::writeString($main::headContent);
    XDHq::FAAS::SHRD::writeString($wAddr);

    $token = XDHq::FAAS::SHRD::getString();

    if ( $token eq "") {
        die(XDHq::FAAS::SHRD::getString());
    }

    if (not($wPort eq ":0")) {
#        my $url = "http://${wAddr}${wPort}/${cgi}.php?_token=${token}";
        my $url = XDHq::FAAS::SHRD::getString();

        CORE::say($url);
        CORE::say("^" x length($url));
        CORE::say("Open above URL in a web browser. Enjoy!\n");
        XDHq::SHRD::open($url);
    }
}

sub _serve {
    my ($callback, $userCallback, $callbacks) = @_;

    while(XDHq::SHRD::TRUE) {
        my $id = XDHq::FAAS::SHRD::getSInt();

        if ( $id eq -1 ) {   # Should never happen.
            die("Received unexpected undefined command id!");
        } elsif ( $id eq -2) {   # Value reporting a new front-end.
            $id = XDHq::FAAS::SHRD::getSInt();    # The id of the new front-end.

            if ( $instances{$id} ) {
                die("Instance of id '${id}' exists but should not !")
            }

            my $instance : shared = XDHq::FAAS::Instance::new();

            XDHq::FAAS::Instance::set($instance, $callback->($userCallback, $callbacks, $instance),$id);

            $instances{$id}=$instance;

            {   # Locking scope.
                lock($XDHq::FAAS::SHRD::writeLock);
                XDHq::FAAS::SHRD::writeSInt($id);
                XDHq::FAAS::SHRD::writeString($mainProtocolLabel);
                XDHq::FAAS::SHRD::writeString($mainProtocolVersion);
            }
        } elsif ( $id eq -3 ) {
            $id = XDHq::FAAS::SHRD::getSInt();

            if ( not($instances{$id})) {
                die("Instance of id id '${id}' not available for destruction!")  ;
            }

           $instances{$id}->{quit} = XDHq::SHRD::TRUE;

            XDHq::FAAS::Instance::signal($instances{$id});

            lock($XDHq::FAAS::SHRD::globalCondition);
            cond_wait($XDHq::FAAS::SHRD::globalCondition);

            delete $instances{$id};
        } elsif ( not($instances{$id})) {
            die("Unknown instance of id '${id}'!")
        } elsif (not(XDHq::FAAS::Instance::testAndSetHandshake($instances{$id}))) {
            my $error = XDHq::FAAS::SHRD::getString();

            if ($error) {
                die($error);
            }

            XDHq::FAAS::SHRD::getString();    # Language. Not handled yet.

            {   # Lock scope;
                lock($XDHq::FAAS::SHRD::writeLock);
                XDHq::FAAS::SHRD::writeSInt($id);
                XDHq::FAAS::SHRD::writeString("PRL");
            }
        } else {
            XDHq::FAAS::Instance::signal($instances{$id});

            lock($XDHq::FAAS::SHRD::globalCondition);
            cond_wait($XDHq::FAAS::SHRD::globalCondition);
        }
    }
}
        
sub launch {
    my ($callback, $userCallback, $callbacks, $headContent) = @_;

    $main::headContent = $headContent;

    _init();
    _handshake();
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
