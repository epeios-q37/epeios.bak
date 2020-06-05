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

# Althought it bares some similarities with a class definition,
# this file does NOT define a class, due to its sharing between several threads.

package XDHq::FAAS::Instance;

use XDHq::SHRD;
use warnings;
use threads;
use threads::shared;

sub new {

    my $lock: shared;

    my %self : shared = (
        lock => \$lock,
        handshakeDone => XDHq::SHRD::FALSE
    );

    return \%self;
}

sub set {
    my ($self, $thread, $id) = @_;

#    $self->{thread} = $thread;
    $self->{id} = $id;
}

sub testAndSetHandshake {
    my $self = shift;

    if ($self->{handshakeDone}) {
        return XDHq::SHRD::TRUE;
    } else {
        $self->{handshakeDone} = XDHq::SHRD::TRUE;
        return XDHq::SHRD::FALSE;
    }
}

sub getId {
    my $self = shift;

    return $self->{id};
}

sub wait {
    my $self = shift;

    lock(${$self->{lock}});
    cond_wait(${$self->{lock}});
}

sub signal {
    my $self = shift;

    lock(${$self->{lock}});
    cond_signal(${$self->{lock}});
}

return XDHq::SHRD::TRUE;
