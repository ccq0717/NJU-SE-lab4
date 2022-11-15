cc = g++
obj = main.o FileInput.o JudgeTool.o
target = main

$(target): $(obj)
	$(cc) -o $(target) $(obj)

%.o: %.cpp
	$(cc) -c $< -o $@

clean:
	rm *.o main ./output/*.csv a.out *.txt ./input/filelist.txt ./input/4A/input.txt ./input/50A/input.txt