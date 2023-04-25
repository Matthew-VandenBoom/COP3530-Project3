.PHONY: all, clean, debug

all:
	@g++ -std=c++17 "./src/cpp/*" -o "./Build/project3.exe" -I"./src/include/" 

clean:
	@del ".\Build\*" /q

debug: 