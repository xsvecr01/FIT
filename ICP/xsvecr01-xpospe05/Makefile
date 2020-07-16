# Autori
# Jan Pospech [xpospe05]
# Radek Svec [xsvecr01]

# Preklad celeho projektu (parametr pro vypis varovani zrusen)
make:
	qmake -o ./src/Makefile ./src/icp.pro
	$(MAKE) -C src

# Smazani vytvorenych souboru po provedeni make a smazani generovane dokumentace
clean:
	rm -r -f ./doc/html
	rm -r -f ./doc/latex
	$(MAKE) distclean -C src
	
# Spusti vytvorenou aplikaci po provedeni make
run:
	make
	./src/icp

# Zabaleni projektu do archivu
pack:
	zip -r xsvecr01-xpospe05.zip Makefile README.txt examples src doc

# Generovani doxygen dokumentace
doxygen:
	doxygen ./doc/Doxyfile

