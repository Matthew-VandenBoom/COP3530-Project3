.PHONY: all, debug, run

all:
	@g++ -std=c++17 "./src/cpp/*" "../src/cpp/entries.cpp" -o "./converter.exe" -I"./src/include/" -I"../src/include/" -Wall -m64

debug:
	@g++ -std=c++17 "./src/cpp/*" -o "./Build/project3debug.exe" -I"./src/include/" -g -Wall

run:
	@cls
	@g++ -std=c++17 "./src/cpp/*" -o "./Build/project3.exe" -I"./src/include/" -Wall
	@cd "./build" && project3.exe