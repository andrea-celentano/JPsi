#!/bin/tcsh -f

#$1 is the "x" incrementing number (also the seed)
#$2 is the number of events

set DATADIR=$JPSIDATA
#set DATADIR=$ETAPIWORKDIR_FARM/data/signal_original
set RUNDIR=$JPSIANA_ROOT/run
set EXE1=$JPSIANA_ROOT/JPsiExe/generatePS
set EXE2=$JPSIANA_ROOT/JPsiExe/generatePhysics


set DATACONFIG=$RUNDIR/MC.cfg

#set OUTFILE=$DATADIR/PseudoData_$1.root
#set OUTFILE=$DATADIR/gen_$1.root
set OUTFILE=$DATADIR/gen_$1.lund

cd $RUNDIR

echo "Doing Phase Space"
set PSFILE=$DATADIR/PS_$1.root
$EXE1 -N $2 -s $1 -o $PSFILE 
echo "Done Phase Space"

echo "Do Physics"
$EXE2 -c $DATACONFIG -i $PSFILE -o $OUTFILE -s $1
echo "Done Physics"

echo "Removing PhaseSpace File: $PSFILE"
rm $PSFILE
echo "Done"
