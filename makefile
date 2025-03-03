BUILDPATH=build/

build_all:  $(BUILDPATH) BUILD_TYPE_LIB $(BUILDPATH)tester

debugbuild: $(BUILDPATH) BUILD_TYPE_LIB_DEBUG $(BUILDPATH)tester_debug

$(BUILDPATH)tester: $(BUILDPATH)tester.o $(BUILDPATH)typelib.a
	g++ -o $(BUILDPATH)tester $(BUILDPATH)tester.o $(BUILDPATH)typelib.a -lglut -lGL `pkg-config --libs freetype2`

$(BUILDPATH)tester_debug: $(BUILDPATH)tester_debug.o $(BUILDPATH)typelib_debug.a
	g++ -o $(BUILDPATH)tester_debug $(BUILDPATH)tester_debug.o $(BUILDPATH)typelib_debug.a -lglut -lGL `pkg-config --libs freetype2`


$(BUILDPATH)tester.o: tester/tester.cpp $(wildcard include/*.h)
	g++ -c -o $(BUILDPATH)tester.o tester/tester.cpp

$(BUILDPATH)tester_debug.o: tester/tester.cpp $(wildcard include/*.h)
	g++ -g -c -o $(BUILDPATH)tester_debug.o tester/tester.cpp

BUILD_TYPE_LIB:
	$(MAKE) build_all -C typelib

BUILD_TYPE_LIB_DEBUG:
	$(MAKE) debugbuild -C typelib
	
clean: CLEAN_TYPE_LIB
	rm -f $(BUILDPATH)*.o $(BUILDPATH)tester $(BUILDPATH)tester_debug $(BUILDPATH)*.a

echopath:
	echo $(MAKE)
	
$(BUILDPATH):
	mkdir -p $(BUILDPATH)
	
CLEAN_TYPE_LIB:
	$(MAKE) clean -C typelib
