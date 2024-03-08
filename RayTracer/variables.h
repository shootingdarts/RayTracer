#include <string>
#include <vector>
#include <glm/glm.hpp>

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN glm::vec3 eyeinit;
EXTERN glm::vec3 center;
EXTERN glm::vec3 upinit;
EXTERN float fovy;
EXTERN int w, h;
EXTERN int depth;
EXTERN string outputFile;
