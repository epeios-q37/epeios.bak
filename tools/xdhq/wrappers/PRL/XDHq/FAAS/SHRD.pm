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

package XDHq::FAAS::SHRD;

use threads;
use threads::shared;

$XDHq::FAAS::SHRD::socket;

$XDHq::FAAS::SHRD::writeLock;
share ($XDHq::FAAS::SHRD::writeLock);

$XDHq::FAAS::SHRD::globalCondition;
share($XDHq::FAAS::SHRD::globalCondition);

sub writeUInt {
    my $value = shift;

    my $result = chr($value & 0x7f);
    $value >>= 7;

    while ( $value ne 0) {
        $result =chr(($value & 0x7f) | 0x80) . $result;
        $value >>=7;
    }

    $socket->send($result);
}

sub writeSInt {
    my $value = shift;

    writeUInt( $value < 0 ? ( ( -$value - 1 ) << 1 ) | 1 : $value << 1 );
}

sub writeString {
    my $string = shift;

    writeUInt(length($string));
    $socket->send($string);
}

sub writeStrings {
    my $strings = shift;

    writeUInt(scalar @{$strings});

    foreach my $string (@{$strings}) {
        writeString($string);
    }
}

sub getByte {
    my $byte;
    $socket->recv($byte,1);

    return ord($byte);
}

sub getUInt {
    my $byte = getByte();
    my $value = $byte & 0x7f;

    while ($byte & 0x80) {
        $byte = getByte();
        $value = ($value << 7) + ($value & 0x7f);
    }

    return $value;
}

sub getSInt {
    my $value = getUInt();

    return $value & 1 ? -( ( $value >> 1 ) + 1 ) : $value >> 1;
}

sub getString {
    my $size = getUInt();

    if ($size) {
        my $string;

        $socket->recv($string,$size);

        return $string;
    } else {
        return "";
    }
}

sub getStrings {
    my $size = getUInt();
    my @strings;

    while ($size--) {
        push(@strings, getString());
    }

    return @strings;
}

return TRUE;

