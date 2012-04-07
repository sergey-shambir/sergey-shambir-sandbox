#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import sys
irc = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
irc.connect ( ( "google.com", 80 ) )

irc.send('GET / HTTP/1.1\r\n')
irc.send('User-agent: Mozilla/5.0 (wikibooks test)\r\n\r\n')
f = irc.recv(4096)
print f
