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

class PGetOptionParser(optparse.OptionParser):
    """class of all the options supported by pget"""
    def __init__(self):
        usage = 'Pget %s, a non-interactive network retriever.\n' % version
        #usage .= 'Usage: %prog [OPTION]... [URL]...\n\n'
        usage += 'Mandatory arguments to long options are mandatory for short options too.\n\n'
        optparse.OptionParser(usage)

        group = optparse.OptionGroup(parser, 'Startup')
        group.add_option('-V', '--version', action='store', type='string',
                         dest='version', help='display the version of Wget and exit.')
        group.add_option('-h', '--help', action='callback', callback=self.parser.print_help,
                         help='print this help.')
        group.add_option('-e', '--execute', metavar='COMMAND', action='store',
                         dest='command', help='execute a \`.wgetrc\'-style command.')
        self.parser.add_option_group(group)

class Crawler(object):
    """crawler class used to crawl a specified page"""
    pass

class PGet(object):
    """the main framework class of pget"""
    pass

def main():
    """the pget entry"""
    parser = PGetOptionParser()

if __name__ == '__main__':
    main()
