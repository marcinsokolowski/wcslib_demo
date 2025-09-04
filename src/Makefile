clean :
	rm -f wcslib_fits
all :
# -lwcs
	g++ wcslib_fits.cpp -lwcs -lcfitsio $(OPT) -o wcslib_fits
	g++ wcslib_test.cpp -lwcs -lcfitsio $(OPT) -o wcslib_test