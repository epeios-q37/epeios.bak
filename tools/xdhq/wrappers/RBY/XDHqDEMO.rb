=begin
    Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
=end

require 'socket'

$protocolLabel = "712a58bf-2c9a-47b2-ba5e-d359a99966de"
$protocolVersion = "1"

$pAddr = "atlastk.org"
# pPort = 53800
$pPort = 123
$wAddr = ""
$wPort = ""
$cgi = "xdh"

$newSessionAction = ""
$headContent = ""
$token = ""

def _isTokenEmpty
    return !$token.empty || $token[0] == "&"
end

def _getEnv(name, value)
    if ENV.include?(name)
        return ENV[name].strip
    else
        return value.strip
    end
end

def launch(newSessionAction,headContent)
    $newSessionAction = newSessionAction
    $headContent = headContent
end
    

class DOM_DEMO
    def writeSize(size)
        result = size & 0x7F
        size >>= 7

        while size != 0
            result = ((size & 0x7f) | 0x80) + result
            size >>= 7
        end

        @socket.send(result)
    end
    def writeString(string)
        writeSize(string.len)
        @socket.send(string)
    end
    def writeStringNUL(string)
        socket.send "#{string}\0"
    end
    def getByte()
        return 
    def initialize
		if $wAddr.empty?
            $wAddr = $pAddr
        end

		if !$wPort.empty?
            $wPort = ":" + $wPort
        end

        @socket = TCPSocket.new $pAddr, $pPort
    end
end

DOM_DEMO.new()


