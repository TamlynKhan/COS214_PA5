CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g
TARGET   = campusguard

SRCS = main.cpp \
       Alarm.cpp \
       BlueSecurityAlarm.cpp \
       Building.cpp \
       BuildingState.cpp \
       DispatchCentre.cpp \
       DispatchStrategy.cpp \
       Incident.cpp \
       IncidentMediator.cpp \
       OperatorCommand.cpp \
       OperatorConsole.cpp \
       Person.cpp \
       ResponseUnit.cpp \
       UnitType.cpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

.PHONY: all run valgrind gdb clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(TARGET)

gdb: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
