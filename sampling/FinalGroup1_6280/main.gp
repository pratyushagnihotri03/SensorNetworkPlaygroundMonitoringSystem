set terminal postscript eps enhanced color

set key out spacing 1.1
set key box
set key below width 3
set grid
set xdata time
set timefmt  "%H:%M:%S"
set format x "%H:%M:%S"

set ytics nomirror # Switch mirroring of primary system OFF 


set xlabel "HH:MM:SS"
set xlabel textcolor rgb "blue"
set ylabel textcolor rgb "red"

set size 2.0,1.0
set ylabel "Temperature [°C]"
set style line 1 lt 1 lc rgb "red" lw 2 pt 1
set output "temp.eps"
plot "temperature_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 Temperature}"


set ylabel "Light [lux]"
set style line 1 lt 1 lc rgb "blue" lw 2 pt 1
set output "light.eps"
plot "light_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 Light)}"

set ylabel "Humidity [%]"
set style line 1 lt 1 lc rgb "green" lw 2 pt 1
set output "humidity.eps"
plot "humidity_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 Humidity}"

set ylabel "Soil Moisture (kalanchoe) [VWT]"
set style line 1 lt 1 lc rgb "olive" lw 2 pt 1
set output "soil_kalanchoe.eps"
plot "soil_kalanchoe_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 Soil Moisture(Kalanchoe)}"

set ylabel "Soil Moisture(peperomia) [VWT]"
set style line 1 lt 1 lc rgb "olive" lw 2 pt 1
set output "soil_peperomia.eps"
plot "soil_peperomia_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 Soil Moisture (Peperomia)}"

set ylabel "CO2 [PPM]"
set style line 1 lt 1 lc rgb "brown" lw 2 pt 1
set output "co2.eps"
plot "co2_6280.dat" using 1:2 axes x1y1 with lines ls 1 title "{/Arial=14 CO2}"
