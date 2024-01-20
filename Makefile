CXX=g++
CFLAGS=-w -fno-stack-protector -g -no-pie -g3 -DNO_PIE

all: aslr_examp print_mem stack_demo

aslr_examp: aslr_examp.cpp
	$(CXX) $(CFLAGS) -o aslr_examp aslr_examp.cpp

print_mem: print_mem.cpp
	$(CXX) $(CFLAGS) -o print_mem print_mem.cpp

stack_demo: overflow_demo.cpp
	$(CXX) $(CFLAGS) -o stack_demo overflow_demo.cpp -lcrypto

clean:
	rm -f aslr_examp print_mem stack_demo pass.bin gui
