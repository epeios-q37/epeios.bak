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
use Thread::Semaphore;

sub new {
    my $classe  = shift;

    $classe = ref($classe) || $classe;

    my $this = {};
    share($this);

    bless $this, $classe;

    $this->{id} = shift;

    $this->{readLock} = Thread::Semaphore->new();
    $this->{readLock}->down();

    return $this;
}

sub waitForData {
    my $this = shift;

    $this->{readLock}->down();

    if ( $this->{quit} ) {
        XDHq::FAAS::SHRD::instanceDataRead();
        threads->exit();
    }
}

sub dataAvailable {
    shift->{readLock}->up();
}

return XDHq::SHRD::TRUE;
