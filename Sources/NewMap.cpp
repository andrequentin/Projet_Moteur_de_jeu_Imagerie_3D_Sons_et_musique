#include "NewMap.hpp"

double cubic_interpolate(double a,double b,double d){
    //Calcul des coefficients de notre polynôme
    double a3 = 1.5*a - 1.5*b ;
    double a2 = - 2.5*a + 2*b ;
    double a1 = 0.5*b;
    double a0 = a;
    //Calcul de la valeur de ce polynôme en t
    return (a3 * d*d*d) + (a2 * d*d) + (a1 * d) + a0;
	}

unsigned int biInterpolation(double a,double b,double c,double d,double u,double v){
	double  ap {cubic_interpolate(a,c,u)},
	 	 			bp  {cubic_interpolate(b,d,u)},
					result  {cubic_interpolate(ap,bp,v)};

	  return static_cast<unsigned int>(result);
}
void treeBranch(unsigned int x,unsigned int y,unsigned int z,VoxelMap &currentMap,std::default_random_engine &engin,unsigned int depth, int j){
  switch(j){
    case 0:
      x-=1;y-=1;
    break;
    case 1:
      x-=1;
    break;
    case 2:
      x-=1;y+=1;
    break;
    case 3:
      y+=1;
    break;
    case 4:
    x+=1;y+=1;
    break;
    case 5:
    x+=1;
    break;
    case 6:
    x+=1;y-=1;
    break;
    case 7:
    y-=1;
    break;
  }
  if(x<currentMap.getWorldDimensions()[0] && y<currentMap.getWorldDimensions()[1] &&z<currentMap.getWorldDimensions()[2] ){
    currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{0.5f,0.28f,0.0f,1.0f});
  }
  if(depth!=0){

    int nj = -1;
    nj += engin()%3;

    treeBranch(x,y,z+1,currentMap,engin,depth-1,(j+nj)%8);
  }
  for(int i{-1};i<=1;i++){
    for(int j{-1};j<=1;j++){
      for(int k{-1};k<=1;k++){
        if( i*i+k*k+j*j <= 1 &&
            (static_cast<int>(x)+i)>=0 &&
            (static_cast<int>(y)+j)>=0 &&
            (static_cast<int>(z)+k)>=0 &&
            ((x+i) < currentMap.getWorldDimensions()[0]) &&
            ((y+j) < currentMap.getWorldDimensions()[1]) &&
            ((z+k) < currentMap.getWorldDimensions()[2]) &&
            (currentMap.getColor(x+i,y+j,z+k)[3]==0.0f)
            ){

              currentMap.setColor(currentMap.getVoxelID(x+i,y+j,z+k), glm::vec4{0.25f,0.5f,0.1f,1.0f});
          }
        }
      }
    }
  }

void putTreeHere(unsigned int x,unsigned int y,unsigned int z,VoxelMap &currentMap,std::default_random_engine &engin){

	unsigned int hmax{15},hmin{5};

	//Tronc
	unsigned int height =hmin+ engin()%(hmax-hmin);
  unsigned int depth{static_cast<unsigned int>(engin()%(height/2))};
  for (unsigned int i {0};(i+z) < currentMap.getWorldDimensions()[2] && i<=height ;i++){
     currentMap.setColor(currentMap.getVoxelID(x,y,z+i), glm::vec4{0.5f,0.28f,0.005f,1.0f});
     for(unsigned int j{0};j<8;j++){
       if(engin()%8 ==j && i>2){
           treeBranch(x,y,z+i,currentMap,engin,depth,j);
       }
     }
  }
  for(int i{-2};i<=2;i++){
    for(int j{-2};j<=2;j++){
      for(int k{-2};k<=2;k++){
        if( i*i+k*k+j*j <= 4 &&
            (static_cast<int>(x)+i)>=0 &&
            (static_cast<int>(y)+j)>=0 &&
            (static_cast<int>(z)+k)>=0 &&
            ((x+i) < currentMap.getWorldDimensions()[0]) &&
            ((y+j) < currentMap.getWorldDimensions()[1]) &&
            ((z+k) < currentMap.getWorldDimensions()[2])  ){
              currentMap.setColor(currentMap.getVoxelID(x+i,y+j,z+k+height), glm::vec4{0.25f,0.5f,0.1f,1.0f});
          }
        }
      }
    }



}
void generateWorld(VoxelMap &currentMap, const unsigned int interpolationFrequency){
	// BRUIT PERLIN ALÉATOIRE
	std::vector<std::vector<unsigned int>> bruit;
	std::array<unsigned int, 3> worldDimension{currentMap.getWorldDimensions()};
	bruit.resize(worldDimension[0]);

	auto const seed = std::time(nullptr);
//	auto const seed=1575536532;
	std::cout<<seed<<std::endl;
	std::default_random_engine engin { seed };
	std::vector<std::vector<bool> > t;
	t.resize(worldDimension[0]);
	for(unsigned  x{0};x < worldDimension[0] ;x++){
		for(unsigned int y{0};y < worldDimension[1]; y++){
		 bruit[x].push_back(engin()%(worldDimension[2]/(interpolationFrequency/2)));
		 t[x].push_back((worldDimension[0]*worldDimension[1]*0.997f)<(engin()%(worldDimension[0]*worldDimension[1])));
	 }
	}

	if(worldDimension[0] % interpolationFrequency != 0 || worldDimension[1] % interpolationFrequency != 0){
	throw std::runtime_error("Error : worldDimension[0] and worldDimension[1] must be multiples of interpolationFrequency");
	}

	//INTERPOLATION LINÉAIRE 2D
	for(unsigned int f{0};f < interpolationFrequency;f++){
	for(unsigned int g{0};g < interpolationFrequency;g++){
	 unsigned int minX{(worldDimension[0]*f)/interpolationFrequency};
	 unsigned int  maxX{(worldDimension[0]*(f+1))/interpolationFrequency-1};
	 if(f>0)minX--;
	 unsigned int  minY{(worldDimension[1]*g)/interpolationFrequency};
	 unsigned int  maxY{(worldDimension[1]*(g+1))/interpolationFrequency-1};
	 if(g>0)minY--;
	 double  a (bruit[minX][minY]), // point d'interpolation 0-0
			b (bruit[minX][maxY]), // point d'interpolation 0-1
			c (bruit[maxX][minY]), // point d'interpolation 1-0
			d (bruit[maxX][maxY]); // point d'interpolation 1-1

			for(unsigned int i = minX; i <= maxX; i++) {
				for(unsigned int j = minY; j <= maxY; j++) {
			 		double  u=static_cast<double>(i-minX)/static_cast<double>(maxX-minX), // distance entre a et la coordonées x du point courant
				 					v=static_cast<double>(j-minY)/static_cast<double>(maxY-minY); // distance entre b et la coordonées y du point courant
					bruit[i][j]=biInterpolation(a,b,c,d,u,v);
		  	}
	 		}
		}
	}
	// std::cout<<"--------------"<<std::endl;

	for(unsigned int x{0};x < worldDimension[0] ;x++){
		for(unsigned int y{0};y < worldDimension[1]; y++){
			for(unsigned int z{0};z<worldDimension[2] && z<=bruit[x][y]  ;z++){
				if(z==bruit[x][y]){
					currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{0.24f,0.56f,0.1f ,1.f});
				}else{
					currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{0.56f,0.24f,0.05f, 1.f});
				}
			}
		 	if(t[x][y])	putTreeHere(x,y,bruit[x][y],currentMap,engin);
		}
	}

}

std::vector<std::pair<unsigned int, glm::vec3>> voxelsAndOrientations(const unsigned int voxelMapSize) {

	std::vector<std::pair<unsigned int, glm::vec3>> result;
	result.reserve(voxelMapSize*6);

	for(unsigned int i{0}; i < voxelMapSize; i++) {

		result.emplace_back(std::make_pair(i, glm::vec3{1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{-1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 1.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, -1.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, 1.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, -1.f}));
	}

	return result;
}
std::vector<std::pair<unsigned int, glm::vec3>> voxelsAndOrientations(std::vector<unsigned int> v) {

	std::vector<std::pair<unsigned int, glm::vec3>> result;
	result.reserve(v.size()*6);

	for(unsigned int j{0}; j < v.size(); j++) {
    unsigned int i = v[j];
		result.emplace_back(std::make_pair(i, glm::vec3{1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{-1.f, 0.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 1.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, -1.f, 0.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, 1.f}));
		result.emplace_back(std::make_pair(i, glm::vec3{0.f, 0.f, -1.f}));
	}

	return result;
}
std::vector<std::pair<unsigned int, glm::vec3>> selectVisibleFaces(const VoxelMap &map, const std::vector<std::pair<unsigned int, glm::vec3>> &facesToSelect) {

	std::vector<std::pair<unsigned int, glm::vec3>> result;
	result.reserve(facesToSelect.size());

	glm::vec3 testingFace;
	std::array<unsigned int, 3> worldSize{map.getWorldDimensions()};

	for(const std::pair<unsigned int, glm::vec3> &currentFace: facesToSelect) {

		testingFace = map.getVoxelPosition(currentFace.first) + currentFace.second;

		if(map.getColor(currentFace.first) != glm::vec4{0.f, 0.f, 0.f, 0.f}) { // Delete void voxel

			//Out of bounds, have to draw it
			if(testingFace.x < 0.f || testingFace.x >= worldSize[0]
			|| testingFace.y < 0.f || testingFace.y >= worldSize[1]
			|| testingFace.z < 0.f || testingFace.z >= worldSize[2]) {

				result.emplace_back(currentFace);
			}

			//No neighbourg
			else if(map.getColor(testingFace.x, testingFace.y, testingFace.z) == glm::vec4{0.f, 0.f, 0.f, 0.f}) {

				result.emplace_back(currentFace);
			}
		}
	}

	return result;
}

std::array<unsigned int, 4> getPointsOfOrientedFace(const glm::vec3 &orientation) {

	std::array<unsigned int, 4>  quad;

	if(orientation[0] == 1.f) {  quad[0] = 1; quad[1] = 5; quad[2] = 6; quad[3] = 2; }
	if(orientation[0] == -1.f) { quad[0] = 0; quad[1] = 3; quad[2] = 7; quad[3] = 4; }
	if(orientation[1] == 1.f) {  quad[0] = 4; quad[1] = 7; quad[2] = 6; quad[3] = 5; }
	if(orientation[1] == -1.f) { quad[0] = 0; quad[1] = 1; quad[2] = 2; quad[3] = 3; }
	if(orientation[2] == 1.f) {  quad[0] = 3; quad[1] = 7; quad[2] = 6; quad[3] = 2; }
	if(orientation[2] == -1.f) { quad[0] = 0; quad[1] = 4; quad[2] = 5; quad[3] = 1; }


	return quad;
}

std::array<unsigned int, 6> getTrianglesOfOrientedFace(const glm::vec3 &orientation) {

	std::array<unsigned int, 6> triangle;

	if(orientation[0] == 1.f) {  triangle[0] = 1; triangle[1] = 5; triangle[2] = 2;
								 triangle[3] = 5; triangle[4] = 6; triangle[5] = 2; }

	if(orientation[0] == -1.f) { triangle[0] = 0; triangle[1] = 3; triangle[2] = 4;
								 triangle[3] = 3; triangle[4] = 7; triangle[5] = 4; }

	if(orientation[1] == 1.f) {  triangle[0] = 4; triangle[1] = 7; triangle[2] = 5;
								 triangle[3] = 7; triangle[4] = 6; triangle[5] = 5; }

	if(orientation[1] == -1.f) { triangle[0] = 0; triangle[1] = 1; triangle[2] = 3;
								 triangle[3] = 1; triangle[4] = 2; triangle[5] = 3; }

	if(orientation[2] == 1.f) {  triangle[0] = 7; triangle[1] = 4; triangle[2] = 6;
								 triangle[3] = 4; triangle[4] = 6; triangle[5] = 2; }

	if(orientation[2] == -1.f) { triangle[0] = 0; triangle[1] = 1; triangle[2] = 4;
								 triangle[3] = 1; triangle[4] = 5; triangle[5] = 4; }


	return triangle;
}

glm::vec3 getPositionOfPoint(const VoxelMap &map, const unsigned int voxelID, const unsigned int point) {

	glm::vec3 centerPosition{map.getVoxelPosition(voxelID)};

	if(point == 0) { centerPosition += glm::vec3{-0.5f, -0.5f, -0.5f}; }
	if(point == 1) { centerPosition += glm::vec3{0.5f, -0.5f, -0.5f}; }
	if(point == 2) { centerPosition += glm::vec3{0.5f, -0.5f, 0.5f}; }
	if(point == 3) { centerPosition += glm::vec3{-0.5f, -0.5f, 0.5f}; }
	if(point == 4) { centerPosition += glm::vec3{-0.5f, 0.5f, -0.5f}; }
	if(point == 5) { centerPosition += glm::vec3{0.5f, 0.5f, -0.5f}; }
	if(point == 6) { centerPosition += glm::vec3{0.5f, 0.5f, 0.5f}; }
	if(point == 7) { centerPosition += glm::vec3{-0.5f, 0.5f, 0.5f}; }

	return centerPosition;
}

void localRemeshing(std::vector<std::vector<unsigned int>> v,VoxelMap &map, Gg::Component::Mesh &mesh){
  for(unsigned int x{0};x<v.size();x++){
    for(unsigned int y{0};y<v[x].size();y++){
      unsigned int idV = v[x][y];
      for(unsigned int i{0};i<map.idVoxel_vertexInds[idV].size();i++){
        unsigned int ind =  map.idVoxel_vertexInds[idV][i];
        //Hiding old vertex
        mesh.m_vertexPosition[ind*4]=glm::vec3{0.f,0.f,0.f};
        mesh.m_vertexPosition[ind*4+1]=glm::vec3{0.f,0.f,0.f};
        mesh.m_vertexPosition[ind*4+2]=glm::vec3{0.f,0.f,0.f};
        mesh.m_vertexPosition[ind*4+3]=glm::vec3{0.f,0.f,0.f};
      }
      map.idVoxel_vertexInds[idV].clear();
    }
    std::vector<std::pair<unsigned int, glm::vec3>> allFaces{voxelsAndOrientations(v[x])};

    allFaces = selectVisibleFaces(map, allFaces);
    unsigned int s_t= mesh.m_vertexPosition.size()/4;
    mesh.m_vertexPosition.resize(mesh.m_vertexPosition.size() + allFaces.size()*4);
    mesh.m_vertexNormal.resize(mesh.m_vertexNormal.size() + allFaces.size()*4);
    mesh.m_vertexColor.resize(mesh.m_vertexColor.size() + allFaces.size()*4);
    mesh.m_vertexIndice.resize(mesh.m_vertexIndice.size() + allFaces.size()*6);
    std::array<unsigned int, 4> pointsToAdd;
    glm::vec3 currentNormal;

    for(unsigned int ii{0};ii<allFaces.size();ii++){
      unsigned int i = ii+s_t;
      map.idVoxel_vertexInds[allFaces[ii].first].push_back(i);
  		pointsToAdd = getPointsOfOrientedFace(allFaces[ii].second);

  		mesh.m_vertexPosition[i*4] = getPositionOfPoint(map, allFaces[ii].first, pointsToAdd[0]);
  		mesh.m_vertexPosition[i*4 + 1] = getPositionOfPoint(map, allFaces[ii].first, pointsToAdd[1]);
  		mesh.m_vertexPosition[i*4 + 2] = getPositionOfPoint(map, allFaces[ii].first, pointsToAdd[2]);
  		mesh.m_vertexPosition[i*4 + 3] = getPositionOfPoint(map, allFaces[ii].first, pointsToAdd[3]);

  		mesh.m_vertexColor[i*4] = map.getColor(allFaces[ii].first);
  		mesh.m_vertexColor[i*4 + 1] = map.getColor(allFaces[ii].first);
  		mesh.m_vertexColor[i*4 + 2] = map.getColor(allFaces[ii].first);
  		mesh.m_vertexColor[i*4 + 3] = map.getColor(allFaces[ii].first);

  		currentNormal = glm::triangleNormal(mesh.m_vertexPosition[i*4],
  											mesh.m_vertexPosition[i*4 + 1],
  											mesh.m_vertexPosition[i*4 + 2]);

  		mesh.m_vertexNormal[i*4] = currentNormal;
  		mesh.m_vertexNormal[i*4 + 1] = currentNormal;
  		mesh.m_vertexNormal[i*4 + 2] = currentNormal;
  		mesh.m_vertexNormal[i*4 + 3] = currentNormal;

  		mesh.m_vertexIndice[i*6] = i*4;
  		mesh.m_vertexIndice[i*6 + 1] = i*4 + 1;
  		mesh.m_vertexIndice[i*6 + 2] = i*4 + 2;

  		mesh.m_vertexIndice[i*6 + 3] = i*4;
  		mesh.m_vertexIndice[i*6 + 4] = i*4 + 2;
  		mesh.m_vertexIndice[i*6 + 5] = i*4 + 3;
    }

  }
}
void worldMapToMesh(VoxelMap &map, Gg::Component::Mesh &mesh) {

	mesh.m_vertexPosition.clear();
	mesh.m_vertexColor.clear();
	mesh.m_vertexNormal.clear();
	std::array<unsigned int, 3> worldDimensions{map.getWorldDimensions()};

	std::vector<std::pair<unsigned int, glm::vec3>> allFaces{voxelsAndOrientations(worldDimensions[0]*worldDimensions[1]*worldDimensions[2])};
	allFaces = selectVisibleFaces(map, allFaces);

	std::vector<std::pair<unsigned int, unsigned int>> pointsToDrawWithDoublon;
	pointsToDrawWithDoublon.resize(allFaces.size()*4);

	std::array<unsigned int, 4> pointsToAdd;

	mesh.m_vertexPosition.resize(allFaces.size()*4);
  mesh.m_vertexNormal.resize(allFaces.size()*4);
	mesh.m_vertexColor.resize(allFaces.size()*4);
	mesh.m_vertexIndice.resize(allFaces.size()*6);

  // std::cout<<allFaces.size()<<std::endl;
  // std::cout<<worldDimensions[0]*worldDimensions[1]*worldDimensions[2]<<std::endl;
  // std::cout<<mesh.m_vertexColor.size()<<std::endl;
	glm::vec3 currentNormal;

  map.idVoxel_vertexInds.clear();
  map.idVoxel_vertexInds.resize(worldDimensions[0]*worldDimensions[1]*worldDimensions[2]);
	for(unsigned int i{0}; i < allFaces.size(); i++) {

    map.idVoxel_vertexInds[allFaces[i].first].push_back(i);
		pointsToAdd = getPointsOfOrientedFace(allFaces[i].second);

		mesh.m_vertexPosition[i*4] = getPositionOfPoint(map, allFaces[i].first, pointsToAdd[0]);
		mesh.m_vertexPosition[i*4 + 1] = getPositionOfPoint(map, allFaces[i].first, pointsToAdd[1]);
		mesh.m_vertexPosition[i*4 + 2] = getPositionOfPoint(map, allFaces[i].first, pointsToAdd[2]);
		mesh.m_vertexPosition[i*4 + 3] = getPositionOfPoint(map, allFaces[i].first, pointsToAdd[3]);

		mesh.m_vertexColor[i*4] = map.getColor(allFaces[i].first);
		mesh.m_vertexColor[i*4 + 1] = map.getColor(allFaces[i].first);
		mesh.m_vertexColor[i*4 + 2] = map.getColor(allFaces[i].first);
		mesh.m_vertexColor[i*4 + 3] = map.getColor(allFaces[i].first);

		currentNormal = glm::triangleNormal(mesh.m_vertexPosition[i*4],
											mesh.m_vertexPosition[i*4 + 1],
											mesh.m_vertexPosition[i*4 + 2]);

		mesh.m_vertexNormal[i*4] = currentNormal;
		mesh.m_vertexNormal[i*4 + 1] = currentNormal;
		mesh.m_vertexNormal[i*4 + 2] = currentNormal;
		mesh.m_vertexNormal[i*4 + 3] = currentNormal;

		mesh.m_vertexIndice[i*6] = i*4;
		mesh.m_vertexIndice[i*6 + 1] = i*4 + 1;
		mesh.m_vertexIndice[i*6 + 2] = i*4 + 2;

		mesh.m_vertexIndice[i*6 + 3] = i*4;
		mesh.m_vertexIndice[i*6 + 4] = i*4 + 2;
		mesh.m_vertexIndice[i*6 + 5] = i*4 + 3;
	}
}


void Cube(std::shared_ptr<Gg::Component::Mesh> mesh,float size){

  mesh->m_vertexPosition.resize(8);
    mesh->m_vertexPosition[0] = glm::vec3{-size, -size, -size};
    mesh->m_vertexPosition[1] = glm::vec3{size, -size, -size};
    mesh->m_vertexPosition[2] = glm::vec3{size, size, -size};
    mesh->m_vertexPosition[3] = glm::vec3{-size, size, -size};
    mesh->m_vertexPosition[4] = glm::vec3{-size, -size, size};
    mesh->m_vertexPosition[5] = glm::vec3{size, -size, size};
    mesh->m_vertexPosition[6] = glm::vec3{size, size, size};
    mesh->m_vertexPosition[7] = glm::vec3{-size, size, size};
     for(unsigned int i{0}; i < 8; i++) {
        mesh->m_vertexNormal.emplace_back(glm::vec3{0.f, 0.f, 1.f});
        mesh->m_vertexColor.emplace_back(glm::vec3{0.f, 1.f, 1.f});
    }
    mesh->m_vertexIndice.emplace_back(0);    mesh->m_vertexIndice.emplace_back(1);    mesh->m_vertexIndice.emplace_back(3);
    mesh->m_vertexIndice.emplace_back(3);    mesh->m_vertexIndice.emplace_back(1);    mesh->m_vertexIndice.emplace_back(2);
    mesh->m_vertexIndice.emplace_back(1);    mesh->m_vertexIndice.emplace_back(5);    mesh->m_vertexIndice.emplace_back(2);
    mesh->m_vertexIndice.emplace_back(2);    mesh->m_vertexIndice.emplace_back(5);    mesh->m_vertexIndice.emplace_back(6);
    mesh->m_vertexIndice.emplace_back(5);    mesh->m_vertexIndice.emplace_back(4);    mesh->m_vertexIndice.emplace_back(6);
    mesh->m_vertexIndice.emplace_back(6);    mesh->m_vertexIndice.emplace_back(4);    mesh->m_vertexIndice.emplace_back(7);
    mesh->m_vertexIndice.emplace_back(4);    mesh->m_vertexIndice.emplace_back(0);    mesh->m_vertexIndice.emplace_back(7);
    mesh->m_vertexIndice.emplace_back(7);    mesh->m_vertexIndice.emplace_back(0);    mesh->m_vertexIndice.emplace_back(3);
    mesh->m_vertexIndice.emplace_back(3);    mesh->m_vertexIndice.emplace_back(2);    mesh->m_vertexIndice.emplace_back(7);
    mesh->m_vertexIndice.emplace_back(7);    mesh->m_vertexIndice.emplace_back(2);    mesh->m_vertexIndice.emplace_back(6);
    mesh->m_vertexIndice.emplace_back(4);    mesh->m_vertexIndice.emplace_back(5);    mesh->m_vertexIndice.emplace_back(0);
    mesh->m_vertexIndice.emplace_back(0);    mesh->m_vertexIndice.emplace_back(5);    mesh->m_vertexIndice.emplace_back(1);

    mesh->reshape();
}


void newMap(Gg::GulgEngine & engine, Gg::Entity &worldID, GLuint program){

	std::shared_ptr<Gg::Component::SceneObject> worldScene{std::make_shared<Gg::Component::SceneObject>()};
	std::shared_ptr<Gg::Component::Transformation> worldTransformation{std::make_shared<Gg::Component::Transformation>()};
	std::shared_ptr<Gg::Component::Mesh> worldMesh{std::make_shared<Gg::Component::Mesh>(program)};
	std::shared_ptr<VoxelMap> worldMap{std::make_shared<VoxelMap>(200, 600, 40)};

	engine.addComponentToEntity(worldID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldScene));
	engine.addComponentToEntity(worldID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldTransformation));
	engine.addComponentToEntity(worldID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMesh));
	engine.addComponentToEntity(worldID, "VoxelMap", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMap));

	generateWorld(*worldMap, 4);

	worldMapToMesh(*worldMap, *worldMesh);

	worldMesh->reshape();
}
