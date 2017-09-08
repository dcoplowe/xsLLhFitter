CXX = g++
# Added Wextra and std=c++0x as it was in Ozgur make file. Should probably check what these do.
CXXFLAGS = -g -Wall -fPIC -Wextra -std=c++0x
ROOTFLAGS = `root-config --cflags --glibs`

INCS =	-Isrc/ -I$(PLOTUTILSROOT)/
LDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG)

SRCS =	$(wildcard src/*.cpp)
# OBJS = $(SRCS:.c=.o)

DET = MakeDetSysts.exe
DETCPP = app_detsysts.cpp
DETALL = $(DETCPP) $(SRCS)
DETOBJS = $(ALLDET:.cpp=.o)

MOD = MakeModSysts.exe
MODCPP = app_modelsysts.cpp
MODALL = $(MODCPP) $(OBJS)
MODOBJS = $(MODALL:.c=.o)

all: $(DET) $(MOD)
	@echo  $(DET) has compiled successfully.
	@echo  $(MOD) has compiled successfully.

$(DET): $(DETOBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

$(MOD): $(MODOBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(ROOTFLAGS) $(LDLIBS)

.cpp.o:
	$(CXX) $(ROOTFLAGS) $(CXXFLAGS) $(INCS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) *.o *~ $(DET) 

