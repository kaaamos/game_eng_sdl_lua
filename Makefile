build:
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o game \
	-I"./lib/lua" \
	-IC:\mingw_dev_lib\include \
	-L"./lib/lua" \
	-LC:\mingw_dev_lib\lib \
	-llua53 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \

clean:
	del /Q /F game.exe