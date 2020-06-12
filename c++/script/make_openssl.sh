#!/bin/bash

OPENSSL=openssl
OPENSSL_PATH=https://github.com/openssl/openssl.git
CUR_DIR=
download() {
    if [ -d "$1" ]; then
        echo "$1 existed."
    else
        echo "$1 not existed, begin to download..."
        git clone $2
        if [ $? -eq 0 ]; then
            echo "download $1 successed";
        else
            echo "Error: download $1 failed";
            return 1;
        fi
    fi
    return 0
}

check_user() {
    if [ $(id -u) != "0" ]; then
        echo "Error: You must be root to run this script, please use root to install im"
        exit 1
    else
        echo "[ CHECK ] Already root."
    fi
}

get_cur_dir() {
    # Get the fully qualified path to the script
    case $0 in
        /*)
            SCRIPT="$0"
            ;;
        *)
            PWD_DIR=$(pwd);
            SCRIPT="${PWD_DIR}/$0"
            ;;
    esac
    # Resolve the true real path without any sym links.
    CHANGED=true
    while [ "X$CHANGED" != "X" ]
    do
        # Change spaces to ":" so the tokens can be parsed.
        SAFESCRIPT=`echo $SCRIPT | sed -e 's; ;:;g'`
        # Get the real path to this script, resolving any symbolic links
        TOKENS=`echo $SAFESCRIPT | sed -e 's;/; ;g'`
        REALPATH=
        for C in $TOKENS; do
            # Change any ":" in the token back to a space.
            C=`echo $C | sed -e 's;:; ;g'`
            REALPATH="$REALPATH/$C"
            # If REALPATH is a sym link, resolve it.  Loop for nested links.
            while [ -h "$REALPATH" ] ; do
                LS="`ls -ld "$REALPATH"`"
                LINK="`expr "$LS" : '.*-> \(.*\)$'`"
                if expr "$LINK" : '/.*' > /dev/null; then
                    # LINK is absolute.
                    REALPATH="$LINK"
                else
                    # LINK is relative.
                    REALPATH="`dirname "$REALPATH"`""/$LINK"
                fi
            done
        done

        if [ "$REALPATH" = "$SCRIPT" ]
        then
            CHANGED=""
        else
            SCRIPT="$REALPATH"
        fi
    done
    # Change the current directory to the location of the script
    CUR_DIR=$(dirname "${REALPATH}")

    echo "[ PATH ]" $CUR_DIR
}

build_log4cxx(){
    cd ~

    wget http://www.cpan.org/modules/by-module/Data/Data-Dumper-2.154.tar.gz
    tar xvzf Data-Dumper-2.154.tar.gz
    cd Data-Dumper-2.154
    perl Makefile.PL
    make
    make install
    cd ..

    download $OPENSSL $OPENSSL_PATH

    if [ -d $OPENSSL ]
    then
        cd $OPENSSL
        
        ./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl -d shared
        make -j4
        make install
    fi
}

check_user
get_cur_dir
build_log4cxx
