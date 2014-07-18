* To compile the program and produce the executables

> Installing Dependancies:

```
sudo apt-get install freeglut3-dev libxmu-dev libxi-dev libsoil-dev cmake
```

> Compiling:

```
mkdir build && cd build
cmake ..
make (-j #cores if available)
cd ../bin
```
> Running:

Without Graphics - Console Version

```
./Robsim
```

With Graphics

```
./RobsimGui
```

* There is a generated doxy for the code documentation, just run the script doxy.sh. 