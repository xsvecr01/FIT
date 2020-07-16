all: run

clean:
	rm -rf ../../xdosta51_xcupak07_xsvecr01_xvasic30
	rm ../../xdosta51_xcupak07_xsvecr01_xvasic30.zip
	rm -rf __pycache__
	rm -rf documentation
	rm doxy_error.log

pack: packfolder xdosta51_xcupak07_xsvecr01_xvasic30.zip
xdosta51_xcupak07_xsvecr01_xvasic30.zip:
	cd ../.. && zip -r $@ xdosta51_xcupak07_xsvecr01_xvasic30

doc: Doxyfile
	doxygen

packfolder:
	mkdir ../../xdosta51_xcupak07_xsvecr01_xvasic30
	mkdir ../../xdosta51_xcupak07_xsvecr01_xvasic30/installer
	mkdir ../../xdosta51_xcupak07_xsvecr01_xvasic30/repo
	mkdir ../../xdosta51_xcupak07_xsvecr01_xvasic30/doc
	cp -a ../../ivs_project_2/. ../../xdosta51_xcupak07_xsvecr01_xvasic30/repo
	cp ../../ivs_project_2/dokumentace.pdf ../../xdosta51_xcupak07_xsvecr01_xvasic30/doc
	mkdir ../../xdosta51_xcupak07_xsvecr01_xvasic30/doc/documentation
	cp -a ../../ivs_project_2/src/documentation/. ../../xdosta51_xcupak07_xsvecr01_xvasic30/doc/documentation

test: test_Math_Lib.py
	python3 -m unittest test_Math_Lib

run: calc.py
	sudo add-apt-repository ppa:kivy-team/kivy && sudo apt-get update && sudo apt-get install python3-kivy && python3 calc.py