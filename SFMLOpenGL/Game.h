#ifndef GAME_H
#define GAME_H

#include <string>
#include <sstream>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <Debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GameObject.h>

using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();
private:
	GameObject* game_object[4];
	vec3 m_objectPositions[4];	// position vector used for collision detection

	GameObject* m_playerObject;
	vec3 m_playerPosition;		// position vector used for collision detection

	RenderWindow window;
	Clock clock;
	Time time;
	bool animate = false;
	vec3 animation = vec3(0.0f);
	float rotation = 0.0f;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();

	std::pair<bool, bool> checkCollision(vec3 t_objectOne, vec3 t_objectTwo);
	float m_objectLength{ 1.0f }; // the width and heigth of the objects
	int m_collisions{ 0 };

	int m_objectNum; // number of game objects

	enum class PlayerState { IDLE, JUMPING, FALLING };
	PlayerState m_playerState{ PlayerState::IDLE };
};

#endif  // ! GAME_H