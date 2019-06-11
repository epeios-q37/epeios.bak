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

package XDHq::DEMO;

use XDHq::SHRD;
use XDHq::DEMO::Instance;
use IO::Socket::INET;
use threads;
use threads::shared;
use strict;

my $demoProtocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
my $demoProtocolVersion = "0";
my $mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
my $mainProtocolVersion = "0";

my $pAddr = "atlastk.org";
my $pPort = 53800;
my $wAddr = "";
my $wPort = "";
my $cgi = "xdh";

my $token = "";
my $socket;

my @instances;
my $writeLock: shared;
my $globalCondition: shared;

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

my sub tokenIsEmpty {
    return ($token eq "") ||(substr($token,0,1) == "&");
}

my sub init {
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

    if ($wPort eq "") {
        $wPort = ":" . $wPort;
    }

    if (not(tokenIsEmpty())) {
        $token = getAnv("ATK_TOKEN");
    }

    if ($token ne "") {
        $token = "&" . $token;
    }

    # auto-flush on socket
    $| = 1;

    # create a connecting socket
    $socket = new IO::Socket::INET (
        PeerHost => $pAddr,
        PeerPort => $pPort,
        Proto => 'tcp',
    );

    die("Error on connection to '${pAddr}:${pPort}': $! !!!\n") unless $socket;
}

my sub writeByte {
    $socket->send(shift);
}

my sub writeSize {
    my $size = shift;

    my $result = chr($size & 0x7f);
    $size >>= 7;

    while ( $size ne 0) {
        $result =chr(($size & 0x7f) | 0x80) . $result;
        $size >>=7;
    }

    $socket->send($result);
}

my sub writeString {
    my $string = shift;

    writeSize(length($string));
    $socket->send($string);
}

my sub getByte {
    my $byte;
    $socket->recv($byte,1);

    return ord($byte);
}

my sub getSize {
    my $byte = getByte();
    my $size = $byte & 0x7f;

    while ($byte & 0x80) {
        $byte = getByte();
        $size = ($size << 7) + ($byte & 0x7f);
    }

    return $size;
}

my sub getString {
    my $size = getSize();

    if ($size) {
        my $string;

        $socket->recv($string,$size);

        return $string;
    } else {
        return "";
    }
}

my sub demoHandshake {
    writeString($demoProtocolLabel);
    writeString($demoProtocolVersion);

    my $error = getString();

    if ( $error ne "") {
        die($error);
    }

    my $notification = getString();

    if ( $notification ne "") {
        CORE::say($notification);
    }
}

my sub ignition {
    writeString($token);
    writeString("");

    $token = getString();

    if ( $token eq "") {
        die(getString());
    }

    if (not($wPort eq ":0")) {
        my $url = "http://${wAddr}${wPort}/${cgi}.php?_token=${token}";

        CORE::say($url);
        CORE::say("^" x length($url));
        CORE::say("Open above URL in a web browser. Enjoy!");
        XDHq::SHRD::open($url);
    }
}

my sub serve {
    my ($callback, $userCallback, %callbacks) = @_;

    while(XDHq::SHRD::TRUE) {
        my $id = getByte();

        if ( $id eq 255) {   # Value reporting a new front-end.
            $id = getByte();    # The id of the new front-end.

            if( grep $_ eq $id, @instances ) {
                die("Instance of id '${id}' exists but should not !")
            }

            my $instance = XDHq::DEMO::Instance->new();

            print("--------------! ${instance}\n");

            $instance.set($callback->($userCallback, %callbacks, &$instance),$id);

            @instances[$id]=$instance;

            {   # Locking scope.
                lock($writeLock);
                writeByte($id);
                writeString($mainProtocolLabel);
                writeString($mainProtocolVersion);
            }
        } elsif ( not(grep $_ eq $id, @instances)) {
            die("Unknown instance of id '${id}'!")
        } elsif (not(@instances[$id]->testAndSetHandshake())) {
            my $error = getString();

            if ($error) {
                die($error);
            }

            getString();    # Language. Not handled yet.

            {   # Lock scope;
                lock($writeLock);
                writeByte($id);
                writeString("PRL");
            }
        } else {
            @instances[$id]->signal();

            cond_wait($globalCondition);
        }
    }
}
        
sub launch {
    my ($callback, $userCallback, %callbacks, $headContent) = @_;

    print("${callback}\n");


    init();
    demoHandshake();
    ignition();
    serve($callback, $userCallback, %callbacks);
}

return XDHq::SHRD::TRUE;
