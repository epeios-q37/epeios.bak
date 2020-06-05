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

use strict; use warnings;

use lib "atlastk";
use Atlas;

use lib "Chatroom";
use Shared;
use Chatroom;

sub acConnect {
    my ($chatroom, $dom) = @_;

    $dom->setLayout("", Shared::readAsset("Main.html"));
    $dom->focus("Pseudo");

    $chatroom->displayMessages($dom);
}

sub acSubmitPseudo {
    my ($chatroom, $dom) = @_;

    my $pseudo = Shared::trim($dom->getContent("Pseudo"));

    if ( not ($pseudo)) {
        $dom->alert("Pseudo. can not be empty!");
        $dom->setContent("Pseudo", "");
        $dom->focus("Pseudo");
    } elsif ($chatroom->handlePseudo(uc $pseudo)) {
        $chatroom->{pseudo} = $pseudo;
        $dom->addClass("PseudoButton", "hidden");
		$dom->disableElements(["Pseudo", "PseudoButton"]);
		$dom->enableElements(["Message", "MessageButton"]);
		$dom->setContent("Pseudo", $pseudo);
		$dom->focus("Message");
		CORE::say("\t>>>> New user: $pseudo");
    } else {
        $dom->alert("Pseudo. not available!");
        $dom->setContent("Pseudo", $pseudo);
        $dom->focus("Pseudo");
    }
}

sub acSubmitMessage {
    my ($chatroom, $dom) = @_;

	my $message = $dom->getContent("Message");
	
    $dom->setContent("Message", "");
	$dom->focus("Message");
	$chatroom->addMessage($message);
	$chatroom->displayMessages($dom);
    Atlas::broadcastAction("Update");
}

sub acUpdate {
    my ($chatroom, $dom) = @_;

    $chatroom->displayMessages( $dom );
}

my %callbacks = (
    "" => \&acConnect,
    "SubmitPseudo" => \&acSubmitPseudo,
    "SubmitMessage" => \&acSubmitMessage,
    "Update" => \&acUpdate,
);

Atlas::launch(\%callbacks, sub {return Chatroom->new()}, Shared::readAsset("Head.html"));
