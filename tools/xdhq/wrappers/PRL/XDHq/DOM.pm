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

my sub split {
    my $keysAndValues = shift;

    my @keys, @values;

    foreach my $key (keys(%h)) {
        push(@keys, $key);
        push(@values, $keysAndValues{$key});
    }

    return [@keys,@values];
}

my sub unsplit {
    my ($keys, $values) = @_;
    my %keysAndValues;

    foreach my $i (0 .. $#{$keys}) {
       $keysAndValues{${keys}[$i]}=${values}[$i];
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

    return $self->{dom}->call("Confirm_1", XDHq::SHRD::RT_STRING, 1, $message, 0) eq "True";
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

    return unsplit($ids, $self->{dom}->call("GetContents_1", XDHq::SHRD::RT_STRINGS, 0, 1, $ids));
}

sub getContent {
    my ($self, $id) = @_;

    return %{$self->getContents([$id])}{$id};
}

sub focus {
    shift->{dom}->call("Focus_1", XDHq::SHRD::RT_VOID, 1, shift, 0);
}

return XDHqSHRD::TRUE;
