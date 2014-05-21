CCPLUS  = g++
CCFLAGS = -g -Wall -MMD -I. 
LD      = g++

CPPSOURCES = main.cpp sset.cpp
CPPOBJECTS = $(CPPSOURCES:%.cpp=%.o)
CPPDEPS    = $(CPPSOURCES:%.cpp=%.d)
CPPPRES    = $(CPPSOURCES:%.cpp=%.i)
CPPASS     = $(CPPSOURCES:%.cpp=%.s)

all:  sudoku

sudoku: $(CPPOBJECTS)
	$(LD) $^ $(CCFLAGS) -o $@

$(CPPOBJECTS): %.o : %.cpp
	$(CCPLUS) $(CCFLAGS) -c $< -o $@

$(CPPPRES): %.i : %.cpp
	$(CCPLUS) $(CCFLAGS) -E $< -o $@

$(CPPASS): %.s : %.cpp
	$(CCPLUS) $(CCFLAGS) -S $< -o $@

$(CPPDEPS): %.d : %.cpp
	@touch $@


clean:
	rm -f *.d *.i *.o *~ sudoku

-include $(CPPDEPS)

