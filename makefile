CC = g++
INTER = python3
CFLAGS = -Wall -O2 -std=c++17

SOURCES = orbit.cpp
PYSCRIPT = plot.py
EXECUTABLE = orbit

DATA_DIR = data/files
DATA_DIR_PY = ./data/files

# Use wildcard to find all .txt files in DATA_DIR
DATA_FILES := $(wildcard $(DATA_DIR)/*.txt)

all: run_cpp run_python

run_cpp: $(EXECUTABLE)
	./$(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS)

run_python: wait_for_data_generation
	$(INTER) $(PYSCRIPT) $(DATA_DIR_PY)

wait_for_data_generation: $(DATA_FILES)
	@echo "Waiting for data generation..."
	@while [ -z "$(DATA_FILES)" ]; do sleep 1; DATA_FILES := $(wildcard $(DATA_DIR)/*.txt); done
	@echo "Data files generated."

clean:
	rm -f $(EXECUTABLE)
