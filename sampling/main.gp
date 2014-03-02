set terminal postscript eps enhanced color

set key out spacing 1.1
set key box
set key below width 3

set ytics nomirror # Switch mirroring of primary system OFF 
set y2tics	# Switch secondary system ON

set xlabel "Time [System Ticks]"
set ylabel "Threshold value [ADC]"
set y2label "Threshold value [units]"

set style line 1 lt 1 lc rgb "cyan" lw 2 pt 1
set style line 2 lt 1 lc rgb "yellow" lw 2 pt 2
set style line 3 lt 1 lc rgb "red" lw 6 pt 3
set style line 4 lt 1 lc rgb "green" lw 4 pt 4
set style line 5 lt 1 lc rgb "blue" lw 1 pt 5
set style line 6 lt 1 lc rgb "black" lw 2 pt 6

set size 1.0,0.5

set output "main.eps"

plot "main.dat" using 3:4 axes x1y1 with lines \
     ls 1 title "{/Arial=14 Time}",\
     "main.dat" using 3:5 axes x1y1 with lines \
     ls 2 title "{/Arial=14 Light}",\
     "main.dat" using 3:6 axes x1y1 with lines \
     ls 3 title "{/Arial=14 Temp}",\
     "main.dat" using 3:7 axes x1y1 with lines \
     ls 4 title "{/Arial=14 Humidity}",\
     "main.dat" using 3:8 axes x1y1 with lines \
     ls 5 title "{/Arial=14 CO2}",\
     "main.dat" using 3:9 axes x1y2 with lines \
     ls 6 title "{/Arial=14 Moisture}"
     
