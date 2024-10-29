mmult:
	gcc -g -Wall -o mmult.x code/mmult.c

rect:
	gcc -g -Wall -o rect.x code/rect.c -lm

rk3_8:
	gcc -g -Wall -o rk3_8.x code/rk3_8.c

mmult_par:
	gcc -g -Wall -O3 -o mmult_par.x code/mmult_par.c -fopenmp

rect_par:
	gcc -g -Wall -O3 -o rect_par.x code/rect_par.c -lm -fopenmp

rk3_8_par:
	gcc -g -Wall -O3 -o rk3_8_par.x code/rk3_8_par.c -fopenmp
