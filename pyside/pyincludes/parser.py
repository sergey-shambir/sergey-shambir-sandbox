# -*- coding: utf-8 -*-

import os
import subprocess
import json

class Parser:
    def __init__(self):
        self.cpu_info = [{}]
        self.os_info = {}
        self.core_exe = "./hardinform"

    def parse_cpu_info(self):
        """
        Returns data-structured info about CPU
        """
        pipe = subprocess.Popen([self.core_exe, '-c'], 0, None, None,subprocess.PIPE)
        lines = pipe.stdout.readlines()
        x = 0
        json_str = ''
        while x < len(lines):
            json_str += lines[x].decode('utf-8').strip()
            x += 1
        decoder = json.decoder.JSONDecoder()
        self.cpu_info = decoder.decode(json_str)
        return self.cpu_info

    def parse_os_info(self):
        """
        Returns data-structured info about OS
        """
        pipe = subprocess.Popen([self.core_exe, '-o'], 0, None, None, subprocess.PIPE)
        lines = pipe.stdout.readlines()
        x = 0
        json_str = ''
        while x < len(lines):
            json_str += lines[x].decode('utf-8').strip()
            x += 1
        decoder = json.decoder.JSONDecoder()
        decoder.strict = False
        self.os_info = decoder.decode(json_str)
        return self.os_info

    def parse(self):
        self.parse_cpu_info()
        self.parse_os_info()
