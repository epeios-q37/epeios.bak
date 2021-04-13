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

use lib "Notes";
use Shared;
use Notes;

sub acConnect {
    my ($notes, $dom) = @_;

    $dom->inner("", Shared::readAsset("Main.html"));
    $notes->displayList($dom);
}

sub acToggleDescriptions {
    my ($notes, $dom, $id) = @_;

    $notes->{hideDescriptions} = $dom->getValue($id) eq "true";
    $notes->handleDescriptions($dom);
}

sub acSearch {
    my ($notes, $dom) = @_;

    $notes->{pattern} = lc $dom->getValue("Pattern");
    $notes->displayList($dom);
}

sub acEdit {
    my ($notes, $dom, $id) = @_;

    my $idx = $dom->getValue($id);
    $notes->{idx} = int($idx);

    my %note = %{$notes->{notes}[$notes->{idx}]};

    $dom->inner("Edit.$idx", Shared::readAsset("Note.html"));
    $dom->setValues({Title => $note{title}, Description => $note{description}});
    $dom->disableElements(\@Shared::viewModeElements);
    $dom->focus("Title");
}

sub acDelete {
    my ($notes, $dom, $id) = @_;

    if ($dom->confirm("Are you sure you want to delete this entry?")) {
        splice @{$notes->{notes}}, int($dom->getValue($id)), 1;
        $notes->displayList($dom);
    }
}

sub acSubmit {
    my ($notes, $dom) = @_;

    my %result = $dom->getValues(["Title","Description"]);
    my $title = Shared::trim($result{Title});
    my $description = $result{Description};

    if ( $title) {
        $notes->{notes}[$notes->{idx}] =  { title => $title, description => $description };

        if ( $notes->{idx} eq 0) {
            unshift @{$notes->{notes}}, { title => '', description => ''};
            $notes->displayList($dom);
        } else {
            $dom->setValues({ "Title.$notes->{idx}" => $title, "Description.$notes->{idx}" => $description});
            $notes->view($dom);
        }
    } else {
        $dom->alert("Title can not be empty!");
        $dom->focus("Title");
    }
}

sub acCancel {
    my ($notes, $dom) = @_;

    my %note = %{$notes->{notes}[$notes->{idx}]};

    my %result = $dom->getValues(["Title","Description"]);
    my $title = Shared::trim($result{Title});
    my $description = $result{Description};

    if ( ($title ne $note{title}) or ($description ne $note{description})) {
        if ($dom->confirm("Are you sure you want to cancel your modifications?")) {
            $notes->view($dom);
        }
    } else {
        $notes->view($dom);
    }
}

my %callbacks = (
    "" => \&acConnect,
	"ToggleDescriptions" => \&acToggleDescriptions,
	"Search" => \&acSearch,
	"Edit" => \&acEdit,
	"Delete" => \&acDelete,
	"Submit" => \&acSubmit,
	"Cancel" => \&acCancel,
);

Atlas::launch(\%callbacks, sub {return Notes->new()}, Shared::readAsset("Head.html"));
