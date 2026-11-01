#目标名:依赖文件
#	命令1
#	命令2


#定义变量
#object  := hello.o main.o
#object  += hello.o



#定义伪目标
.PHONY : clean

object := hello.o main.o

main : $(object)
	gcc $(object) -o main

hello.o : hello.c 
	gcc -c hello.c -o hello.o

main.o : main.c hello.h
	gcc -c main.c -o main.o

clean :
	rm *.o main 

