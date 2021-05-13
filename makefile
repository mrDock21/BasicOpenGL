# usage: make targetFile=filename targetFolder=folderName
srcFolder = ./include/src
objFolder = ./builds/objFiles
buildFolder = ./builds
glfwLibs = -lglfw3 -ldl -lX11 -pthread
includeFolder = ./include/

vpath %.cpp ${srcFolder}/ ./excercises/
vpath %.c ./include/
vpath %.o ${objFolder}/

all: ${buildFolder}/${targetFile}


${buildFolder}/${targetFile}: glad.o window.o Shader.o Texture.o Mesh.o element_buffer.o vertex_buffer.o input.o material.o transform.o camera.o ${targetFile}.o
	echo 'Linking obj files'
	#link all compiled objects
	g++ ${objFolder}/${targetFile}.o ${objFolder}/glad.o ${objFolder}/window.o ${objFolder}/Shader.o ${objFolder}/Texture.o ${objFolder}/Mesh.o ${objFolder}/element_buffer.o ${objFolder}/vertex_buffer.o ${objFolder}/camera.o ${objFolder}/input.o ${objFolder}/material.o ${objFolder}/transform.o -o ${buildFolder}/${targetFile} ${glfwLibs}

${targetFile}.o: ${targetFolder}/${targetFile}.cpp
	echo 'Compilling => ${targetFile}.cpp'
	# compile "main"
	g++ -Wall -I${includeFolder} -c ./excercises/${targetFolder}/${targetFile}.cpp -o ${objFolder}/${targetFile}.o

glad.o: ${includeFolder}glad/include/glad.c
	echo 'Compilling => glad.c'
	g++ -Wall -I${includeFolder} -c ${includeFolder}glad/include/glad.c -o ${objFolder}/glad.o

window.o: window.cpp
	echo 'Compilling => window.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/window.cpp -o ${objFolder}/window.o

Shader.o: Shader.cpp
	echo 'Compilling => Shader.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/Shader.cpp -o ${objFolder}/Shader.o

Texture.o: Texture.cpp
	echo 'Compilling => Texture.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/Texture.cpp -o ${objFolder}/Texture.o

Mesh.o: element_buffer.o vertex_buffer.o Texture.o material.o transform.o Mesh.cpp
	echo 'Compilling => Mesh.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/Mesh.cpp -o ${objFolder}/Mesh.o

element_buffer.o: vertex_buffer.o element_buffer.cpp
	echo 'Compilling => element_buffer.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/element_buffer.cpp -o ${objFolder}/element_buffer.o

vertex_buffer.o: vertex_buffer.cpp
	echo 'Compilling => vertex_buffer.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/vertex_buffer.cpp -o ${objFolder}/vertex_buffer.o

material.o: Shader.o shading/material.cpp
	echo 'Compilling => material.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/shading/material.cpp -o ${objFolder}/material.o

camera.o: transform.o camera.cpp
	echo 'Compilling => camera.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/camera.cpp -o ${objFolder}/camera.o

input.o: input.cpp
	echo 'Compilling => input.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/input.cpp -o ${objFolder}/input.o

transform.o: components/transform.cpp
	echo 'Compilling => transform.cpp'
	g++ -Wall -I${includeFolder} -c ${srcFolder}/components/transform.cpp -o ${objFolder}/transform.o
