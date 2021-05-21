.RECIPEPREFIX += 

# Target
BIN_DIR = bin
BIN = $(BIN_DIR)/anaExotic

# Match all sources in src direcotry
SRCS_DIR = src
SRCS = $(wildcard $(SRCS_DIR)/**/*.cpp $(SRCS_DIR)/*.cpp) 
# Add generated dictionary
SRCS += $(SRCS_DIR)/ExoticDictionary.cpp

# Builds object list from sources, substitues .cpp for .o
OBJS_DIR = obj
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Include headers files
INCLUDE_DIRS = include $(MIDASSYS)/include 
ROOT_INCLUDE_DIRS = $(shell root-config --incdir) 
INCLUDE = $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
ROOT_INCLUDE = $(foreach includedir,$(ROOT_INCLUDE_DIRS),-I$(includedir))

# Search libraries in following directories
HEADERS = $(wildcard include/**/*.h)
INCLUDE_LIB_DIRS = $(MIDASSYS)/lib $(shell root-config --libdir) 
INCLUDE_LIB = $(foreach includedir,$(INCLUDE_LIB_DIRS),-L$(includedir))

# Set compiler, preprocesor and linker flags
CXXFLAGS += -O3
CPPFLAGS += -Wall -Wno-unused-result -Wno-deprecated-declarations -DMIDASSYS
LDFLAGS += $(INCLUDE_LIB)
LIBS = $(MIDASSYS)/lib/libmidas.a $(MIDASSYS)/lib/librootana.a
LDLIBS += $(shell root-config --libs) -lutil -lrt -lrootana -lRHTTP

# rootcling flags
CLING_DIR = bin
ROOTCLING = $(ROOTSYS)/bin/rootcling

# use DEBUG=1 to include debugging
ifdef DEBUG
   CPPFLAGS += -g  
endif

# Set other tools
MKDIR = mkdir -p

# Avoid filename conflicts
.PHONY: all clean

# Rules
all: dict $(BIN)

-include $(DEPS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
   @$(MKDIR) $(dir $@)
   $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD $(INCLUDE) $(ROOT_INCLUDE) -c $< -o $@

$(BIN): $(OBJS)
   @$(MKDIR) $(dir $@)
   $(CXX) $(LDFLAGS) $^ $(LIBS) $(LDLIBS) -o $@

clean:
   @$(RM) $(BIN) $(OBJS) $(DEPS) src/ExoticDictionary.cpp \
   src/ExoticDictionary_rdict.pcm bin/ExoticDictionary_rdict.pcm

dict:
   $(ROOTCLING) -f $(INCLUDE) src/ExoticDictionary.cpp -p \
   $(shell readlink -f include/rootana/Application.h) \
   $(shell readlink -f include/rootana/DataPointer.h) \
   $(shell readlink -f include/rootana/Cut.h) \
   $(shell readlink -f include/rootana/Globals.h) \
   include/Linkdef.h
   cp src/*.pcm $(BIN_DIR)

debug:
   @echo $(SRCS)
   @echo $(OBJS)
