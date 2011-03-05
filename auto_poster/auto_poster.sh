#!/bin/sh

############################################################
#brief   A auto-poster used to post posts to a Discuz bbs
#author  zeshengwu <wuzesheng86@gmail.com>
#since   2011-03-04
#version 1.0.0
############################################################

if [ $# -ne 3 ]
then
	echo "usage: $0 board_id subject message"
	exit 1
fi

#arguments
g_fid=$1
g_subject=`echo "$2" | iconv -fgb2312 -tutf8`
g_message=`echo "$3" | iconv -fgb2312 -tutf8`

#site info
host=bbs.sosospider.com
user=zeshengwu
passwd=zeshengwu

page=".page" 
header=".header"
cookie_file=".cookie"
user_agent="Mozilla/5.0 (Windows; U; Windows NT 6.1; zh-CN; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8"

#login discuz bbs
function login()
{
	local url="http://$host/member.php?mod=logging&action=login&loginsubmit=yes&infloat=yes&inajax=1" 
	local data="fastloginfield=username&username=$user&cookietime=2592000&password=$passwd&quickforward=yes&handlekey=ls&questionid=0&answer=" 
	wget $url -d \
              --save-cookies=$cookie_file \
              --user-agent="$user_agent" \
              --post-data="$data" \
              -O $page \
              -a $header 2>/dev/null
}

#get formhash from "$page"
function get_formhash()
{
	formhash=`grep -Eio "formhash=.*" $page \
			 | awk -F'"' '{print $1}' \
			 | awk -F'=' '{print $2}'`
	echo -n "$formhash"
}

#goto specified board
function goto_board()
{
	local fid=$1
	local url="http://$host/forum.php?mod=forumdisplay&fid=$fid"
	wget $url -d \
              --load-cookies=$cookie_file \
              --user-agent="$user_agent" \
              --save-cookies=$cookie_file \
              -O $page \
              -a $header 2>/dev/null
}

#post a new post
function post()
{
	local fid=$1
	local subject=$2
	local message=$3

	goto_board $fid
	local formhash=`get_formhash`
	local referer="http://$host/forum.php?mod=forumdisplay&fid=$fid"
	local url="http://$host/forum.php?mod=post&action=newthread&fid=$fid&topicsubmit=yes&infloat=yes&handlekey=fastnewpost&inajax=1"
	local data="subject=$subject&message=$message&formhash=$formhash&usesig=1"
	wget $url -d \
              --load-cookie=$cookie_file \
              --user-agent="$user_agent" \
              --referer="$referer" \
              --post-data="$data" \
              -O $page \
              -a $header 2>/dev/null
}

#clear temporary files
function clear()
{
	rm $page && rm $header && rm $cookie_file
}

login
post $g_fid "$g_subject" "$g_message"
clear
