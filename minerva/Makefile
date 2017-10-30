CXX = g++
# Added Wextra and std=c++0x as it was in Ozgur make file. Should probably check what these do.
CXXFLAGS = -g -Wall -fPIC -Wextra -std=c++0x
ROOTFLAGS = `root-config --cflags --glibs`

RM = rm -fv

# OPTS=-D __REWEIGHT__

DIR_LIST = src src/base src/fileio src/parser src/tools
# src/model/

# files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))
INCS = $(foreach dir, $(DIR_LIST),-I$(dir))
INCS += -Isrc/detector -Isrc/model
INCS += -I$(PLOTUTILSROOT)/
LDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG) -lplotutils

# TMPINC = -I$(ntup)/Classes/KinematicCalculator
TMP_LIST = KinematicCalculator TransverseTools
TMPINC = $(foreach dir, $(TMP_LIST),-I$(ntup)/Classes/$(dir))
TMP = $(foreach dir, $(TMP_LIST), $(ntup)/Classes/$(dir)/$(dir).cpp )
# $(ntup)/Classes/KinematicCalculator/KinematicCalculator.cpp

# Sample.cpp:
# 	rootcint -f Sampledict.cpp -c src/base/Sample.h src/base/ErrorType.h $(PLOTUTILSROOT) LinkDef.h
 
# libSample.so: SampleDict.cpp src/base/Sample.cpp
# 	$(CXX) -shared $(CXXFLAGS) $(INCS) -o$@ $^ $(ROOTFLAGS) $(LDLIBS) 

SRCS = $(foreach dir, $(DIR_LIST), $(wildcard $(dir)/*.cpp) )
# SRCS += MyDict.cpp

DSRCS = $(wildcard src/detector/*.cpp) $(SRCS) 
MSRCS = $(wildcard src/model/*.cpp) $(SRCS)

DET = MakeDetSysts.exe
DETCPP = app/app_detsysts.cpp
DETALL = $(DETCPP) $(DSRCS) $(TMP)
DETOBJS = $(DETALL:.cpp=.o)

MOD = MakeModSysts.exe
MODCPP = app/app_modelsysts.cpp
MODALL = $(MODCPP) $(MSRCS) $(TMP)
MODOBJS = $(MODALL:.cpp=.o)

all: $(DET) $(MOD)
	@echo  $(DET) and $(MOD) have compiled successfully.
# all: $(DET)

$(DET): $(DETOBJS)
	$(CXX) $(OPTS) $(CXXFLAGS) $(INCS) $(TMPINC) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

$(MOD): $(MODOBJS)
	$(CXX) $(OPTS) $(CXXFLAGS) $(INCS) $(TMPINC) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

.cpp.o:
	$(CXX) $(OPTS) $(ROOTFLAGS) $(CXXFLAGS) $(INCS) $(TMPINC) -c $< -o $@

.PHONY: clean
clean:
	$(RM) *.o *~ $(DET) $(DETOBJS) $(MOD) $(MODOBJS)