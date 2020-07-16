proj5.pdf: proj5.ps
	ps2pdf proj5.ps

proj5.ps: proj5.dvi
	dvips proj5.dvi

proj5.dvi: proj5.tex
	latex proj5.tex
	latex proj5.tex
  
pdflatex: proj5.tex
	pdflatex proj5.tex
	pdflatex proj5.tex
  
clean:
	rm -f proj5.aux proj5.pdf proj5.ps proj5.dvi proj5.log proj5.nav proj5.out proj5.snm proj5.toc proj5.vrb
