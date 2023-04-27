.PHONY: all, clean, debug, run, build
FILES=


all: build
	

clean:
	@del ".\Build\*" /q

debug: 

run: build
	@cd "./build" && project3.exe

build: 
	@g++ -std=c++2a "./src/cpp/*" -o "./Build/project3.exe" -I"./src/include/" -I"./src/tpp/" -I"D:/Libraries/SFML-2.5.1/include/" -DSFML_STATIC \
	-L"D:/Libraries/SFML-2.5.1/lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -Wall 