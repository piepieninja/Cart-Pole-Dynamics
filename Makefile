all:
	@echo "USAGE: make mac "
	@echo "USAGE: make linux"
	@echo "USAGE: make clean"
linux:
	@echo "TODO"
mac:
	g++ -w src/sim_pendulum.cpp -o bin/sim_pendulum.x -L/System/Library/Frameworks -framework GLUT -framework OpenGL
	g++ -w src/sim_cartpole.cpp -o bin/sim_cartpole.x -L/System/Library/Frameworks -framework GLUT -framework OpenGL
	# g++ -w src/comitride.cpp -o bin/comit.x -L/System/Library/Frameworks -framework GLUT -framework OpenGL
	# g++ -w src/balls.cpp -o bin/balls.x -L/System/Library/Frameworks -framework GLUT -framework OpenGL
clean:
	rm -f bin/*.x
	rm -f src/*~
	rm -f *~
