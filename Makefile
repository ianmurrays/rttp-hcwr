CXX = g++
CXXFLAGS ?= -g -Wall -W -ansi # -pedantic
LDFLAGS ?= 
SED = sed
MV = mv
RM = rm

lib = test/libUnitTest++.a
test = test/RTTPTest
rttp = build/RTTP

rttp_files = rttp.cpp hcwr.cpp

all: $(rttp) test

$(rttp):
	@echo Build not implemented

test:
	@echo "---> Linking $(test)..."
	@$(CXX) -Wall -o $(test) test/test.cpp $(rttp_files) $(lib)
	@echo "---> Running unit tests..."
	@./$(test)
	
test_no_exec:
	@echo "---> Linking $(test)..."
	@$(CXX) -Wall -o $(test) -g test/test.cpp $(rttp_files) $(lib)

clean:
	@echo "---> Cleaning test files"
	-@$(RM) $(test) 2> /dev/null
	@echo "---> Cleaning build files"
	-@$(RM) $(rttp) 2> /dev/null