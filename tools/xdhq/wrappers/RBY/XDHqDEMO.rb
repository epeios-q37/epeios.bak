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

$RT_VOID = 0
$RT_STRING = 1
$RT_STRINGS = 2

$protocolLabel = "712a58bf-2c9a-47b2-ba5e-d359a99966de"
$protocolVersion = "1"

$pAddr = "atlastk.org"
$pPort = 53800
$wAddr = ""
$wPort = ""
$cgi = "xdh"

$newSessionAction = ""
$headContent = ""
$token = ""

def tokenEmpty?()
    return !$token.empty?() || $token[0] == "&"
end

def getEnv(name, value = "")
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
    
def open(document)
    opener = case RbConfig::CONFIG['host_os']
    when /mswin|mingw/
       "start"
    when /cygwin/
        "cygstart"
    when /darwin/
        "open"
    else
        "xdg-open"
    end

    system opener + " #{document}"
end

class DOM_DEMO
    def writeSize(size)
        result = size & 0x7F
        size >>= 7

        while size != 0
            result = ((size & 0x7f) | 0x80) + result
            size >>= 7
        end

        @socket.write(result)
    end
    def writeString(string)
        writeSize(string.length())
        @socket.weite(string)
    end
    def writeStringNUL(string)
        socket.send "#{string}\0"
    end
    def getByte()
        return @socket.recv 1
    end
    def getString
        size = getSize()

        if size
            return @socket.recv(size)
        else
            return ""
        end
    end
    def getStrings()
        amount = getSize()
        strings = []

        while amount
            strings.push(getString())
            amount += 1
        end

        return strings
    end
    def initialize
        @firstLaunch = true
        token = ""

        case getEnv("ATK")
        when ""
        when "DEV"
            $pADDR = "localhost"
            $wPort = "8080"
            print("\tDEV mode !")
        when "TEST"
            $cgi = "xdh_"
        else
            abort("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !")
        end

		$pAddr = getEnv("ATK_PADDR", $pAddr)
		$pPort = getEnv("ATK_PPORT", $pPort.to_s())
		$wAddr = getEnv("ATK_WADDR", $wAddr)
		$wPort = getEnv("ATK_WPORT",$wPort)

		if $wAddr.empty?
            $wAddr = $pAddr
        end

		if !$wPort.empty?
            $wPort = ":" + $wPort
        end

        if tokenEmpty?()
            token = getEnv("ATK_TOKEN")
        end

        if !token.empty?
            $token = "&" + token
        end

        @socket = TCPSocket.new $pAddr, $pPort

        writeString($token)

        if tokenEmpty?()
            writeString(headContent)

            $token = getString()

            if tokenEmpty?()
                abort("Invalid connection information !!!")
            end

            if $wPort != ":0"
                url = "http://#{$wAddr}#{$wPort}/#{$cgi}.php?_token=#{$token}"

                print(url)
                print("Open above URL in a web browser. Enjoy!\n")
                open(url)
            end
        elsif getString() != $token
            abort("Unmatched token !!!")
        end

        getString() # Language.
        writeString($protocolLabel)
        writeString($protocolVersion)
        writeString("RBY")
    end

    def getAction()
        if @firstLaunch
            @firstLaunch = false
        else
            writeStringNUL("StandBy_1")
        end

        id = getString()
        action = getString()

        if action.empty?()
            action = $newSessionAction
        end

        return[action,id]
    end

    def call(command, type, *args)
        i = 0
        writeStringNUL(command)

        amount = args[i]

        while amount
            writeString(args[i])
            i += 1
            amount -= 1
        end

        amount = args[i]

        while amount
            writeString(args[i])
            i += 1
            amount -= 1
        end

        case type
        when RT_VOID
        when RT_STRING
            return getString()
        when RT_STRINGS
            return getStrings()
        else
            abort("Unknown return type !!!")
        end
    end
end

DOM_DEMO.new()
