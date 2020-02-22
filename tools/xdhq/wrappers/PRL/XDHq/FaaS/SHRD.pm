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

package XDHq::Faas::SHRD;

use threads;
use threads::shared;

$XDHq::Faas::SHRD::socket;

$XDHq::Faas::SHRD::writeLock;
share ($XDHq::Faas::SHRD::writeLock);

$XDHq::Faas::SHRD::globalCondition;
share($XDHq::Faas::SHRD::globalCondition);

sub writeByte {
    $socket->send(chr(shift));
}

sub writeSize {
    my $size = shift;

    my $result = chr($size & 0x7f);
    $size >>= 7;

    while ( $size ne 0) {
        $result =chr(($size & 0x7f) | 0x80) . $result;
        $size >>=7;
    }

    $socket->send($result);
}

sub writeString {
    my $string = shift;

    writeSize(length($string));
    $socket->send($string);
}

sub writeStrings {
    my $strings = shift;

    writeSize(scalar @{$strings});

    foreach my $string (@{$strings}) {
        writeString($string);
    }
}

sub writeStringNUL {
    $socket->send(shift . "\0");
}

sub getByte {
    my $byte;
    $socket->recv($byte,1);

    return ord($byte);
}

sub getSize {
    my $byte = getByte();
    my $size = $byte & 0x7f;

    while ($byte & 0x80) {
        $byte = getByte();
        $size = ($size << 7) + ($byte & 0x7f);
    }

    return $size;
}

sub getString {
    my $size = getSize();

    if ($size) {
        my $string;

        $socket->recv($string,$size);

        return $string;
    } else {
        return "";
    }
}

sub getStrings {
    my $size = getSize();
    my @strings;

    while ($size--) {
        push(@strings, getString());
    }

    return @strings;
}

return TRUE;

