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

package DOM;

use XDHqSHRD;
use warnings;
use threads::shared;

$globalCondition : shared;

lock($globalCondition)

sub new {
    my $class = shift;
    my $self = {}

    bless $self, $class;

    $self->{firstLaunch} = XDHqSHRD::TRUE;
    $self->{instance} = shift;
}

sub wait {
    $self->{instance}->wait();
}

sub signal {
    cond_signal($globalCondition);
}

sub getAction {
    if ( $self->{firstLaunch}) {
        $self->{firstLaunch} = XDHqSHRD::TRUE;
    } else { # Also a lock scope.
        writeByte(self->{instance}->getId());
        writeStringNUL("StandBy_1");
    }

    self->wait();

    my $id = getString();
    my $action = getString();

    $self->signal();

    return ($id, $action);
}

sub call {
    my $self = shift;
    my $command = shift;
    my $type = shift;

    {   # Lock scope;
        writeByte($self->{instance}->getId());
        writeStringNUL($command);

        my $amount = shift;

        while($amount) {
            writeString(shift);
            $amount -= 1;
        }

        $amount = shift;

        while($amount) {
            writeStrings(shift);
            $amount-=1;
        }

        if($type eq XDHqSHRD::RT_STRINGS) {
            $self->wait();
            my $result = getString();
            $self->signal();
            return $result;
        } elsif($type eq XDHqSHRD::RT_STRINGS) {
            $self->wait();
            my $result = getStrings();
            $self->signal();
            return $result;
        } elif (not ($type eq XDHqSHRD::RT_VOID) {
            die("Unknown return type !!!")
        }
    }
}

return XDHqSHRD::TRUE;