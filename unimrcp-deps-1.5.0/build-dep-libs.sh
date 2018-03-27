#! /bin/sh

############################################################
#
# This script builds and installs the UniMRCP dependencies.
#
# Usage: ./build-dep-libs.sh [OPTIONS]
#
# Author: Arsen Chaloyan achaloyan@gmail.com
#
############################################################

# Root directory.
ROOTDIR=`pwd`

# Source directories.
APR_SRCDIR=${ROOTDIR}/libs/apr
APU_SRCDIR=${ROOTDIR}/libs/apr-util
SOFIA_SRCDIR=${ROOTDIR}/libs/sofia-sip

# Default installation directories.
APR_INSTALLDIR=/usr/local/apr
SOFIA_INSTALLDIR=/usr/local

# Install or just build.
MAKEINSTALL="yes"

# Default operation mode (interactive or silent)
OPERATIONMODE="interactive"

# Build APR library.
build_apr()
{
    echo
    echo ======================================================================
    echo Building APR library
    cd ${APR_SRCDIR}
    ./configure --prefix=${APR_INSTALLDIR}
    make
    if [ $MAKEINSTALL = "yes" ] ; then
        sudo make install
    fi
    cd ${ROOTDIR}
}

# Build APR-Util library.
build_apu()
{
    echo
    echo ======================================================================
    echo Building APR-Util library
    cd ${APU_SRCDIR}
    ./configure --with-apr=${APR_SRCDIR} --prefix=${APR_INSTALLDIR}
    make
    if [ $MAKEINSTALL = "yes" ] ; then
        sudo make install
    fi
    cd ${ROOTDIR}
}

# Build Sofia-SIP library.
build_sofia()
{
    echo
    echo ======================================================================
    echo Building Sofia-SIP library
    cd ${SOFIA_SRCDIR}
    ./configure --with-glib=no --without-doxygen --prefix=${SOFIA_INSTALLDIR}
    make
    if [ $MAKEINSTALL = "yes" ] ; then
        sudo make install
    fi
    cd ${ROOTDIR}
}

# Get answer (y/n).
get_answer()
{
    echo
    while echo "$* (y/n)? "
    do
    read yn
    case $yn in
    [yY]) return 0 ;;
    [nN]) return 1 ;;
    *) echo please answer y or n ;;
    esac
    done
}

# Output usage information.
print_usage()
{
    echo
    echo "Usage: ./build-dep-libs.sh [OPTIONS]"
    echo
    echo "where OPTIONS are:"
    echo "   -h --help...................: print the usage"
    echo "   -s --silent.................: set the operation mode to silent"
    echo "   -n --no-install.............: build but do not install the libraries"
    echo "   -a --apr-install-dir DIR ...: set the installation directory for APR and APR-Util"
    echo "   -o --sofia-install-dir DIR..: set the installation directory for Sofia-SIP"
    echo
}

# Options followed by a colon indicate they have a required argument.
if ! options=$(getopt -u -o hsna:o: -l help,silent,no-install,apr-install-dir:,sofia-install-dir: -- "$@"); then
    exit 1
fi

set -- $options

while [ $# -gt 0 ]
do
    case $1 in
        -h|--help) print_usage; exit;;
        -s|--silent) OPERATIONMODE="silent" ;;
        -n|--no-install) MAKEINSTALL="no" ;;
        # For options with required arguments, an additional shift is needed.
        -a|--apr-install-dir) APR_INSTALLDIR="$2" ; shift;;
        -o|--sofia-install-dir) SOFIA_INSTALLDIR="$2" ; shift;;
        (--) shift; break;;
        (-*) echo "$0: invalid option $1" 1>&2; exit 1;;
        (*) break;;
    esac
    shift
done

echo
echo ==================== UniMRCP Dependencies Builder ====================
echo
echo Install libraries............... : $MAKEINSTALL
echo APR installation directory...... : $APR_INSTALLDIR
echo Sofia-SIP installation directory : $SOFIA_INSTALLDIR
echo
echo Operation mode.................. : $OPERATIONMODE
echo
echo ======================================================================

if [ $OPERATIONMODE != "interactive" ] || get_answer "Do you want to build APR libraries"; then
    build_apr
    build_apu
fi

if [ $OPERATIONMODE != "interactive" ] || get_answer "Do you want to build Sofia-SIP library"; then
    build_sofia
fi
