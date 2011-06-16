#!/usr/bin/env python

import urllib
import sys
import os
import optparse

#################################################
#file   pget.py
#brief  a python implementation of GNU wget
#author zeshengwu <wuzesheng@vip.qq.com>
#date   2011-06-16
#################################################

version = '1.0.0'

class PGetOptionParser(object):
    """class of all the options supported by pget"""

    def __init__(self):
        usage = 'Pget %s, a non-interactive network retriever.\n' % version + \
            'Usage: %prog [OPTION]... [URL]...\n'
        self.parser = optparse.OptionParser(usage)
        self.init_all_options()
        self.version = version

    def init_all_options(self):
        #group startup
        group = optparse.OptionGroup(self.parser, 'Startup')
        group.add_option('-V', '--version', action='callback',
                callback=self.print_version,
                help='display the version of Wget and exit.')
        group.add_option('-b', '--background', action='store_true',
                dest='background', help='go to background after startup.')
        group.add_option('-e', '--execute', action='store', type='string',
                dest='command', metavar='CMD',
                help='execute a \`.wgetrc\'-style command.')
        self.parser.add_option_group(group)

        #group logging and input file

    def parse_args(self, args):
        (self.values, self.args) = self.parser.parse_args(args)

    def print_version(self, opt, value, parser, *args, **kwargs):
        print self.version

    def has_option(self, opt_str):
        return self.parser.has_option(opt_str)

    def get_option_value(self, opt_str):
        if not self.has_option(opt_str):
            return None
        key = self.parser.get_option(opt_str).dest
        return self.values.__dict__[key]

class Crawler(object):
    """crawler class used to crawl a specified page"""
    pass

class PGet(object):
    """the main framework class of pget"""
    pass

def main():
    """the pget entry"""
    parser = PGetOptionParser()
    parser.parse_args(sys.argv[1:])

if __name__ == '__main__':
    main()
