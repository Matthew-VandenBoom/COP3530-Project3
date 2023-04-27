.PHONY: all, clean, debug, run, build
FILES=


all: build
	

clean:
	@del ".\Build\*" /q

debug: 

run: build
	@cd "./build" && project3.exe

build: 
	@g++ -std=c++17 "./src/cpp/*" -o "./Build/project3.exe" -I"./src/include/" -I"./src/tpp/" -Wall