EXE=FbxParser

CLEAN=rm -f $(EXE) *.o *.a

all: $(EXE)

FbxParser.o: FbxParser.cpp FolderCreator.h
#	g++ -c -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include  -L/Applications/Autodesk/FBX\ SDK/2020.0.1/lib/clang/release/ -lfbxsdk -std=c++17 $<
	g++ -c -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include -std=c++11 $<


Animation.o: Animation.cpp FolderCreator.h
#	g++ -c -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include  -L/Applications/Autodesk/FBX\ SDK/2020.0.1/lib/clang/release/ -lfbxsdk -std=c++17 $<
	g++ -c -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include -std=c++11 $<

MeshExporter.o: MeshExporter.cpp
	g++ -c -std=c++11 $<

FolderCreator.o: FolderCreator.cpp
	g++ -c -std=c++11 $<


FbxParser: FbxParser.o MeshExporter.o Animation.o FolderCreator.o
	g++ -I/Applications/Autodesk/FBX\ SDK/2020.0.1/include  -L/Applications/Autodesk/FBX\ SDK/2020.0.1/lib/clang/release/ -lfbxsdk -o $@ $^

clean:
	$(CLEAN)

