1. Compiling the codes:
	all files needed for compiling are included in the archive;
	just type in "make" or "make waveguide";
	for cleaning up type in "make clean".

2. Calling the programm:
	the programm is called via "./lbm para.dat [bonus]";
	where "para.dat" indicates the file name of configure file;
	"[bonus]" activates bonus mode and can be dismissed if not necessary;

3. Input:
	three input files are included in the archive submitted:
	"params_normal.dat" generates a 5*5 grid for the normal task, type "./lbm paras_normal.dat" to apply it;
	"params_bonus.dat" activates the bonus mode, type "./lbm paras_bonus.dat bonus" to apply it;
        "test.pgm" is a .pgm file prepared for the bonus task;

4. Output:
	a folder of .vtk files are to be generated if the visualization function is activated;
	the time used is output to the screen;
	the MLUps is also shown on the screen if the visualizaion function is disabled;
