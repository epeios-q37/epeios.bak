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

use lib "atlastk";
use Atlas;

sub readAsset {
    return Atlas::readAsset( shift, "Blank" );
}

sub acConnect {
    my ($blank, $dom) = @_;

    $dom->inner("", readAsset("Main.html"));
    $dom->addClass("Input", "hidden");
}

sub acShowInput {
    my ($blank, $dom) = @_;

    $dom->removeClass("Input", "hidden");
    $dom->focus("Pattern");
}

my %callbacks = (
    "" => \&acConnect,
    "Submit" => sub {my ($blank, $dom) = @_; $dom->setValue("Pattern", uc $dom->getValue("Pattern"));},
    "HideInput" => sub {my ($blank, $dom) = @_; $dom->addClass("Input", "hidden");},
    "ShowInput" => \&acShowInput
);

Atlas::launch(\%callbacks, sub {return undef;}, readAsset("Head.html"));
