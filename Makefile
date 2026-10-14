# Makefile del simulador de Maquina de Turing multicinta determinista.
# Compilacion out-of-source: los artefactos (.o, .d) van a build/ y el binario
# final (mt_sim) se deposita en la raiz del proyecto.

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Iinclude
DEPFLAGS := -MMD -MP

SRC_DIR   := src
BUILD_DIR := build
BIN       := mt_sim

SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR) $(BIN)
