#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <string>

#define FGIBD_BUTTON_A 0
#define FGIBD_BUTTON_B 1
#define FGIBD_BUTTON_X 2
#define FGIBD_BUTTON_Y 3
#define FGIBD_BUTTON_L1 4
#define FGIBD_BUTTON_R1 5
#define FGIBD_DPAD_UP 14
#define FGIBD_DPAD_RIGHT 15
#define FGIBD_DPAD_DOWN 16
#define FGIBD_DPAD_LEFT 17

enum class EInputs : uint8_t
{
	Punch,
	Kick,
	HeavySlash,
	Slash,
	Dust,
	DPad1,
	DPad2,
	DPad3,
	DPad4,
	DPad5,
	DPad6,
	DPad7,
	DPad8,
	DPad9
};

struct Move
{
public:
	std::vector<std::vector<EInputs>> InputSequence;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << std::endl;
}

void joystick_callback(int jid, int event)
{
	if (event == GLFW_CONNECTED)
	{
		std::cout << "joystick was connected" << std::endl;
	}
	else if (event == GLFW_DISCONNECTED)
	{
		std::cout << "joystick was disconnected" << std::endl;
	}
}

void coutDpadKey(const std::vector<std::vector<EInputs>>& inputBuffers, EInputs input, HANDLE hConsole)
{
	if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), input)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
	else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), input)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
	else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), input)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
	else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), input)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
	else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), input)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
	else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), input)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
	else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), input)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
	else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), input)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
	else { SetConsoleTextAttribute(hConsole, 7);  std::cout << "-"; }

	SetConsoleTextAttribute(hConsole, 7);
}

bool moveDown(const std::vector<std::vector<EInputs>>& inputBuffers, const Move& move)
{
	for (size_t i = 0; i < inputBuffers.size(); i++)
	{
		// If we are out of the move input sequence without a fail, the move is succeeded.
		if (i >= move.InputSequence.size()) { return true; }
		
		for (EInputs sequenceInput : move.InputSequence[move.InputSequence.size() - 1 - i])
		{
			// If the input is not pressed, the move is failed.
			if (!std::count(inputBuffers[i].begin(), inputBuffers[i].end(), sequenceInput))
			{
				return false;
			}
		}
	}

	return false;
}

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		return -1;
	}

	Move quaterCircleSlash;
	quaterCircleSlash.InputSequence = { { EInputs::DPad2 }, { EInputs::DPad3 }, { EInputs::DPad6, EInputs::Slash } };
	//quaterCircleSlash.InputSequence.push_back({ EInputs::DPad6, EInputs::Slash });

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Fighting Game Input Buffering Demo", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwDefaultWindowHints();
	glfwShowWindow(window);

	glfwSetKeyCallback(window, key_callback);
	
	const int inputBufferCount = 8;
	std::vector<std::vector<EInputs>> inputBuffers(inputBufferCount);
	float framerate = 60.0;
	double currentTime = glfwGetTime();
	double lastTime = currentTime;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();

		if (currentTime - lastTime >= 1.0 / framerate)
		{
			
			lastTime = currentTime;

			int buttonCount;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

			int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
			if (present == 1)
			{
				// Shift back array, remove last element
				std::rotate(inputBuffers.begin(), inputBuffers.begin() + (inputBufferCount - 1), inputBuffers.end());

				// Fill input buffer
				inputBuffers[0].clear();
				// Basic buttons
				if (buttons[FGIBD_BUTTON_A] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Punch); }
				if (buttons[FGIBD_BUTTON_B] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Kick); }
				if (buttons[FGIBD_BUTTON_X] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::HeavySlash); }
				if (buttons[FGIBD_BUTTON_Y] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Slash); }
				if (buttons[FGIBD_BUTTON_R1] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Dust); }
				// Dpad
				if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS && buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad1); }
				else if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS && buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad3); }
				else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS && buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad7); }
				else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS && buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad9); }
				else if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad2); }
				else if (buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad4); }
				else if (buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad6); }
				else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad8); }
				else { inputBuffers[0].push_back(EInputs::DPad5); }

				system("cls");
				coutDpadKey(inputBuffers, EInputs::DPad7, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad8, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad9, hConsole);
				std::cout << "  P ";
				std::cout << "  K ";
				std::cout << "  HS";
				std::cout << "  S ";
				std::cout << "  D ";
				std::cout << std::endl;
				coutDpadKey(inputBuffers, EInputs::DPad4, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad5, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad6, hConsole);
				std::cout << "  "; coutDpadKey(inputBuffers, EInputs::Punch, hConsole); std::cout << " ";
				std::cout << "  "; coutDpadKey(inputBuffers, EInputs::Kick, hConsole); std::cout << " ";
				std::cout << "  "; coutDpadKey(inputBuffers, EInputs::HeavySlash, hConsole); std::cout << " ";
				std::cout << "  "; coutDpadKey(inputBuffers, EInputs::Slash, hConsole); std::cout << " ";
				std::cout << "  "; coutDpadKey(inputBuffers, EInputs::Dust, hConsole); std::cout << " ";
				std::cout << std::endl;
				coutDpadKey(inputBuffers, EInputs::DPad1, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad2, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad3, hConsole);
			}

			// Optimal quater circle forward
			/*if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), EInputs::Slash) &&
				std::count(inputBuffers[0].begin(), inputBuffers[0].end(), EInputs::DPad6) &&
				std::count(inputBuffers[1].begin(), inputBuffers[1].end(), EInputs::DPad3) &&
				std::count(inputBuffers[2].begin(), inputBuffers[2].end(), EInputs::DPad2) &&
				std::count(inputBuffers[3].begin(), inputBuffers[3].end(), EInputs::DPad5)) { glClearColor(0.0f, 1.0f, 0.0f, 1.0f); }*/
			if (moveDown(inputBuffers, quaterCircleSlash)) { glClearColor(0.0f, 1.0f, 0.0f, 1.0f); }
			else { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); }
			glClear(GL_COLOR_BUFFER_BIT); 
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();

	return 0;
}