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

package XDHq;

use XDHq::SHRD;
use XDHq::DEMO;
use XDHq::DEMO::DOM;

use Cwd;

sub _getAssetPath {
    if (XDHq::SHRD::isDev() ) {
        return "/home/csimon/epeios/tools/xdhq/examples/common/" . shift;
    } else {
        return getcwd . '/' . shift;
    }
}

sub _getAssetFilename {
    my ($path, $dir) = @_;

    return _getAssetPath($dir) . '/' . $path; 
}

sub readAsset {
    open FILEHANDLE, _getAssetFilename(shift, shift) or die $!;

    return do { local $/; <FILEHANDLE> };
}

sub launch {
    my ($callback,$userCallback,$callbacks,$headContent,$dir) = @_;

    XDHq::DEMO::launch($callback,$userCallback,$callbacks,$headContent);
}

return XDHq::SHRD::TRUE;
