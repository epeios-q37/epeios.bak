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

package Atlas;

use XDHq;
use XDHq::SHRD;
use XDHq::DOM;
use XDHq::XML;
use strict;
use threads;

# print("\t>>>>> " . __FILE__ . ":" . __LINE__ . "\n");

sub readAsset {
    return XDHq::readAsset(@_);
}

sub createXML {
    return XDHq::XML->new(shift);
}

sub createHTML {
    return createHTML(shift);
}

sub _worker {
    my ($userCallback, $instance, $callbacks) = @_;
    my $userObject;

    my $dom = XDHq::DOM->new($instance);

    if ( $userCallback ) {
        $userObject = &$userCallback();
    }

    while (XDHq::SHRD::TRUE) {
        my ($action, $id) = $dom->getAction();

        if ( $dom->isQuitting() ) {
            last;
        }

        if (($action eq "" ) or not $callbacks->{"_PreProcess"} or $callbacks->{"_PreProcess"}->($userObject,$dom, $id)) {
            if ( $callbacks->{$action}->($userObject,$dom, $id) and $callbacks->{"_PostProcess"} ) {
                $callbacks->{"_PostProcess"}->($userObject,$dom, $id);
            }
        }
    }

   # CORE::say("Quitting thread!");
}

sub _callback {
    my ($userCallback, $callbacks, $instance) = @_;

    return threads->create(\&_worker, $userCallback, $instance, $callbacks);
}

sub launch {
    my ($callbacks,$userCallback,$headContent,$dir) = @_;

    XDHq::launch(\&_callback,$userCallback,$callbacks,$headContent,$dir);
}

sub broadcastAction {
    my $action = shift // "";
    my $id = shift // "";

    if ( $action eq "" ) {
        die("There must be a non-empty 'action' parameter when calling 'broadcastAction(…)'!")
    }

    XDHq::broadcastAction($action, $id);
}


return XDHq::SHRD::TRUE;
