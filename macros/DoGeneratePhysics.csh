#!/bin/tcsh -f


#set DATADIR=$ETAPIWORKDIR_FARM/data/signal_original
set DATADIR=$JPSIDATA
set NEVENTS=3000000 #3E6 PS events/file


set GENERATOR = $JPSIANA_ROOT/macros/GenerateOne.csh


#set QUEUE = aiace
set QUEUE = long

#100 files from 0 to 100 
foreach x (`seq 0 1 19`) 
      rm $DATADIR/gen_$x.root
      bsub -q $QUEUE -P sl5_64 -o $DATADIR/$x.out.log -e $DATADIR/$x.err.log -R "rusage[mem=6000,swp=5000]" $GENERATOR $x $NEVENTS
end
