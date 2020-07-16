pdf: ps
	ps2pdf proj2.ps

ps:	dvi
	dvips -t article proj2.dvi

dvi:
	latex proj2
	bibtex proj2||true
	latex proj2
	latex proj2

clean:
	rm -f proj2.{ps,pdf,log,aux,out,dvi,bbl,blg}
