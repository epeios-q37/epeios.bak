package XDHqDEMO;
use IO::Socket::INET;
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
}

init();

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
}
        
sub launch {
    # auto-flush on socket
    $| = 1;

    # create a connecting socket
    $socket = new IO::Socket::INET (
        PeerHost => $pAddr,
        PeerPort => $pPort,
        Proto => 'tcp',
    );
    die "Error on connection to '$pAddr:$pPort': $! !!!\n" unless $socket;

    demoHandshake();
    ignition();
}

return 1;

