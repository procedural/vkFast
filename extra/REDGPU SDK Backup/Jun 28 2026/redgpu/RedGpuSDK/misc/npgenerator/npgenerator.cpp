#include <vector>
#include <string>
#include <fstream>
#include <sstream>

int main(int ArgsCount, const char ** Args) {
  if (ArgsCount <= 1) {
    printf("Error: pass filename as an argument to npgenerator.exe\n");
    exit(0);
  }

  // NOTE(Constantine):
  //
  // The rules:
  // * No const's.
  // * Pointer chars * should be sticked to the left, to the name of their parameter type.
  //
  // The format is:
  // FunctionReturnType
  // <space>
  // FunctionName
  // (
  // ParameterType until <space>
  // <space>
  // ParameterName until ,
  // ,
  // repeat the 4 rules above until )
  // )

  srand(time(0));

  std::string line;
  std::ifstream file(Args[1]);
  if (file.is_open() == false) {
    printf("Error: can't open \"%s\" file.\n", Args[1]);
  } else {
     printf("#pragma once\n");
     printf("\n");
     while (getline(file, line)) {

      std::string string = line;

      std::vector<std::string> tokens;
      std::string              s_FunctionReturnType;
      std::string              s_FunctionName;
      std::vector<std::string> s_ParameterTypes;
      std::vector<std::string> s_ParameterNames;

      std::string s = string;
      std::stringstream ss(s);
      std::string tmp;
      while (getline(ss, tmp, ' ')) {
        std::stringstream sss(tmp);
        while(getline(sss, tmp, '(')) {
          std::stringstream ssss(tmp);
          while(getline(ssss, tmp, ',')) {
            std::stringstream sssss(tmp);
            while(getline(sssss, tmp, ')')) {
              tokens.push_back(tmp);
            }
          }
        }
      }

      s_FunctionReturnType = tokens[0];
      s_FunctionName       = tokens[1];

      tokens.erase(tokens.begin() + 0); // Pop FunctionReturnType
      tokens.erase(tokens.begin() + 0); // Pop FunctionName

      for (size_t i = 0, count = tokens.size(); i < count; i += 2) {
        s_ParameterTypes.push_back(tokens[i + 0]);
        s_ParameterNames.push_back(tokens[i + 1]);
      }

      // Generate 8 random hex chars
      char randomHex[9];
      randomHex[8] = 0;
      for(int i = 0; i < 8; i += 1) {
        sprintf(randomHex + i, "%x", rand() % 16);
      }

      for (size_t i = 0, count = s_ParameterNames.size(); i < count; i += 1) {
        printf("#define _np%zu_%s %s\n", i + 1, s_FunctionName.c_str(), s_ParameterNames[i].c_str());
      }
      printf("\n");
    }
  }
}
