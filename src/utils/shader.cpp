#include "shader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "utils.h"

unsigned long getFileLength(std::ifstream& file) {
  if (!file.good()) {
    return 0;
  }

  file.seekg(0, std::ios::end);
  unsigned long len = file.tellg();
  file.seekg(std::ios::beg);
  return len;
}

int loadShader(char* filename, GLchar** ShaderSource, GLint* len) {
  std::ifstream file;
  file.open(filename, std::ios::in);  // opens as ASCII!

  if (!fileExists(filename)) {
    return -1;
  }

  GLint fileLength = getFileLength(file);
  *len = fileLength;

  if (*len == 0)
    return -2;  // Error: Empty File

  char* tShaderSource = (GLchar*)new char[*len + 1]();

  // can't reserve memory
  if (tShaderSource == nullptr) {
    return -3;
  }

  // len isn't always strlen cause some characters are stripped in ascii read...
  // it is important to 0-terminate the real length later, len is just max
  // possible value...
  tShaderSource[*len] = '\0';

  unsigned int i = 0;
  while (file.good()) {
    tShaderSource[i] = file.get();  // get character from file.
    if (!file.eof()) {
      i++;
    }
  }
  tShaderSource[i] = '\0';  // 0-terminate it at the correct position

  file.close();

  *ShaderSource = tShaderSource;

  return 0;  // No Error
}

void setupShader(const std::string& shaderFile, GLuint newShader) {
  // Load them!
  GLchar* shaderSource;
  GLint shaderFileLength;

  // Convert from string to char*
  char* charShaderFile = const_cast<char*>(shaderFile.c_str());

  if (loadShader(charShaderFile, &shaderSource, &shaderFileLength) != 0) {
    abort_on_error((shaderFile + " NOT found!").c_str());
  } else {
    std::cout << shaderFile << " found\n";
  }

  glShaderSource(newShader, 1, const_cast<const GLcharARB**>(&shaderSource),
                 &shaderFileLength);

  // Compile them
  glCompileShaderARB(newShader);

  // Check success
  GLint compiled;
  glGetShaderiv(newShader, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
    abort_on_error(("Dude, " + shaderFile + " didnt compile...").c_str());
  else
    std::cout << shaderFile << " shader compiled!\n";
}

// Setup program
void setupProgram(GLuint newProgram) {
  glLinkProgram(newProgram);

  GLint linked;
  glGetProgramiv(newProgram, GL_LINK_STATUS, &linked);

  if (!linked) {
    abort_on_error(
        ("Program " + convertIntToString(newProgram) + "didnt link...")
            .c_str());
  } else {
    std::cout << "Program " << newProgram << " linked!\n\n";
  }
}