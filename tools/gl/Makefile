BUILDDIR = build
SRC = main.cpp glshader.cpp math.cpp camera.cpp
OBJ = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRC))
DEP = $(patsubst %.cpp, $(BUILDDIR)/%.d, $(SRC))
RWDIR=$(HOME)/src/librw

INC := -I/usr/local/include
LDLIBS=-L/usr/local/lib -pthread -lX11 -lXrandr -lXi -lXxf86vm -lGL -lGLEW -lm
STATICLIBS= $(RWDIR)/librw-opengl.a /usr/local/lib/libglfw3.a

#LDLIBS=-pthread -lGL -lGLEW -lglfw
#STATICLIBS=$(RWDIR)/librw-opengl.a

CFLAGS=-g -I$(RWDIR) -Wall -Wextra

rwtest: $(OBJ) $(STATICLIBS)
	$(CXX) $^ $(LDLIBS) -o $@

$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(INC) -c $< -o $@

$(BUILDDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	$(CXX) -MM -MT '$(patsubst %.cpp,$(BUILDDIR)%.o,$<)' $(CFLAGS) $(INC) $< > $@

clean:
	rm $(BUILDDIR)/* rwtest

-include $(DEP)
