BUILDPATH=build/
LIB_BUILDPATH=../build/
OBJS := $(shell find ./ -name "*.cpp" -printf "$(BUILDPATH)%f\n")
OBJS := $(OBJS:.cpp=.o)

build_all: CHK_BUILD_DIR $(LIB_BUILDPATH)typelib.a

$(LIB_BUILDPATH)typelib.a: $(OBJS)
	ar rcs $@ $^

CHK_BUILD_DIR:
	mkdir -p $(BUILDPATH)

echopath:
	echo $(OBJS)

$(BUILDPATH)%.o: %.cpp
	g++ $^ -c -o $@  `pkg-config --cflags freetype2`

clean:
	rm -f $(BUILDPATH)*.o
