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

package XDHq::DEMO::DOM;

use XDHq::SHRD;
use warnings;
use threads;
use threads::shared;

sub new {
    my $class = shift;
    my $self = {};

    bless $self, $class;

    $self->{firstLaunch} = XDHq::SHRD::TRUE;
    $self->{instance} = shift;

    return $self;
}

sub wait {
    my $self = shift;

    XDHq::DEMO::Instance::wait($self->{instance});
}

sub signal {
    print("\t>>>>> " . __FILE__ . ":" . __LINE__ . "\n");    
    lock($XDHq::DEMO::SHRD::globalCondition);
    cond_signal($XDHq::DEMO::SHRD::globalCondition);
    print("\t>>>>> " . __FILE__ . ":" . __LINE__ . "\n");    
}

sub getAction {
    my $self = shift;

    if ( $self->{firstLaunch}) {
        $self->{firstLaunch} = XDHq::SHRD::FALSE;
    } else { 
        {# Also a lock scope.
        lock($XDHq::DEMO::SHRD::writeLock);
        XDHq::DEMO::SHRD::writeByte($self->{instance}->{id});
        XDHq::DEMO::SHRD::writeStringNUL("StandBy_1");
        }
    }

    print("\t>>>>> " . __FILE__ . ":" . __LINE__ . "\n");    

    $self->wait();

    print("\t>>>>> " . __FILE__ . ":" . __LINE__ . "\n");    

    my $id = XDHq::DEMO::SHRD::getString();
    my $action = XDHq::DEMO::SHRD::getString();

    $self->signal();

    return ($action,$id);
}

sub call {
    my $self = shift;
    my $command = shift;
    my $type = shift;

    {   # Lock scope;
        lock($XDHq::DEMO::SHRD::writeLock);

        XDHq::DEMO::SHRD::writeByte($self->{instance}->{id});
        XDHq::DEMO::SHRD::writeStringNUL($command);

   
        my $amount = shift;

        while($amount) {
            XDHq::DEMO::SHRD::writeString(shift);
            $amount -= 1;
        }

        $amount = shift;

        while($amount) {
            XDHq::DEMO::SHRD::writeStrings(shift);
            $amount-=1;
        }
    }

    if($type eq XDHq::SHRD::RT_STRING) {
        $self->wait();
        my $result = XDHq::DEMO::SHRD::getString();
        $self->signal();
        return $result;
    } elsif($type eq XDHq::SHRD::RT_STRINGS) {
        $self->wait();
        my $result = XDHq::DEMO::SHRD::getStrings();
        $self->signal();
        return $result;
    } elsif (not ($type eq XDHq::SHRD::RT_VOID)) {
        die("Unknown return type !!!")
    }
}

return XDHqSHRD::TRUE;