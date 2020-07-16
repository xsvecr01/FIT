pdf: ps
	ps2pdf proj1.ps

ps:	dvi
	dvips -t article proj1.dvi

dvi:
	latex proj1
	bibtex proj1||true
	latex proj1
	latex proj1

clean:
	rm -f proj1.{ps,pdf,log,aux,out,dvi,bbl,blg}
