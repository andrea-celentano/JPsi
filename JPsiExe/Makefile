INC_DIR :=  -I.. -I$(ROOTSYS)/include/root -I$(AMPTOOLS) -I$(JPSIANA_ROOT) -I$(CLAS12PHOTONS_AMPTOOLS)/src
LIB_DIR := -L$(ROOTSYS)/lib -L$(AMPTOOLS)/lib -L$(JPSIANA_ROOT)/lib -L$(CLAS12PHOTONS_AMPTOOLS)/lib 

ROOTLIBS := $(shell root-config --glibs)

CXX := g++
#CXX_FLAGS := -O3 -m32
CXX_FLAGS := -O3 -g
LINK_FLAGS := -Wl,-u,G__cpp_setupdict

ifdef GPU

INC_DIR += -I$(CUDA_INSTALL_PATH)/include
LIB_DIR += -L$(CUDA_INSTALL_PATH)/lib

LIBS :=  -lJPsi_GPU -lAmpTools_GPU $(ROOTLIBS)  \
	-lcuda -lcudart -lstdc++
CXX_FLAGS += -DGPU_ACCELERATION

else

LIBS :=  -lJPsi -lClas12Photons_AmpTools -lAmpTools -lProofPlayer -lProof $(ROOTLIBS) -lstdc++ 

endif

SRCFILES := $(wildcard *.cc)
TARGET_EXE := $(subst .cc,_exe,$(SRCFILES))

.PHONY: default

default: $(TARGET_EXE)

# generic build rule (requires all libraries)

%_exe: %.o
	$(CXX) $(CXX_FLAGS) $(LINK_FLAGS) -o $(subst _exe,,$@) $< -O3 $(INC_DIR) $(LIB_DIR) $(LIBS) 

%.o : %.cc
	$(CXX) $(CXX_FLAGS) -M -o $*.d $< $(INC_DIR); \
	cp $*.d $*.dep; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.dep; \
	rm -f $*.d $*.d.bak $*.dep 
	$(CXX) $(CXX_FLAGS) -c -o $@ $< $(INC_DIR) 
clean:
	rm -f *.o *.dep $(subst _exe,,$(TARGET_EXE))

-include $(SRCFILES:.cc=.dep)
