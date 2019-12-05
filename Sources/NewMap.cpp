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
void putTreeHere(unsigned int x,unsigned int y,unsigned int z,VoxelMap &currentMap,std::default_random_engine &engin){

	unsigned int hmax{12},hmin{3};

	//Tronc
	unsigned int height =hmin+ engin()%(hmax-hmin);
	for(unsigned int i = 0;i<=height;i++){
		currentMap.setColor(currentMap.getVoxelID(x,y,z+i), glm::vec4{0.5f,0.28f,0.0f,1.0f});
	}
	for(int i=-1 * (height/2);i<=static_cast<int>(height/2);i++){
		for(int j=-1 *(height/2);j<=static_cast<int>(height/2);j++){
			for(int k=-1 * (height/2);k<=static_cast<int>(height/2);k++){
				if( static_cast<unsigned int>((i*i+j*j+k*k))<=(height*3/2)&& (static_cast<int>(x)+i)>= 0 &&(static_cast<int>(y)+j)>= 0 &&(static_cast<int>(z)+k)>= 0 && (x+i)< currentMap.getWorldDimensions()[0]&&(y+j)< currentMap.getWorldDimensions()[1] && (z+k+height)< currentMap.getWorldDimensions()[2]){
					currentMap.setColor(currentMap.getVoxelID(x+i,y+j,z+k+height), glm::vec4{0.15f,0.51f,0.0f,1.0f});
				}
			}
		}
	}


	//Branches



	//Feuilles



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
					currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{0.24f,0.56f,0.0f ,1.f});
				}else{
					currentMap.setColor(currentMap.getVoxelID(x,y,z), glm::vec4{0.56f,0.24f,0.0f, 1.f});
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

	if(orientation[2] == 1.f) {  triangle[0] = 3; triangle[1] = 7; triangle[2] = 2;
								 triangle[3] = 7; triangle[4] = 2; triangle[5] = 6; }

	if(orientation[2] == -1.f) { triangle[0] = 0; triangle[1] = 4; triangle[2] = 1;
								 triangle[3] = 4; triangle[4] = 5; triangle[5] = 1; }



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

	for(unsigned int i{0}; i < allFaces.size(); i++) {

		pointsToAdd = getPointsOfOrientedFace(allFaces[i].second);

		pointsToDrawWithDoublon[i*4] = std::make_pair(allFaces[i].first, pointsToAdd[0]);
		pointsToDrawWithDoublon[i*4 + 1] = std::make_pair(allFaces[i].first, pointsToAdd[1]);
		pointsToDrawWithDoublon[i*4 + 2] = std::make_pair(allFaces[i].first, pointsToAdd[2]);
		pointsToDrawWithDoublon[i*4 + 3] = std::make_pair(allFaces[i].first, pointsToAdd[3]);
	}

	std::vector<std::pair<unsigned int, unsigned int>> pointsToDraw;
	pointsToDraw.reserve(pointsToDrawWithDoublon.size());

	bool foundPair{false};

	for(unsigned int i{0}; i < pointsToDrawWithDoublon.size(); i++) {

		foundPair = false;

		for(unsigned int j{0}; j < pointsToDraw.size() && !foundPair; j++) {

			if(pointsToDraw[j].first == pointsToDrawWithDoublon[i].first && pointsToDraw[j].second == pointsToDrawWithDoublon[i].second) { foundPair = true; }
		}

		if(!foundPair) { pointsToDraw.emplace_back(pointsToDrawWithDoublon[i]); }
	}

	std::array<unsigned int, 6> trianglesToAdd;
	std::vector<std::pair<unsigned int, unsigned int>> trianglesToDraw;
	trianglesToDraw.resize(allFaces.size()*6);

	for(unsigned int i{0}; i < allFaces.size(); i++) {

		trianglesToAdd = getTrianglesOfOrientedFace(allFaces[i].second);

		trianglesToDraw[i*6] = std::make_pair(allFaces[i].first, trianglesToAdd[0]);
		trianglesToDraw[i*6 + 1] = std::make_pair(allFaces[i].first, trianglesToAdd[1]);
		trianglesToDraw[i*6 + 2] = std::make_pair(allFaces[i].first, trianglesToAdd[2]);
		trianglesToDraw[i*6 + 3] = std::make_pair(allFaces[i].first, trianglesToAdd[3]);
		trianglesToDraw[i*6 + 4] = std::make_pair(allFaces[i].first, trianglesToAdd[4]);
		trianglesToDraw[i*6 + 5] = std::make_pair(allFaces[i].first, trianglesToAdd[5]);
	}

	mesh.m_vertexIndice.resize(trianglesToDraw.size());

	for(unsigned int i{0}; i < trianglesToDraw.size(); i++) {

		foundPair = false;

		for(unsigned int j{0}; j < pointsToDraw.size() && !foundPair; j++) {

			if(pointsToDraw[j].first == trianglesToDraw[i].first && pointsToDraw[j].second == trianglesToDraw[i].second) {

				foundPair = true;
				mesh.m_vertexIndice[i] = j;
			}
		}

		if(!foundPair) { mesh.m_vertexIndice[i] = 0; }
	}

	mesh.m_vertexPosition.resize(pointsToDraw.size());
	mesh.m_vertexNormal.resize(pointsToDraw.size());
	mesh.m_vertexColor.resize(pointsToDraw.size());

	for(unsigned int i{0}; i < pointsToDraw.size(); i++) {

		mesh.m_vertexPosition[i] = getPositionOfPoint(map, pointsToDraw[i].first, pointsToDraw[i].second);
		mesh.m_vertexNormal[i] = glm::vec3{1.f, 1.f, 1.f};
		mesh.m_vertexColor[i]  = map.getColor(pointsToDraw[i].first);
	}
}




void newMap(Gg::GulgEngine & engine, Gg::Entity &worldID, GLuint program){

	std::shared_ptr<Gg::Component::SceneObject> worldScene{std::make_shared<Gg::Component::SceneObject>()};
	std::shared_ptr<Gg::Component::Transformation> worldTransformation{std::make_shared<Gg::Component::Transformation>()};
	std::shared_ptr<Gg::Component::Mesh> worldMesh{std::make_shared<Gg::Component::Mesh>(program)};
	std::shared_ptr<VoxelMap> worldMap{std::make_shared<VoxelMap>(80, 80, 20)};

	engine.addComponentToEntity(worldID, "SceneObject", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldScene));
	engine.addComponentToEntity(worldID, "Transformations", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldTransformation));
	engine.addComponentToEntity(worldID, "MainMesh", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMesh));
	engine.addComponentToEntity(worldID, "VoxelMap", std::static_pointer_cast<Gg::Component::AbstractComponent>(worldMap));

	generateWorld(*worldMap, 4);

	worldMapToMesh(*worldMap, *worldMesh);

	worldMesh->reshape();
}
