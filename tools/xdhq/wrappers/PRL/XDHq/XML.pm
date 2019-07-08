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

package XDHq::XML;

use XDHq::SHRD;

use strict; use warnings;

sub _write {
    shift->{xml} .= shift . "\0";
}

sub new {
    my $class = shift;
    my $self = {
        xml => ""
    };

    bless $self, $class;
    
    $self->_write("dummy");
    $self->_write(shift);

    return $self;
}

sub pushTag {
    my $self = shift;

    $self->{xml} .= ">";
    $self->_write(shift);
}

sub popTag {
    shift->{xml} .= "<";
}

sub putAttribute {
    my $self = shift;

    $self->{xml} .= "A";
    $self->_write(shift);
    $self->_write(shift);
}

sub putValue {
    my $self = shift;

    $self->{xml} .= "V";
    $self->_write(shift);
}

sub putTagAndValue {
    my $self = shift;

    $self->pushTag(shift);
    $self->putValue(shift);
    $self->popTag();
}

sub toString {
#    CORE::say("XML: " . shift->{xml});
    return shift->{xml};
}

return XDHq::SHRD::TRUE;
