set terminal postscript eps enhanced color

set key out spacing 1.1
set key box
set key below width 3

set xdata time
set timefmt x "%H:%M:%S"


set ytics nomirror # Switch mirroring of primary system OFF 
set y2tics	# Switch secondary system ON

set xlabel "Time [System Ticks]"
set ylabel "Threshold value [ADC]"
set y2label "Threshold value [units]"

set style line 1 lt 1 lc rgb "cyan" lw 2 pt 1

set size 2.0,1.0

set output "co2.eps"

plot "co2.dat" using 1:2 axes x1y1 with impulses ls 1 title "{/Arial=14 CO2}"

     
