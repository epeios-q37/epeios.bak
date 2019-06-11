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

package XDHq::DEMO::Instance;

use warnings;
use threads::shared;

sub new {
    my $class = shift;
    my $self = {}

    bless $self, $class;

    $self->{lock} : shared;
    $self->{handshakeDone} = undef;
    lock($self->{lock});

    return $self;
}

sub set {
    $self->{thread} = shift;
    $self->{id} = shift;
}

sub setAndTestHandshake {
    if ($self->{handshakeDone}) {
        return true;
    } else {
        $self->{handshakeDone} = !undef;
        return false;
    }
}

sub getId {
    return $self{id};
}

sub wait {
    cond_wait($self->{lock})
}

sub signal {
    cond_signal($self->{lock})
}

