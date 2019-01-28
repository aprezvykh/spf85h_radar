#!/bin/sh

# $Id: svn_version.sh 756 2007-01-23 04:38:43Z michael $
# $URL: https://ssl.bulix.org/svn/lcd4linux/trunk/svn_version.sh $


OLD_VERSION=`cat svn_version.h 2>/dev/null`

if [ -d .svn ]; then
    NEW_VERSION="#define SVN_VERSION \"`svnversion -n`\""
fi

if [ "$NEW_VERSION" != "$OLD_VERSION" ]; then
    echo $NEW_VERSION >svn_version.h
fi
