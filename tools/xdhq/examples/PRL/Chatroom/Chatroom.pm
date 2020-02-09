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

package Chatroom;

use strict; use warnings;

use lib "atlastk";
use Atlas;

use Shared;

sub new {
    my $class = shift;
    my $self = {
        lastMessage => 0,
        pseudo => ""
    };

    bless $self, $class;

    return $self;
}

sub _buildXML {
    my $self = shift;

    my $xml = Atlas::createXML("XDHTML");
    $xml->pushTag("Messages");
    $xml->putAttribute("pseudo", $self->{pseudo});

    foreach my $i (reverse $self->{lastMessage} .. $#Shared::messageContents ) {
        $xml->pushTag("Message");
        $xml->putAttribute("id", $i);
        $xml->putAttribute("pseudo", $Shared::messagePseudos[$i]);
        $xml->putValue($Shared::messageContents[$i]);
        $xml->popTag();
    }

    $self->{lastMessage} = $#Shared::messageContents + 1;

    $xml->popTag();

    return $xml;
}

sub displayMessages {
    my ($self, $dom) = @_;

    if ($#Shared::messageContents >= $self->{lastMessage}) {
        $dom->prependLayoutXSL("Board", $self->_buildXML(), Shared::readAsset("Messages.xsl"));
    }
}

sub handlePseudo {
    my ($self, $pseudo) = @_;

    if (grep { $_ eq $pseudo } @Shared::pseudos) {
        return Shared::FALSE;
    } else {
        push(@Shared::pseudos, $pseudo);
        return Shared::TRUE;
    }
}

sub addMessage {
    my $self = shift;
    my $message = Shared::trim(shift);

    if ( $message) {
        CORE::say("'$self->{pseudo}': $message");
        push(@Shared::messageContents, $message);
        push(@Shared::messagePseudos, $self->{pseudo});
    }
}

return Shared::TRUE;
