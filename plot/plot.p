set term png size 600 800

set output "output_err.png"
set multiplot
plot 'output_err.txt' using ($1) with dots lt 1, 'output_err.txt' using ($2) with dots lt 2, 'output_err.txt' using ($3) with dots lt 3 , 'output_err.txt' using ($4) with dots lt 4 , 'output_err.txt' using ($5) with dots lt 5 , 'output_err.txt' using ($6) with dots lt 6 , 'output_err.txt' using ($7) with dots lt 7 , 'output_err.txt' using ($8) with dots lt 8
unset multiplot

set output "h_value.png"
set multiplot
plot 'h_value.txt' using ($1) with dots lt 1, 'h_value.txt' using ($2) with dots lt 2, 'h_value.txt' using ($3) with dots lt 3
unset multiplot

set output "w12.png"
set multiplot
plot 'w12.txt' using ($1) with dots lt 1, 'w12.txt' using ($2) with dots lt 2, 'w12.txt' using ($3) with dots lt 3 , 'w12.txt' using ($4) with dots lt 4 , 'w12.txt' using ($5) with dots lt 5 , 'w12.txt' using ($6) with dots lt 6 , 'w12.txt' using ($7) with dots lt 7 , 'w12.txt' using ($8) with dots lt 8
unset multiplot
