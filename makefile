BUILDPATH=build/

build_all:  $(BUILDPATH) BUILD_TYPE_LIB $(BUILDPATH)tester

$(BUILDPATH)tester: $(BUILDPATH)tester.o $(BUILDPATH)typelib.a
	g++ -o $(BUILDPATH)tester $(BUILDPATH)tester.o $(BUILDPATH)typelib.a -lglut -lGL `pkg-config --libs freetype2`

$(BUILDPATH)tester.o: tester/tester.cpp $(wildcard include/*.h)
	g++ -c -o $(BUILDPATH)tester.o tester/tester.cpp

BUILD_TYPE_LIB:
	$(MAKE) -C typelib
	
clean: CLEAN_TYPE_LIB
	rm -f $(BUILDPATH)*.o $(BUILDPATH)tester $(BUILDPATH)*.a

echopath:
	echo $(MAKE)
	
$(BUILDPATH):
	mkdir -p $(BUILDPATH)
	
CLEAN_TYPE_LIB:
	$(MAKE) clean -C typelib
