all:
	@echo 'making...'
	pdflatex Adams_Report.tex
	bibtex Adams_Report
	pdflatex Adams_Report.tex
	pdflatex Adams_Report.tex
clean:
	rm -f *~
	rm -f *.aux
	rm -f *.log
	# rm -f *.pdf
	rm -f *.out
