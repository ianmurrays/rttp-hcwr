CXX = g++
CXXFLAGS ?= -g -Wall -W -ansi # -pedantic
LDFLAGS ?= 
SED = sed
MV = mv
RM = rm

lib = test/libUnitTest++.a
test = test/RTTPTest
rttp = build/RTTP

all: $(rttp) test

$(rttp):
	@echo Build not implemented

test:
	@echo "---> Linking $(test)..."
	@$(CXX) $(LDFLAGS) -o $(test) test/test.cpp $(lib)
	@echo "---> Running unit tests..."
	@./$(test)

clean:
	@echo "---> Cleaning test files"
	-@$(RM) $(test) 2> /dev/null
	@echo "---> Cleaning build files"
	-@$(RM) $(rttp) 2> /dev/null