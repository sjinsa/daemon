TARGET = daemon
C_FLAGS = -g -Wall -fPIC -D_GNU_SOURCE -std=c++0x
CXX = g++
SRC_DIR = src/
TEST_DIR = test/

INC = -I ${SRC_DIR}/ \
      -I ${TEST_DIR}/ \

LIB = -lpthread

TEST_O = $(patsubst %.cc, %.o, $(wildcard $(TEST_DIR)/*.cc))
SRC_O = $(patsubst %.cc, %.o, $(wildcard $(SRC_DIR)*.cc))

OBJECTS = ${SRC_O} ${TEST_O}

%.o:%.cc
	@echo -e "\033[1;33m\nCompiling $< ==> $@ \033[0m..."
	$(CXX) $(INC) $(C_FLAGS) -c $< -o $@
	@echo ""

$(TARGET):$(OBJECTS)
	@echo "$(OBJECTS) ==> $@"
	$(CXX) -o $@ $^ $(LIB)
	@echo ""

clean:
	rm -rf $(TARGET) $(OBJECTS)