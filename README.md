# Transport-stream-parser
This repository is an MPEG Transport Stream parser, enabling the analysis, interpretation, and processing of video and audio streams. Developed in C++ along with CMake.

## How to run
To run this project, you will need to build it first using [CMake](https://cmake.org/).
Preferably use VScode with CMake tools.

1. Clone the repository to your local machine.<br>
```
git clone https://github.com/buennoo/Transport-stream-parser.git
```

2. Navigate to the root of the project directory and create build folder.
```
cd Transport-stream-parser
mkdir build
cd build
```

3. Configure and build the project:
```
cmake ..
make
```

4. Run the project:
```
./TS-PARSER.exe
```

5. Once the build is complete and you've launched it, you should see the results in vscode terminal.

Or if you are using CMake tools Extension in VScode, you can create a build directory and launch the application manually by clicking the arrow next to it:<br>
![obraz](https://github.com/buennoo/Transport-stream-parser/assets/127790828/995a1dfa-86c5-4262-8cd6-180601506445)


