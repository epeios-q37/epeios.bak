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
    
    $self->{dom} = XDHq::FAAS::DOM->new(shift);

    return $self;
}

sub getAction {
    return shift->{dom}->getAction();
}

sub isQuitting {
    return shift->{dom}->isQuitting();
}

sub _execute {
    return shift->{dom}->call("Execute_1", shift, shift);
}

sub executeVoid {
    shift->_execute(XDHq::SHRD::RT_VOID, shift);
}

sub executeString {
    return shift->_execute(XDHq::SHRD::RT_STRING, shift);
}

sub executeStrings {
    return shift->_execute(XDHq::SHRD::RT_STRINGS, shift);
}

sub flush {
    shift->{dom}->call("Flush_1", XDHq::SHRD::RT_STRING, shift);
}

sub alert {
    my ($self, $message) = @_;

    $self->{dom}->call("Alert_1", XDHq::SHRD::RT_STRING, $message);

    # For the return value being 'RT_STRING' instead of 'TR_VOID',
	# see the 'alert' primitive in 'XDHqXDH'.
}

sub confirm {
    my ($self, $message) = @_;

    return $self->{dom}->call("Confirm_1", XDHq::SHRD::RT_STRING, $message) eq "true";
}

sub _handleLayout {
    my ($self, $variant, $id, $xml, $xslFilename) = @_;

    $self->{dom}->call("HandleLayout_1", XDHq::SHRD::RT_VOID, $variant, $id, ref $xml eq "XDHq::XML" ? $xml->toString() : $xml, $xslFilename);
}

sub prependLayout { # Deprecated!
    shift->_handleLayout("Prepend", shift, shift, "");
}

sub setLayout { # Deprecated!
    shift->_handleLayout("Set", shift, shift, "");
}

sub appendLayout { # Deprecated!
    shift->_handleLayout("Append", shift, shift, "");
}

sub _handleLayoutXSL {
    my ($self, $variant, $id, $xml, $xsl) = @_;

    if( XDHq::SHRD::TRUE) { # Replaced with a FAAS/SLFH test when available.
        $xsl =~ s/([^-A-Za-z0-9_.!~*'() ])/sprintf("%%%02X", ord($1))/eg;
#        $xsl =~ tr/ /+/;
        $xsl = "data:text/xml;charset=utf-8," . $xsl;
    }

    $self->_handleLayout( $variant, $id, $xml, $xsl);
}

sub prependLayoutXSL { # Deprecated!
    shift->_handleLayoutXSL("Prepend", shift, shift, shift);
}

sub setLayoutXSL { # Deprecated!
    shift->_handleLayoutXSL("Set", shift, shift, shift);
}

sub appendLayoutXSL { # Deprecated!
    shift->_handleLayoutXSL("Append", shift, shift, shift);
}

sub _layout {
    my ($self, $variant, $id, $xml, $xsl) = @_;

    if ( defined $xsl) {
        $xsl =~ s/([^-A-Za-z0-9_.!~*'() ])/sprintf("%%%02X", ord($1))/eg;
        $xsl = "data:text/xml;charset=utf-8," . $xsl;       
    } else {
        $xsl = "";
    }

    $self->{dom}->call("HandleLayout_1", XDHq::SHRD::RT_VOID, $variant, $id, ref $xml eq "XDHq::XML" ? $xml->toString() : $xml, $xsl);
}

sub before {
    shift->_layout("beforebegin",shift,shift,shift)
}

sub begin {
    shift->_layout("afterbegin",shift,shift,shift)
}

sub inner {
    shift->_layout("inner",shift,shift,shift)
}

sub end {
    shift->_layout("afterend",shift,shift,shift)
}

sub after {
    shift->_layout("beforeend",shift,shift,shift)
}

sub getContents { # Deprecated!
    my ($self, $ids) = @_;

    my @result = $self->{dom}->call("GetContents_1", XDHq::SHRD::RT_STRINGS, $ids);

    return _unsplit($ids, \@result); # To modify to avoid the use of the @result variable.
}

sub getContent { # Deprecated!
    my ($self, $id) = @_;

    my %result = $self->getContents([$id]);

    return $result{$id}; # To modify to avoid the use of the %result variable.
}

sub setContents { # Deprecated!
    my $self = shift;

    my ($ids, $contents) = _split(shift);

    $self->{dom}->call("SetContents_1", XDHq::SHRD::RT_VOID, $ids, $contents);
}

sub setContent { # Deprecated!
    shift->setContents({shift() => shift});
}

sub getValues {
    my ($self, $ids) = @_;

    my @result = $self->{dom}->call("GetValues_1", XDHq::SHRD::RT_STRINGS, $ids);

    return _unsplit($ids, \@result); # To modify to avoid the use of the @result variable.
}

sub getValue {
    my ($self, $id) = @_;

    my %result = $self->getValues([$id]);

    return $result{$id}; # To modify to avoid the use of the %result variable.
}

sub setValues {
    my $self = shift;

    my ($ids, $calues) = _split(shift);

    $self->{dom}->call("SetValues_1", XDHq::SHRD::RT_VOID, $ids, $values);
}

sub setValue {
    shift->setValues({shift() => shift});
}

sub getMarks {
    my ($self, $ids) = @_;

    my @result = $self->{dom}->call("GetMarks_1", XDHq::SHRD::RT_STRINGS, $ids);

    return _unsplit($ids, \@result); # To modify to avoid the use of the @result variable.
}

sub getMark {
    my ($self, $id) = @_;

    my %result = $self->getMarks([$id]);

    return $result{$id}; # To modify to avoid the use of the %result variable.
}

sub setMarks {
    my $self = shift;

    my ($ids, $marks) = _split(shift);

    $self->{dom}->call("SetMarks_1", XDHq::SHRD::RT_VOID, $ids, $marks);
}

sub setMark {
    shift->setMarks({shift() => shift});
}

=pod
sub createElement {
    return shift->{dom}->call("CreateElement_1", XDHq::SHRD::RT_STRING, 2, shift, shift, 0);
}

sub insertChild {
    shift->{dom}->call("InsertChild_1", XDHq::SHRD::RT_VOID, 2, shift, shift, 0);
}
=cut

sub _handleClasses {
    shift->{dom}->call("HandleClasses_1", XDHq::SHRD::RT_VOID, shift, _split(shift));
}

sub addClasses {
    shift->_handleClasses("Add", shift );
}

sub removeClasses {
    shift->_handleClasses("Remove", shift);
}

sub toggleClasses {
    shift->_handleClasses("Toggle", shift);
}

sub addClass {
    shift->addClasses({shift() => shift})
}

sub removeClass {
    shift->removeClasses({shift() => shift});
}

sub toggleClass {
    shift->toggleClasses({shift() => shift});
}

sub enableElements {
    shift->{dom}->call("EnableElements_1", XDHq::SHRD::RT_VOID, shift);
}

sub enableElement {
    shift->enableElements([shift]);
}

sub disableElements {
    shift->{dom}->call("DisableElements_1", XDHq::SHRD::RT_VOID, shift);
}

sub disableElement {
    shift->disableElements([shift]);
}

sub setAttribute {
    shift->{dom}->call("SetAttribute_1", XDHq::SHRD::RT_VOID, shift, shift, shift )
}

sub getAttribute {
    return shift->{dom}->call("GetAttribute_1", XDHq::SHRD::RT_STRING, shift, shift )
}

sub removeAttribute {
    return shift->{dom}->call("RemoveAttribute_1", XDHq::SHRD::RT_VOID, shift, shift )
}

sub setProperty {
    shift->{dom}->call("SetProperty_1", XDHq::SHRD::RT_VOID, shift, shift, shift )
}

sub getProperty {
    return shift->{dom}->call("GetProperty_1", XDHq::SHRD::RT_STRING, shift, shift )
}

sub focus {
    shift->{dom}->call("Focus_1", XDHq::SHRD::RT_VOID, shift);
}

sub parent {
    return shift->{dom}->call("Parent_1", XDHq::SHRD::RT_STRING, shift);
}

sub firstChild {
    return shift->{dom}->call("FirstChild_1", XDHq::SHRD::RT_STRING, shift);
}

sub lastChild {
    return shift->{dom}->call("LastChild_1", XDHq::SHRD::RT_STRING, shift);
}

sub previousSibling {
    return shift->{dom}->call("PreviousSibling_1", XDHq::SHRD::RT_STRING, shift);
}

sub nextSibling {
    return shift->{dom}->call("NextSibling_1", XDHq::SHRD::RT_STRING, shift);
}

sub scrollTo {
    shift->{dom}->call("ScrollTo_1", XDHq::SHRD::RT_VOID, shift);
}

return XDHq::SHRD::TRUE;
