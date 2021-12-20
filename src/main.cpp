#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <GLFW/glfw3.h>
#include <Windows.h>

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
}

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		return -1;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Fighting Game Input Buffering Demo", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);
	
	const int inputBufferCount = 8;
	std::vector<std::vector<EInputs>> inputBuffers(inputBufferCount);
	float framerate = 8.0;
	double currentTime = glfwGetTime();
	double lastTime = currentTime;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();

		if (currentTime - lastTime >= 1.0 / framerate)
		{
			lastTime = currentTime;

			// Input stuff
			glfwPollEvents();

			int buttonCount;
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

			int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
			if (present == 1)
			{
				// Shift back array, remove last element
				std::rotate(inputBuffers.begin(), inputBuffers.begin() + (inputBufferCount - 1), inputBuffers.end());

				// Fill input buffer
				inputBuffers[0].clear();
				/*if (buttons[FGIBD_BUTTON_A] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_A); }
				if (buttons[FGIBD_BUTTON_B] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_B); }
				if (buttons[FGIBD_BUTTON_X] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_X); }
				if (buttons[FGIBD_BUTTON_Y] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_Y); }
				if (buttons[FGIBD_BUTTON_L1] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_L1); }
				if (buttons[FGIBD_BUTTON_R1] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_R1); }*/

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
				std::cout << std::endl;
				coutDpadKey(inputBuffers, EInputs::DPad4, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad5, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad6, hConsole);
				std::cout << std::endl;
				coutDpadKey(inputBuffers, EInputs::DPad1, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad2, hConsole);
				coutDpadKey(inputBuffers, EInputs::DPad3, hConsole);

				// PLZ DONT JUDGE...
				/*if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_UP)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7);  std::cout << "-"; }

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_UP) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }
				std::cout << std::endl;

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 12);std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }
				
				if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (inputBuffers[0].size() == 0) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }
				std::cout << std::endl;

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_LEFT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }

				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_DOWN)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }
				
				if (std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 12); std::cout << "1"; }
				else if (std::count(inputBuffers[1].begin(), inputBuffers[1].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 13); std::cout << "2"; }
				else if (std::count(inputBuffers[2].begin(), inputBuffers[2].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 14); std::cout << "3"; }
				else if (std::count(inputBuffers[3].begin(), inputBuffers[3].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 15); std::cout << "4"; }
				else if (std::count(inputBuffers[4].begin(), inputBuffers[4].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 16); std::cout << "5"; }
				else if (std::count(inputBuffers[5].begin(), inputBuffers[5].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 17); std::cout << "6"; }
				else if (std::count(inputBuffers[6].begin(), inputBuffers[6].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 18); std::cout << "7"; }
				else if (std::count(inputBuffers[7].begin(), inputBuffers[7].end(), FGIBD_DPAD_DOWN) && std::count(inputBuffers[0].begin(), inputBuffers[0].end(), FGIBD_DPAD_RIGHT)) { SetConsoleTextAttribute(hConsole, 19); std::cout << "8"; }
				else { SetConsoleTextAttribute(hConsole, 7); std::cout << "-"; }*/
			}
		}
	}

	glfwTerminate();

	return 0;
}