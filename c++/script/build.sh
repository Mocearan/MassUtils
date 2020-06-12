#! /bin/bash

set +x

CUR_DIR=
SOURCE_DIR=`pwd`/..
INSTALL_DIR=`pwd`/../run
BUILD_DIR=${BUILD_DIR:-../build}
BUILD_TYPE=${BUILD_TYPE:-Debug}
CXX=${CXX:-g++}
MAJOR=`head -1 version`
MINOR=`head -2 version | tail -1`
PATCH=`head -3 version | tail -1`
VERSION=0.0.0

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

# clean all
clean(){
    rm -rf ${BUILD_DIR}
    rm -rf ${INSTALL_DIR}
}

# print build usage help
print_help(){
    echo "Usage: "
	echo "  $0 clean --- clean all build"
	echo "  $0 version [major | minor] --- update version"
    echo "  $0 build [Debug | Release] --- build a debug or release verseion"
}

dependency_install(){
    echo "install dependency..."
    sleep 1
    yum -y install cmake
    yum -y install libuuid-devel
    yum -y install openssl-devel
    yum -y install curl-devel
    echo "dependency install complete."
    sleep 1
}



make_build_dir(){
    mkdir -p ${BUILD_DIR}/${BUILD_TYPE}/${BUILD_TYPE}-${VERSION}
}


build(){
	PATCH=$((${PATCH}+1))
    VERSION=${MAJOR}.${MINOR}.${PATCH}
	make_build_dir
    dependency_install

	cd ${BUILD_DIR}/${BUILD_TYPE}/${BUILD_TYPE}-${VERSION}

	cmake \
            -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
            -DCMAKE_INSTALL_PERFIX=${INSTALL_DIR} \
            -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
            -DVERSION_MAJOR=${MAJOR}\
            -DVERSION_MINOR=${MINOR}\
            -DVERSION_PATCH=${PATCH}\
            ${SOURCE_DIR}



    make
    make install

    sed -i "3c ${PATCH}" ${CUR_DIR}/version
}

# flow
check_user
get_cur_dir
case $1 in
    clean)
        echo "clean all build..."
        clean
        ;;
    version)
        if [ $# != 2 ]; then
            echo $#
            print_help
            exit
        fi

        if [ $2 = "major" ];then
            MAJOR=$((${MAJOR}+1))
            sed -i "1c ${MAJOR}" ${CUR_DIR}/version

            MINOR=0
            sed -i "2c ${MINOR}" ${CUR_DIR}/version
            
            PATCH=0
            sed -i "3c ${PATCH}" ${CUR_DIR}/version
            
            VERSION=${MAJOR}.${MINOR}.${PATCH}
            echo "update version to "${VERSION}
        elif [ $2 = "minor" ];then
            MINOR=$((${MINOR}+1))
            sed -i "2c ${MINOR}" ${CUR_DIR}/version
            
            PATCH=0
            sed -i "3c ${PATCH}" ${CUR_DIR}/version

            VERSION=${MAJOR}.${MINOR}.${PATCH}
            echo "update version to "${VERSION}
        else
            echo $#
            print_help
            exit
        fi

        echo "use "$0$1$2" build new version :"${MAJOR}.${MINOR}.${PATCH}
        build $2
        ;;
    build)
        if [ $# != 2 ]; then
            echo $#
            print_help
            exit
        fi

        if [[ $2 = "Release" ]] || [[ $2 = "Debug" ]];then
            BUILD_TYPE=$2
        else
            echo "chose [Debug] or [Release] you want to build."
            exit
        fi

        echo "build..."
        build
        ;;
    *)
        print_help
        ;;
esac
