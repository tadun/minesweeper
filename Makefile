# Author:      Tomáš David
# Modified by: Tadeáš Horn

# Config =======================================================================
# Compilation settings ---------------------------------------------------------

# general compilation settings
CXX      := g++
CXXFLAGS := -Winvalid-pch -Wall -Wextra `wx-config --cxxflags` -std=c++11
LDFLAGS  := `wx-config --libs`
LDLIBS   :=
debug: CXXFLAGS += -g -DDEBUG
debug: LDFLAGS  += # -Wl,-subsystem,console

# Project settings -------------------------------------------------------------

# print prefix
PRINT :=

# files and folders names
BUILD_PATH                := ./build
BINARY_NAME               := Minesweeper
SOURCES_PATHS             := ./src
HEADERS_PATHS             := ./src
PRECOMPILED_HEADERS_PATHS := ./src/prec

# you can add as many include paths as you want
INCLUDE_PATHS := # ./wxW_3-0-5/lib_dev/gcc810_x64_dll/mswu
INCLUDE_PATHS += # ./wxW_3-0-5/lib/wx/include/osx_cocoa-unicode-3.0
INCLUDE_PATHS += # ./wxW_3-0-5/include

# only works with one path
DLL_PATH := # ./wxW_3-0-5/lib
DLL_LIST := # wx_osx_cocoau_core-3.0
DLL_LIST += # wxmsw30u_core_gcc810_x64


# End of configuration part ====================================================

# final binary path
TARGET   := $(BUILD_PATH)/$(BINARY_NAME)
# generate list of headerfiles in project
DEPS 	 := $(foreach path, $(HEADERS_PATHS), $(wildcard $(path)/*.hpp))
# source and object files
SOURCES  := $(foreach path, $(SOURCES_PATHS), $(wildcard $(path)/*.cpp))
OBJS     := $(patsubst %.cpp, %.o, $(SOURCES))
# headers for precompilation
PCHLIST  := $(foreach path, $(PRECOMPILED_HEADERS_PATHS), $(wildcard $(path)/*.hpp))
GCHLIST  :=  $(patsubst %.hpp, %.hpp.gch, $(PCHLIST))
# where should be DLLs copied to
DLL_DEST := $(addsuffix .dll, $(addprefix $(BUILD_PATH)/, $(DLL_LIST)))

# wxWidgets settings
CXXFLAGS += $(addprefix -I, $(INCLUDE_PATHS))
LDFLAGS  += $(addprefix -L, $(DLL_PATH))
LDLIBS   += $(addprefix -l, $(DLL_LIST))

.PHONY: all debug clean cleanall debugmake

# Compile binary + copy DLLs
all: $(TARGET) $(DLL_DEST)

debug: all

debugmake:
	@echo $(DEPS)
	@echo $(SOURCES)
	@echo $(PCHLIST)

# Link object files
$(TARGET): $(OBJS)
	@echo $(PRINT)Linking... & echo flags: $(LDFLAGS) $(LDLIBS)
	@$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
	@echo \
################################ BUILD DONE ################################\
& echo Please wait for this command to exit.

# Compiling object files
$(OBJS): %.o: %.cpp $(DEPS) $(GCHLIST)
	@echo $(PRINT)$(CXX) -c $< flags: $(CXXFLAGS)
	@$(CXX) $< -c -o $@ $(CXXFLAGS)

# Compiling precompiled headers
$(GCHLIST): %.hpp.gch: %.hpp
	@echo $(PRINT)$(CXX) -c $< flags: $(CXXFLAGS)
	@$(CXX) $< -c -o $@ $(CXXFLAGS)

# Copying DLLs
$(DLL_DEST) : $(BUILD_PATH)/% : $(DLL_PATH)/%
	@echo $(PRINT)copying $($^) to $($@)...
	@copy $($^) $($@)

# list of files to be removed
# fixes pathnames to Windows style (required for CMD)
TOCLEAN :=

# clean:    TOCLEAN += $(subst /,\,$(strip $(TARGET)))
# clean:    TOCLEAN += $(subst /,\,$(strip $(OBJS)))
# clean:    TOCLEAN += $(subst /,\,$(strip $(GCHLIST)))
# cleanall: TOCLEAN += $(subst /,\,$(strip $(DLL_DEST)))

clean:    TOCLEAN += $(TARGET)
clean:    TOCLEAN += $(OBJS)
clean:    TOCLEAN += $(GCHLIST)
cleanall: TOCLEAN += $(DLL_DEST)

cleanall: clean

clean:
	rm -f $(TOCLEAN)
#	@for %%F in ($(TOCLEAN)) do ( echo $(PRINT)del %%F & del %%F )