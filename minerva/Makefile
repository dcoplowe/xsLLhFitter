CXX = g++
# Added Wextra and std=c++0x as it was in Ozgur make file. Should probably check what these do.
CXXFLAGS = -g -Wall -fPIC -Wextra -std=c++0x
ROOTFLAGS = `root-config --cflags --glibs`

RM = rm -fv

DIR_LIST = src src/base src/fileio
# src/model/

# files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))
INCS = $(foreach dir, $(DIR_LIST),-I$(dir))
INCS += -Isrc/detector -Isrc/model
# INCS =	-Isrc/ -Isrc/base/ -Isrc/detector/ -Isrc/model/ -Isrc/
INCS += -I$(PLOTUTILSROOT)/
LDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG) -lplotutils

# TMPINC = -I$(ntup)/Classes/KinematicCalculator
TMPINC = -I$(ntup)/Classes/KinematicCalculator
TMP = $(ntup)/Classes/KinematicCalculator/KinematicCalculator.cpp


MyDict.cpp: src/base/Sample.h Linkdef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^
 
libMyLib.so: MyDict.cxx src/base/Sample.cpp
	g++ -shared -o$@ $(ROOTFLAGS) $(CXXFLAGS) -I$(ROOTSYS)/include $^


SRCS = $(foreach dir, $(DIR_LIST), $(wildcard $(dir)/*.cpp) )
SRCS += MyDict.cpp

DSRCS = $(wildcard src/detector/*.cpp) $(SRCS) 
MSRCS = $(wildcard src/model/*.cpp) $(SRCS)
# SRCS =	$(wildcard src/*.cpp)
# OBJS = $(SRCS:.c=.o)

DET = MakeDetSysts.exe
DETCPP = app/app_detsysts.cpp
DETALL = $(DETCPP) $(DSRCS)
DETOBJS = $(DETALL:.cpp=.o)

MOD = MakeModSysts.exe
MODCPP = app/app_modelsysts.cpp
MODALL = $(MODCPP) $(MSRCS) $(TMP)
MODOBJS = $(MODALL:.cpp=.o)

all: $(DET) $(MOD)
	@echo  $(DET) and $(MOD) have compiled successfully.
# all: $(DET)

$(DET): $(DETOBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

$(MOD): $(MODOBJS)
	$(CXX) $(CXXFLAGS) $(INCS) $(TMPINC) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

.cpp.o:
	$(CXX) $(ROOTFLAGS) $(CXXFLAGS) $(INCS) $(TMPINC) -c $< -o $@

.PHONY: clean
clean:
	$(RM) *.o *~ $(DET) $(DETOBJS) $(MOD) $(MODOBJS)

# MyDict.cxx: $(HEADERS) Linkdef.h

# Special sample:
# Sample.o: Sample.h
# Sampledict.C: src/base/Sample.h
# 	@echo "Generating dictionary ..."
# 	@rootcint Sampledict.C -c src/base/Sample.h