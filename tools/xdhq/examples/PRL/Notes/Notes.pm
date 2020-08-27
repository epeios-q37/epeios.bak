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

package Notes;

use strict; use warnings;

use lib "atlastk";
use Atlas;

use Shared;

sub _put {
    my ($note, $id, $xml) = @_;

    $xml->pushTag("Note");
    $xml->putAttribute("id", $id);

    foreach my $key (keys %{$note}) {
        $xml->putTagAndValue($key,$note->{$key});
    }

    $xml->popTag();
}


sub new {
    my $class = shift;

    my $self = {
        pattern => "",
        hideDescriptions => Shared::FALSE,
        idx => 0,
        notes => [
			{
			title => '',
			description => '',
			},
			{
			title => 'Improve design',
			description => "Tastes and colors… (aka «CSS aren't my cup of tea…»)",
			},
			{
			title => 'Fixing bugs',
			description => "There are bugs ? Really ?",
			},
			{
			title => 'Implement new functionalities',
			description => "Although it's almost perfect…, isn't it ?",
			},
        ]
    };

    bless $self, $class;

    return $self;
}

sub handleDescriptions {
    my ($self, $dom) = @_;

    if ($self->{hideDescriptions}) {
        $dom->disableElement("ViewDescriptions")
    } else {
        $dom->enableElement("ViewDescriptions")
    }
}

sub displayList {
    my ($self, $dom) = @_;

    my $xml = Atlas::createXML("XDHTML");
    my %contents;

    $xml->pushTag("Notes");

    for my $idx (1 .. (scalar( @{$self->{notes}})-1)) {   # 0 is skipped as it serves as buffer for new notes.
        if ((lc substr($self->{notes}[$idx]{title},0,length($self->{pattern}))) eq $self->{pattern}) {
            _put($self->{notes}[$idx], $idx, $xml);
            $contents{"Description.$idx"} = $self->{notes}[$idx]->{description};
        }
    }

    $dom->inner("Notes", $xml, Shared::readAsset("Notes.xsl"));
    $dom->setContents(\%contents);
    $dom->enableElements(\@Shared::viewModeElements);
}

sub view {
    my ($self, $dom) = @_;

    $dom->enableElements(\@Shared::viewModeElements);
    $dom->setContent("Edit.$self->{idx}", "");
    $self->{idx} = -1;
}

return Shared::TRUE;
