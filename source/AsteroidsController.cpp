#include "AsteroidsController.h"



AsteroidsController::AsteroidsController(GLFWwindow* w)
	:shader(Shader("res/shaders/basic.shader")), 
	spaceCraftModel(SpaceCraftModel()),
	spaceCraftView(AsteroidsView(&spaceCraftModel, shader)),
	window(w)
{
	srand(time(NULL));
	AsteroidModel *asteroid1 = new AsteroidModel(0.05f);
	asteroids.insert(std::make_pair(asteroid1, new AsteroidsView(asteroid1, shader)));
	AsteroidModel* asteroid2 = new AsteroidModel(0.05f);
	asteroids.insert(std::make_pair(asteroid2, new AsteroidsView(asteroid2, shader)));
	AsteroidModel* asteroid3 = new AsteroidModel(0.05f);
	asteroids.insert(std::make_pair(asteroid3, new AsteroidsView(asteroid3, shader)));
}

AsteroidsController::~AsteroidsController()
{
}

bool AsteroidsController::GameTick()
{
	updateInput(window, spaceCraftModel.forward, spaceCraftModel.rotation);
	updatePosition(&spaceCraftModel.forward, &spaceCraftModel.pos);
	spaceCraftView.GameTick(&spaceCraftModel, &spaceCraftModel.pos, 10);


	for (auto& asteroidmv : asteroids)
	{		
		auto pos = updatePosition(&asteroidmv.first->forward, &asteroidmv.first->pos);
		asteroidmv.second->GameTick(asteroidmv.first, pos, 20);
		if (isCollision(&spaceCraftModel.pos, 0.01f, &asteroidmv.first->pos, 0.1f)) {
			//asteroid.Unbind();
			//gameIsRunning = false;
			std::cout << "spacecraft: " << &spaceCraftModel.pos.x << " y: " << &spaceCraftModel.pos.y << "\n Asteroid: " << &asteroidmv.first->pos.x << " " << &asteroidmv.first->pos.y << ":" << std::endl;
		}
	}

	
	//GLCall(glDrawArrays(GL_LINES, 0, 20));
	// 
	// 	   
	//for (AsteroidModel& asteroid : asteroids) {
	//	asteroid.GameTick();
	//	if (asteroids::isCollision(spaceCraft.pos, 0.01f, asteroid.pos, 0.1f)) {
	//		//asteroid.Unbind();
	//		//gameIsRunning = false;
	//		std::cout << "spacecraft: " << spaceCraft.pos.x << " y: " << spaceCraft.pos.y << "\n Asteroid: " << asteroid.pos.x << " " << asteroid.pos.y << ":" << std::endl;
	//	}
	//}
	return true;
}

