#include <Game.h>
#include <Cube.h>
#include <Easing.h>

// Helper to convert Number to String for HUD
template <typename T>
string toString(T number)
{
	ostringstream oss;
	oss << number;
	return oss.str();
}

GLuint	vsid,		// Vertex Shader ID
fsid,		// Fragment Shader ID
progID,		// Program ID
vao = 0,	// Vertex Array ID
vbo,		// Vertex Buffer ID
vib,		// Vertex Index Buffer
to[1];		// Texture ID
GLint	positionID,	// Position ID
colorID,	// Color ID
textureID,	// Texture ID
textureTwoID,	// Texture ID my additionnnn
uvID,		// UV ID
mvpID,		// Model View Projection ID
playerMvpID, // MY ADDITION
x_offsetID, // X offset ID
y_offsetID,	// Y offset ID
z_offsetID;	// Z offset ID

GLenum	error;		// OpenGL Error Code


//Please see .//Assets//Textures// for more textures
const string filenamePlayer = ".//Assets//Textures//texture_2.tga";
const string filenameCoin = ".//Assets//Textures//coin.tga";
const string filenameGround = ".//Assets//Textures//blank.tga";

int width;						// Width of texture
int height;						// Height of texture
int comp_count;					// Component of texture

unsigned char* img_dataPlayer;		// image data
unsigned char* img_dataCoin;		// image data
unsigned char* img_dataGround;		// image data

mat4 mvp, projection,
view, model;			// Model View Projection

mat4 mvpPlayer, modelPlayer;	 // think projection and view will stay the same for every object

Font font;						// Game font

float x_offset, y_offset, z_offset; // offset on screen (Vertex Shader)

Game::Game() :
	window(VideoMode(800, 600),
		"Introduction to OpenGL Texturing")
{
}

Game::Game(sf::ContextSettings settings) :
	window(VideoMode(800, 600),
		"Introduction to OpenGL Texturing",
		sf::Style::Default,
		settings)
{
	m_objectNum = 12;	// number of game objects

	m_playerObject = new GameObject();
	m_playerObject->setPosition(vec3(0.5f, 0.5f, -2.0f));

	// Obstacles
	game_objectObstacle[0] = new GameObject();
	game_objectObstacle[0]->setPosition(vec3(20, 0.5f, -2.0f));

	game_objectObstacle[1] = new GameObject();
	game_objectObstacle[1]->setPosition(vec3(80, 0.5f, -2.0f));

	game_objectObstacle[2] = new GameObject();
	game_objectObstacle[2]->setPosition(vec3(100, 0.5f, -2.0f));

	game_objectObstacle[3] = new GameObject();
	game_objectObstacle[3]->setPosition(vec3(180, 0.5f, -2.0f));

	game_objectObstacle[4] = new GameObject();
	game_objectObstacle[4]->setPosition(vec3(200, 0.5f, -2.0f));

	game_objectObstacle[5] = new GameObject();
	game_objectObstacle[5]->setPosition(vec3(250, 0.5f, -2.0f));

	game_objectObstacle[6] = new GameObject();
	game_objectObstacle[6]->setPosition(vec3(275, 0.5f, -2.0f));

	game_objectObstacle[7] = new GameObject();
	game_objectObstacle[7]->setPosition(vec3(300, 0.5f, -2.0f));

	game_objectObstacle[8] = new GameObject();
	game_objectObstacle[8]->setPosition(vec3(340, 0.5f, -2.0f));

	game_objectObstacle[9] = new GameObject();
	game_objectObstacle[9]->setPosition(vec3(360, 0.5f, -2.0f));

	game_objectObstacle[10] = new GameObject();
	game_objectObstacle[10]->setPosition(vec3(390, 0.5f, -2.0f));

	game_objectObstacle[11] = new GameObject();
	game_objectObstacle[11]->setPosition(vec3(410, 0.5f, -2.0f));

	// Coins
	game_objectCoins[0] = new GameObject();
	game_objectCoins[0]->setPosition(vec3(50, 0.5f, -2.0f));

	game_objectCoins[1] = new GameObject();
	game_objectCoins[1]->setPosition(vec3(120, 3.5f, -2.0f));

	game_objectCoins[2] = new GameObject();
	game_objectCoins[2]->setPosition(vec3(150, 0.5f, -2.0f));

	game_objectCoins[3] = new GameObject();
	game_objectCoins[3]->setPosition(vec3(220, 0.5f, -2.0f));

	game_objectCoins[4] = new GameObject();
	game_objectCoins[4]->setPosition(vec3(320, 0.5f, -2.0f));

	game_objectCoins[5] = new GameObject();
	game_objectCoins[5]->setPosition(vec3(350, 0.5f, -2.0f));

	game_objectCoins[6] = new GameObject();
	game_objectCoins[6]->setPosition(vec3(370, 0.5f, -2.0f));

	game_objectCoins[7] = new GameObject();
	game_objectCoins[7]->setPosition(vec3(180, 3.5f, -2.0f));

	game_objectCoins[8] = new GameObject();
	game_objectCoins[8]->setPosition(vec3(420, 0.5f, -2.0f));

	game_objectCoins[9] = new GameObject();
	game_objectCoins[9]->setPosition(vec3(440, 0.5f, -2.0f));

	game_objectCoins[10] = new GameObject();
	game_objectCoins[10]->setPosition(vec3(460, 0.5f, -2.0f));

	game_objectCoins[11] = new GameObject();
	game_objectCoins[11]->setPosition(vec3(480, 0.5f, -2.0f));

	// initialize pos vectors
	m_playerPosition = m_playerObject->getPosition();
	for (int i = 0; i < m_objectNum; i++)
	{
		m_objectCoinPositions[i] = game_objectCoins[i]->getPosition();
		m_objectObstaclePositions[i] = game_objectObstacle[i]->getPosition();
	}

	// ground object stuff..
	GameObject firstGround;
	firstGround.setPosition(vec3(-2.0f, -1.5f, -2.0f));
	m_groundCubes.push_back(firstGround);

	for (int i = 0; i < 300; i++)
	{
		GameObject temp;
		temp.setPosition(vec3(m_groundCubes.back().getPosition().x + 2, m_groundCubes.back().getPosition().y, -2.0f));
		m_groundCubes.push_back(temp);
	}
}

Game::~Game()
{
	delete m_playerObject;
	for (int i = 0; i < m_objectNum; i++)
	{
		delete game_objectCoins[i];
	}
	m_groundCubes.clear(); // deletes all ground objects
}


void Game::run()
{

	initialize();

	Event event;

	float rotation = 0.01f;
	float start_value = 0.0f;
	float end_value = 1.0f;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			//{
			//	// Set Model Rotation
			//	if (!animate)
			//	{
			//		animate = true;
			//		if (rotation < 0)
			//			rotation *= -1; // Set Positive
			//		animation = glm::vec3(0, 1, 0); //Rotate Y
			//	}
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			//{
			//	// Set Model Rotation
			//	if (!animate)
			//	{
			//		animate = true;
			//		if (rotation >= 0)
			//			rotation *= -1; // Set Negative
			//		animation = glm::vec3(0, 1, 0); //Rotate Y
			//	}

				// https://www.sfml-dev.org/documentation/2.0/classsf_1_1Clock.php
				// https://github.com/acron0/Easings
				// http://robotacid.com/documents/code/Easing.cs
				// http://st33d.tumblr.com/post/94243475686/easing-equations-for-unity-c
				// http://easings.net/
				// http://upshots.org/actionscript/jsas-understanding-easing
				// https://www.kirupa.com/html5/animating_with_easing_functions_in_javascript.htm
				// https://medium.com/motion-in-interaction/animation-principles-in-ui-design-understanding-easing-bea05243fe3#.svh4gczav
				// http://thednp.github.io/kute.js/easing.html
				// http://gizma.com/easing/#quad1
				// https://github.com/warrenm/AHEasing

				// VR
				// https://www.sfml-dev.org/documentation/2.4.2/classsf_1_1Sensor.php
				// http://en.sfml-dev.org/forums/index.php?topic=9412.msg65594
				// https://github.com/SFML/SFML/wiki/Tutorial:-Building-SFML-for-Android-on-Windows
				// https://github.com/SFML/SFML/wiki/Tutorial:-Building-SFML-for-Android
				// https://www.youtube.com/watch?v=n_JSi6ihDFs
				// http://en.sfml-dev.org/forums/index.php?topic=8010.0
				// 

				/*
				// Set Model Rotation
				// t = time, b = startvalue, c = change in value, d = duration:

				time = clock.getElapsedTime();
				std::cout << time.asSeconds() << std::endl;
				float original = 0.001f;
				float destination = 0.05f;

				float factor, temp;

				for (int t = 0; t < 5.0f; t++)
				{
				factor = gpp::Easing::easeIn(t, original, 0.00001f, 5.0f);
				cout << "Factor : " << factor << endl;
				}


				factor = gpp::Easing::easeIn(time.asMilliseconds(), original, 0.00001f, 5.0f);
				cout << "Factor : " << factor << endl;
				temp = original + ((destination - original) * factor);
				cout << "Temp : " << factor << endl;
				model = rotate(model, temp, glm::vec3(0, 1, 0)); // Rotate
				*/
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			//{
			//	// Set Model Rotation
			//	//model = rotate(model, -0.01f, glm::vec3(1, 0, 0)); // Rotate
			//	modelPlayer = glm::translate(modelPlayer, glm::vec3(0, 0.1, 0));
			//	m_playerObject->setPosition(glm::vec3(m_playerObject->getPosition().x, m_playerObject->getPosition().y + 0.1, m_playerObject->getPosition().z));
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			//{
			//	// Set Model Rotation
			//	model = rotate(model, 0.01f, glm::vec3(1, 0, 0)); // Rotate
			//	modelPlayer = glm::translate(modelPlayer, glm::vec3(0, -0.1, 0));
			//}

			if (animate)
			{
				rotation += (1.0f * rotation) + 0.05f;
				model = rotate(model, 0.01f, animation); // Rotate
				rotation = 0.0f;
				animate = false;
			}

			/*if (m_playerObject->getPosition().y > 0.5f)
			{
				modelPlayer = glm::translate(modelPlayer, glm::vec3(0, -0.005, 0));
				m_playerObject->setPosition(glm::vec3(m_playerObject->getPosition().x, m_playerObject->getPosition().y - 0.005, m_playerObject->getPosition().z));
			}*/

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_playerState == PlayerState::IDLE)
			{
				m_playerState = PlayerState::JUMPING;
			}


			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::V)
				{
					// Camera Matrix
					view = lookAt(
						vec3(-15.0f, 10.0f, -2.0f),	// Camera (x,y,z), in World Space
						vec3(0.0f, 0.0f, -2.0f),		// Camera looking at origin
						vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
					);
				}

				if (event.key.code == sf::Keyboard::C)
				{
					// Camera Matrix
					view = lookAt(
						vec3(0.0f, 0.0f, 15.0f),	// Camera (x,y,z), in World Space
						vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
						vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
					);
				}

				if (event.key.code == sf::Keyboard::B)
				{
					// Camera Matrix
					view = lookAt(
						vec3(-5.0f, 5.0f, 15.0f),	// Camera (x,y,z), in World Space
						vec3(0.0f, 0.0f, -2.0f),		// Camera looking at origin
						vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
					);
				}
			}
		}
		update();
		render();
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();

}

void Game::reset()
{
	// Model matrix
	model = mat4(
		1.0f					// Identity Matrix set back to 1 so all objects return to initial position
	);

	// reset all posiiotns.. to there originals
	// obstacles
	game_objectObstacle[0]->setPosition(vec3(20, 0.5f, -2.0f));
	game_objectObstacle[1]->setPosition(vec3(80, 0.5f, -2.0f));
	game_objectObstacle[2]->setPosition(vec3(100, 0.5f, -2.0f));
	game_objectObstacle[3]->setPosition(vec3(180, 0.5f, -2.0f));
	game_objectObstacle[4]->setPosition(vec3(200, 0.5f, -2.0f));
	game_objectObstacle[5]->setPosition(vec3(250, 0.5f, -2.0f));
	game_objectObstacle[6]->setPosition(vec3(275, 0.5f, -2.0f));
	game_objectObstacle[7]->setPosition(vec3(300, 0.5f, -2.0f));
	game_objectObstacle[8]->setPosition(vec3(340, 0.5f, -2.0f));
	game_objectObstacle[9]->setPosition(vec3(360, 0.5f, -2.0f));
	game_objectObstacle[10]->setPosition(vec3(390, 0.5f, -2.0f));
	game_objectObstacle[11]->setPosition(vec3(410, 0.5f, -2.0f));

	// coins
	game_objectCoins[0]->setPosition(vec3(50, 0.5f, -2.0f));
	game_objectCoins[1]->setPosition(vec3(120, 3.5f, -2.0f));
	game_objectCoins[2]->setPosition(vec3(150, 0.5f, -2.0f));
	game_objectCoins[3]->setPosition(vec3(220, 0.5f, -2.0f));
	game_objectCoins[4]->setPosition(vec3(320, 0.5f, -2.0f));
	game_objectCoins[5]->setPosition(vec3(350, 0.5f, -2.0f));
	game_objectCoins[6]->setPosition(vec3(370, 0.5f, -2.0f));
	game_objectCoins[7]->setPosition(vec3(180, 3.5f, -2.0f));
	game_objectCoins[8]->setPosition(vec3(420, 0.5f, -2.0f));
	game_objectCoins[9]->setPosition(vec3(440, 0.5f, -2.0f));
	game_objectCoins[10]->setPosition(vec3(460, 0.5f, -2.0f));
	game_objectCoins[11]->setPosition(vec3(480, 0.5f, -2.0f));

	for (int i = 0; i < m_objectNum; i++)
	{
		m_objectCoinPositions[i] = game_objectCoins[i]->getPosition();
		m_objectObstaclePositions[i] = game_objectObstacle[i]->getPosition();
	}

	m_Points = 0; //reset
}

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (!(!glewInit())) { DEBUG_MSG("glewInit() failed"); }

	// Copy UV's to all faces
	for (int i = 1; i < 6; i++)
		memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	// Vertex Array Buffer
	glGenBuffers(1, &vbo);		// Generate Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Vertices (3) x,y,z , Colors (4) RGBA, UV/ST (2)

	int countVERTICES = game_objectCoins[0]->getVertexCount();
	//countVERTICES += game_object[1]->getVertexCount();  i added this, dont think its needed
	int countCOLORS = game_objectCoins[0]->getColorCount();
	//countCOLORS += game_object[1]->getColorCount();
	int countUVS = game_objectCoins[0]->getUVCount();
	//countUVS += game_object[1]->getUVCount();

	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Generate Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);


	int countINDICES = game_objectCoins[0]->getIndexCount();
	// Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// NOTE: uniforms values must be used within Shader so that they 
	// can be retreived
	const char* vs_src =
		"#version 400\n\r"
		""
		"in vec3 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_uv;"
		""
		"out vec4 color;"
		"out vec2 uv;"
		""
		"uniform mat4 sv_mvp;"
		"uniform mat4 sv_mvpPlayer;"	// MY ADDITION
		"uniform float sv_x_offset;"
		"uniform float sv_y_offset;"
		"uniform float sv_z_offset;"
		""
		"void main() {"
		"	color = sv_color;"
		"	uv = sv_uv;"
		//"	gl_Position = vec4(sv_position, 1);"
		"	gl_Position = sv_mvp * vec4(sv_position.x + sv_x_offset, sv_position.y + sv_y_offset, sv_position.z + sv_z_offset, 1 );"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	// Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	const char* fs_src =
		"#version 400\n\r"
		""
		"uniform sampler2D f_texture;"
		""
		"in vec4 color;"
		"in vec2 uv;"
		""
		"out vec4 fColor;"
		""
		"void main() {"
		"	fColor = texture2D(f_texture, uv);"
		""
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);

	// Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	// Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Set image data
	// https://github.com/nothings/stb/blob/master/stb_image.h
	img_dataPlayer = stbi_load(filenamePlayer.c_str(), &width, &height, &comp_count, 4);

	img_dataCoin = stbi_load(filenameCoin.c_str(), &width, &height, &comp_count, 4);

	img_dataGround = stbi_load(filenameGround.c_str(), &width, &height, &comp_count, 4);

	if (img_dataPlayer == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	if (img_dataCoin == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to[0]);
	glBindTexture(GL_TEXTURE_2D, to[0]);

	// Wrap around
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Filtering
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind to OpenGL
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_dataPlayer				// Image Data
	);

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
	);

	// Camera Matrix
	view = lookAt(
		vec3(0.0f, 0.0f, 15.0f),	// Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
	);

	// Model matrix
	model = mat4(
		1.0f					// Identity Matrix
	);

	modelPlayer = mat4(
		1.0f					// Identity Matrix for player
	);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Load Font
	font.loadFromFile(".//Assets//Fonts//BBrick.ttf");
}

void Game::update()
{
	// 

	// player states..
	switch (m_playerState)
	{
	case PlayerState::IDLE:

		break;
	case PlayerState::JUMPING:
		modelPlayer = glm::translate(modelPlayer, glm::vec3(0, 0.07, 0));
		m_playerPosition = (glm::vec3(m_playerPosition.x, m_playerPosition.y + 0.07, m_playerPosition.z));

		if (m_playerPosition.y > 7.0f)
		{
			m_playerState = PlayerState::FALLING;
		}
		break;
	case PlayerState::FALLING:
		if (m_collisionState != CollisionState::TOP)
		{
			modelPlayer = glm::translate(modelPlayer, glm::vec3(0, -0.07, 0));
			m_playerPosition = (glm::vec3(m_playerPosition.x, m_playerPosition.y - 0.07, m_playerPosition.z));
		}

		if (m_playerPosition.y < 0.51f)
		{
			m_playerState = PlayerState::IDLE;
		}
		break;

	}

	for (int i = 0; i < m_objectNum; i++)
	{
		std::pair<bool, bool> collision = checkCollision(m_playerPosition, m_objectObstaclePositions[i]);
		if (collision.first == true && collision.second == true)
		{
			m_collisionIndex = i;
			//then a collision has happened with an obstacle
			if (m_playerPosition.y > m_objectObstaclePositions[i].y + 1)
			{
				m_collisionState = CollisionState::TOP;
			}
			else
			{
				reset();
			}
		}

		collision = checkCollision(m_playerPosition, m_objectCoinPositions[i]);
		if (collision.first == true && collision.second == true)
		{
			//then a collision has happened with a coin
			game_objectCoins[i]->setPosition(vec3(-100, -100, -100)); // move offscreen so cant be hit again
			m_objectCoinPositions[i] = vec3(-100, -100, -100);
			m_Points++;
		}
	}
	
	if (m_playerPosition.x > m_objectObstaclePositions[m_collisionIndex].x + 2 && m_collisionState == CollisionState::TOP)
	{
		m_collisionState = CollisionState::NORMAL;
	}


	model = glm::translate(model, glm::vec3(-0.05, 0, 0));
	for (int i = 0; i < m_objectNum; i++)
	{
		m_objectCoinPositions[i] = (glm::vec3(m_objectCoinPositions[i].x - 0.05, m_objectCoinPositions[i].y, m_objectCoinPositions[i].z)); // for collision detection
		m_objectObstaclePositions[i] = (glm::vec3(m_objectObstaclePositions[i].x - 0.05, m_objectObstaclePositions[i].y, m_objectObstaclePositions[i].z)); // for collision detection
	}
#if (DEBUG >= 2)
	DEBUG_MSG("Updating...");
#endif
	// Update Model View Projection
	// For mutiple objects (cubes) create multiple models
	// To alter Camera modify view & projection
	mvp = projection * view * model;
	mvpPlayer = projection * view * modelPlayer;  // i added this

	DEBUG_MSG(model[0].x);
	DEBUG_MSG(model[0].y);
	DEBUG_MSG(model[0].z);
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save current OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7
	window.pushGLStates();

	// Find mouse position using sf::Mouse
	int x = Mouse::getPosition(window).x;
	int y = Mouse::getPosition(window).y;

	/*string hud = "Heads Up Display ["
	+ string(toString(x))
	+ "]["
	+ string(toString(y))
	+ "]";*/

	string hud = "Points: " + std::to_string(m_Points);

	Text text(hud, font);

	text.setFillColor(sf::Color(255, 255, 255, 170));
	text.setPosition(50.f, 50.f);

	window.draw(text);

	// Restore OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7

	window.popGLStates();

	// Rebind Buffers and then set SubData
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	// Use Progam on GPU
	glUseProgram(progID);

	// Find variables within the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	if (positionID < 0) { DEBUG_MSG("positionID not found"); }

	colorID = glGetAttribLocation(progID, "sv_color");
	if (colorID < 0) { DEBUG_MSG("colorID not found"); }

	uvID = glGetAttribLocation(progID, "sv_uv");
	if (uvID < 0) { DEBUG_MSG("uvID not found"); }

	textureID = glGetUniformLocation(progID, "f_texture");
	if (textureID < 0) { DEBUG_MSG("textureID not found"); }

	mvpID = glGetUniformLocation(progID, "sv_mvp");
	if (mvpID < 0) { DEBUG_MSG("mvpID not found"); }

	// MY ADDITION
	playerMvpID = glGetUniformLocation(progID, "sv_mvpPlayer");
	if (playerMvpID < 0) { DEBUG_MSG("playerMvpID not found"); }

	x_offsetID = glGetUniformLocation(progID, "sv_x_offset");
	if (x_offsetID < 0) { DEBUG_MSG("x_offsetID not found"); }

	y_offsetID = glGetUniformLocation(progID, "sv_y_offset");
	if (y_offsetID < 0) { DEBUG_MSG("y_offsetID not found"); }

	z_offsetID = glGetUniformLocation(progID, "sv_z_offset");
	if (z_offsetID < 0) { DEBUG_MSG("z_offsetID not found"); };

	// VBO Data....vertices, colors and UV's appended
	// Add the Vertices for all your GameOjects, Colors and UVS


	glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), game_objectCoins[0]->getVertex());
	//glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), uvs);


	// Set Active Texture .... 32 GL_TEXTURE0 .... GL_TEXTURE31
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0); // 0 .... 31

	// Set the X, Y and Z offset (this allows for multiple cubes via different shaders)
	// Experiment with these values to change screen positions

	/*for (int i = 0; i < m_objectNum; i++)
	{
		glUniform1f(x_offsetID, game_object[i]->getPosition().x);
		glUniform1f(y_offsetID, game_object[i]->getPosition().y);
		glUniform1f(z_offsetID, game_object[i]->getPosition().z);

	}*/
	/*glUniform1f(x_offsetID, 0.00f);
	glUniform1f(y_offsetID, 0.00f);
	glUniform1f(z_offsetID, 0.00f);*/

	// Set pointers for each parameter (with appropriate starting positions)
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));

	// Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);


	// specify the 2d texture image
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_dataCoin				// Image Data
	);

	// Send transformation to shader mvp uniform [0][0] is start of array
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
	//draw gameObjects
	for (int i = 0; i < m_objectNum; i++)
	{
		glUniform1f(x_offsetID, game_objectCoins[i]->getPosition().x);
		glUniform1f(y_offsetID, game_objectCoins[i]->getPosition().y);
		glUniform1f(z_offsetID, game_objectCoins[i]->getPosition().z);

		// Draw Element Arrays
		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}

	// specify the 2d texture image
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_dataGround				// Image Data
	);

	//draw groundObjects
	for (int i = 0; i < m_groundCubes.size(); i++)
	{
		glUniform1f(x_offsetID, m_groundCubes.at(i).getPosition().x);
		glUniform1f(y_offsetID, m_groundCubes.at(i).getPosition().y);
		glUniform1f(z_offsetID, m_groundCubes.at(i).getPosition().z);

		// Draw Element Arrays
		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}
	//obstacles..
	for (int i = 0; i < m_objectNum; i++)
	{
		glUniform1f(x_offsetID, game_objectObstacle[i]->getPosition().x);
		glUniform1f(y_offsetID, game_objectObstacle[i]->getPosition().y);
		glUniform1f(z_offsetID, game_objectObstacle[i]->getPosition().z);

		// Draw Element Arrays
		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}

	// specify the 2d texture image
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_dataPlayer				// Image Data
	);
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpPlayer[0][0]);	//MY ADDITION
	//player stuff..
	glUniform1f(x_offsetID, m_playerObject->getPosition().x);
	glUniform1f(y_offsetID, m_playerObject->getPosition().y);
	glUniform1f(z_offsetID, m_playerObject->getPosition().z);
	// Draw Element Arrays
	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);

	window.display();

	// Disable Arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);

	// Unbind Buffers with 0 (Resets OpenGL States...important step)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Reset the Shader Program to Use
	glUseProgram(0);

	// Check for OpenGL Error code
	error = glGetError();
	if (error != GL_NO_ERROR) {
		DEBUG_MSG(error);
	}
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDetachShader(progID, vsid);	// Shader could be used with more than one progID
	glDetachShader(progID, fsid);	// ..
	glDeleteShader(vsid);			// Delete Vertex Shader
	glDeleteShader(fsid);			// Delete Fragment Shader
	glDeleteProgram(progID);		// Delete Shader
	glDeleteBuffers(1, &vbo);		// Delete Vertex Buffer
	glDeleteBuffers(1, &vib);		// Delete Vertex Index Buffer
	stbi_image_free(img_dataPlayer);		// Free image stbi_image_free(..)
}

std::pair<bool, bool> Game::checkCollision(vec3 t_playerPosition, vec3 t_objectPositon)
{
	// Collision x-axis?
	bool collisionX = (t_playerPosition.x - 1) + m_objectLength >= (t_objectPositon.x - 1) &&		// position - 1 beacuse initially the position is the center of object
		(t_objectPositon.x - 1) + m_objectLength >= (t_playerPosition.x - 1);
	// Collision y-axis?
	bool collisionY = (t_playerPosition.y - 1) + m_objectLength >= (t_objectPositon.y - 1) &&
		(t_objectPositon.y - 1) + m_objectLength >= (t_playerPosition.y - 1);
	// Collision only if on both axes
	return std::make_pair(collisionX, collisionY);
}