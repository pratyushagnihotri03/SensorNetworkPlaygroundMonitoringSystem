set terminal postscript eps enhanced color

set key out spacing 1.1
set key box
set key below width 3

set xdata time
set timefmt x "%H:%M:%S"

set ytics nomirror # Switch mirroring of primary system OFF 
set y2tics	# Switch secondary system ON

set xlabel "Time [min]"
set ylabel "Threshold value [ADC]"
set y2label "Threshold value [unit]"

set style line 1 lt 1 lc rgb "cyan" lw 2 pt 1
set style line 2 lt 1 lc rgb "yellow" lw 2 pt 2
set style line 3 lt 1 lc rgb "red" lw 6 pt 3
set style line 4 lt 1 lc rgb "green" lw 4 pt 4
set style line 5 lt 1 lc rgb "blue" lw 1 pt 5
set style line 6 lt 1 lc rgb "black" lw 2 pt 6

set size 2.0,1.0

set output "temperature.eps"

plot "temperature.dat" using 1:2 axes x1y1 with impulses \
	 ls 1 title "{/Arial=14 Time}"
   #  ls 1 title "{/Arial=14 Time}",\
   #  "main.dat" using 3:5 axes x1y1 with impulses \
    # ls 2 title "{/Arial=14 Light}",\
    # "main.dat" using 3:6 axes x1y1 with impulses \
    # ls 3 title "{/Arial=14 Temp}",\
    # "main.dat" using 3:7 axes x1y1 with impulses \
    # ls 4 title "{/Arial=14 Humidity}",\
    # "main.dat" using 3:8 axes x1y1 with impulses \
    # ls 5 title "{/Arial=14 CO2}",\
    # "main.dat" using 3:9 axes x1y2 with impulses \
    # ls 6 title "{/Arial=14 Moisture}"
     
