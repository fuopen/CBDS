#######################################################
# this is automatically generate the executive file
#######################################################
Object_cpp=main.o call.o psmc.o update.o cor.o mean_sd.o quantile.o coalescent.o file.o list.o tree.o stack.o node.o gen_func.o fast_median.o random.o
psmc_opt6:$(Object_cpp)
	g++ -o cbds $(Object_cpp)
main.o:main.cpp call.h
	g++ -c main.cpp
call.o:call.cpp call.h macro.h update.h psmc.h
	g++ -c call.cpp
psmc.o:psmc.cpp psmc.h macro.h cor.h
	g++ -c psmc.cpp
update.o:update.cpp update.h macro.h gen_func.h file.h tree.h list.h
	g++ -c update.cpp
cor.o:cor.cpp cor.h mean_sd.h quantile.h fast_median.h random.h file.h list.h gen_func.h macro.h
	g++ -c cor.cpp
random.o:random.cpp random.h
	g++ -c random.cpp
mean_sd.o:mean_sd.cpp mean_sd.h fast_median.h
	g++ -c mean_sd.cpp
quantile.o:quantile.cpp quantile.h fast_median.h
	g++ -c quantile.cpp
coalescent.o:coalescent.cpp coalescent.h
	g++ -c coalescent.cpp
file.o:file.cpp file.h macro.h gen_func.h
	g++ -c file.cpp
list.o:list.cpp list.h macro.h node.h tree.h gen_func.h
	g++ -c list.cpp
tree.o:tree.cpp tree.h node.h stack.h gen_func.h
	g++ -c tree.cpp
stack.o:stack.cpp stack.h macro.h node.h
	g++ -c stack.cpp
node.o:node.cpp node.h macro.h
	g++ -c node.cpp
gen_func.o:gen_func.cpp gen_func.h macro.h
	g++ -c gen_func.cpp
.PHONY:clean
clean:
	-rm $(Object_cpp)
