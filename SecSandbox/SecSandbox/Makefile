cc=g++
flags=-I.
libs=
headers=$(shell find . -name '*.h')
cpps=$(shell find . -name '*.cpp')
objs=$(cpps:%.cpp=%.o)

all	:	sandbox

objs	:	$(cpps) $(headers)
	$(cc) -c -o $@ $< $(flags)

sandbox:	$(objs)
	$(cc) -o $@ $^ $(flags) $(libs)


.PHONY: clean

clean:
	rm -f *.o sandbox
