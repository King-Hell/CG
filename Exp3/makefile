prom = main
include=../include
flags=-lglfw3 -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
src = $(shell find ./ -name "*.c" -or -name "*.cpp")

$(prom): $(src)
	g++ -g -o $(prom) $(src) -I $(include) $(flags)

clean:
	rm -rf $(prom)
