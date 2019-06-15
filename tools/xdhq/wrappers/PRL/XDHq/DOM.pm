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

package XDHq::DOM;

use XDHq::SHRD;

use strict; use warnings;

sub _split {
    my $keysAndValues = shift;

    my @keys = keys %$keysAndValues;
    my @values = values %$keysAndValues;

    return (\@keys, \@values);  # Is there a way to avoid the use of variables?
}

sub _unsplit {
    my ($keys, $values) = @_;
    my %keysAndValues;

    foreach my $i (0 .. $#{$keys}) {
       $keysAndValues{@{$keys}[$i]}=@{$values}[$i];
    }

    return %keysAndValues;
}

sub new {
    my $class = shift;
    my $self = {};

    bless $self, $class;
    
    $self->{dom} = XDHq::DEMO::DOM->new(shift);

    return $self;
}

sub getAction {
    return shift->{dom}->getAction();
}

sub alert {
    my ($self, $message) = @_;

    $self->{dom}->call("Alert_1", XDHq::SHRD::RT_STRING, 1, $message, 0);

    # For the return value being 'RT_STRING' instead of 'TR_VOID',
	# see the 'alert' primitive in 'XDHqXDH'.
}

sub confirm {
    my ($self, $message) = @_;

    return $self->{dom}->call("Confirm_1", XDHq::SHRD::RT_STRING, 1, $message, 0) eq "true";
}

sub setLayoutPrivate {
    my ($self, $id, $xml, $xslFilename) = @_;

    $self->{dom}->call("SetLayout_1", XDHq::SHRD::RT_VOID, 3, $id, $xml, $xslFilename, 0);
}

sub setLayout {
    shift->setLayoutPrivate(shift, shift, "");
}

sub getContents {
    my ($self, $ids) = @_;

    my @result = $self->{dom}->call("GetContents_1", XDHq::SHRD::RT_STRINGS, 0, 1, $ids);

    return _unsplit($ids, \@result); # To modify to avoid the use of the @result variable.
}

sub getContent {
    my ($self, $id) = @_;

    my %result = $self->getContents([$id]);

    return $result{$id}; # To modify to avoid the use of the %result variable.
}

sub setContents {
    my $self = shift;

    my ($ids, $contents) = _split(shift);

    $self->{dom}->call("SetContents_1", XDHq::SHRD::RT_VOID, 0, 2, $ids, $contents);
}

sub setContent {
    my ($self, $id, $value) = @_;

    $self->setContents({$id => $value});
}

sub focus {
    shift->{dom}->call("Focus_1", XDHq::SHRD::RT_VOID, 1, shift, 0);
}

return XDHq::SHRD::TRUE;
