#include "model_loader.h"

model_loader::model_loader()
{
  //ctor
}

model_loader::~model_loader()
{
  //dtor
}

bool model_loader::load_model(  const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals ){

  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;


  FILE * file = fopen(path, "r");
  if( file == NULL ){
      printf("Impossible to open the file !\n");
      return false;
  }

  while( 1 ){
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    if ( strcmp( lineHeader, "v" ) == 0 ){
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
      temp_vertices.push_back(vertex);
    }
    else if ( strcmp( lineHeader, "vt" ) == 0 ){
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y );
      temp_uvs.push_back(uv);
    }
    else if ( strcmp( lineHeader, "vn" ) == 0 ){
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      temp_normals.push_back(normal);
    }
    else if ( strcmp( lineHeader, "f" ) == 0 ){
      // temp storage for face info
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

      // Method 1 (vertex, uv, normal)
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
      if (matches == 9){
          //std::cout << path << " read with vertex texture and normal.\n";

          vertexIndices.push_back(vertexIndex[0]);
          vertexIndices.push_back(vertexIndex[1]);
          vertexIndices.push_back(vertexIndex[2]);
          uvIndices    .push_back(uvIndex[0]);
          uvIndices    .push_back(uvIndex[1]);
          uvIndices    .push_back(uvIndex[2]);
          normalIndices.push_back(normalIndex[0]);
          normalIndices.push_back(normalIndex[1]);
          normalIndices.push_back(normalIndex[2]);
      }
      // Method 2 (vertex, normal)
      else{
        matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
        if (matches == 6){
            //std::cout << path << " read with vertex and normal.\n";
        }
        // Method 3 (vertex and uv)
        else{
          matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
          if (matches == 6){
             // std::cout << path << " read with vertex and texture.\n";
          }
          // Method 4 (vertex)
          else{
            matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
            if (matches == 3){
                //std::cout << path << " read with vertex.\n";
            }
            // None :(
            else{
              printf("File can't be read by our simple parser : ( Try exporting with other options\n");
              return false;
            }
          }
        }
      }
    }
  }

  // For each vertex of each triangle
  for( unsigned int i=0; i<vertexIndices.size(); i++ ){
    unsigned int vertexIndex = vertexIndices[i];
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    out_vertices.push_back(vertex);

    //std::cout << vertexIndex << " " << temp_vertices[ vertexIndex-1 ].x << " " << temp_vertices[ vertexIndex-1 ].y << " " << temp_vertices[ vertexIndex-1 ].z << "\n";
  }

  // For each uv of each triangle
  for( unsigned int i = 0; i < uvIndices.size(); i++ ){
    unsigned int uvIndex = uvIndices[i];
    glm::vec2 uv = temp_uvs[ uvIndex-1 ];
    out_uvs.push_back(uv);

    // std::cout << uvIndex << " " << temp_uvs[ uvIndex-1 ].x << " " << temp_uvs[ uvIndex-1 ].y << "\n";
  }

  // For each normal of each triangle
  for( unsigned int i=0; i<normalIndices.size(); i++ ){
    unsigned int normalIndex = normalIndices[i];
    glm::vec3 normal = temp_normals[ normalIndex-1 ];
    out_normals.push_back(normal);

    //std::cout << normalIndex << " " << temp_normals[ normalIndex-1 ].x << " " << temp_normals[ normalIndex-1 ].y << " " << temp_normals[ normalIndex-1 ].z << "\n";
  }
  return true;
}
