#!/bin/tcsh -f

alias echo 'if($?prompt) echo \!*  '

echo "Set up environment for jpsi"


setenv JPSIANA_ROOT  /work/JPsi
setenv JPSIDATA   /work/JPsi/data

echo " >> jpsi ana dir is in $JPSIANA_ROOT "
if ( ! -d $JPSIDATA ) then
    echo "Error no data found"
else
    echo " >> jpsi data is in $JPSIDATA "
endif

unalias echo
