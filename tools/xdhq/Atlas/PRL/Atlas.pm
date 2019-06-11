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

use XDHq;
use XDHq::SHRD;
use XDHq::DEMO::DOM;
use strict;
use threads;

sub readAsset {
    return XDHq::readAsset(@_);
}

my sub worker {
    my ($userCallback, $dom, %callbacks) = @_;
    my $userObject;

    if ( $userCallback ) {
        $userObject = &$userCallback();
    }

    while (XDHq::SHRD::TRUE) {
        my ($action, $id) = $dom->getAction();
        my $callback = $callbacks{$action};

        &$callback($userObject,$dom, $id);
    }
}

my sub callback {
    my ($userCallback, %callbacks, $instance) = @_;

    print("-------------> ${instance}\n");

    return threads->create(\&worker, $userCallback, XDHq::DEMO::DOM->new($instance), %callbacks);
}

sub launch {
    my (%callbacks,$userCallback,$headContent,$dir) = @_;

    XDHq::launch(\&callback,$userCallback,%callbacks,$headContent,$dir);
}

return XDHq::SHRD::TRUE;
