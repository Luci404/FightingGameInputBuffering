#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <GLFW/glfw3.h>

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

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Fighting Game Input Buffering Demo", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);
	
	const int inputBufferCount = 8;
	std::vector<std::vector<int>> inputBuffers(inputBufferCount);
	float framerate = 60.0;
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
				std::rotate(inputBuffers.begin(), inputBuffers.begin() + 1, inputBuffers.end());

				// Fill input buffer
				inputBuffers[0].clear();
				if (buttons[FGIBD_BUTTON_A] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_A); }
				if (buttons[FGIBD_BUTTON_B] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_B); }
				if (buttons[FGIBD_BUTTON_X] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_X); }
				if (buttons[FGIBD_BUTTON_Y] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_Y); }
				if (buttons[FGIBD_BUTTON_L1] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_L1); }
				if (buttons[FGIBD_BUTTON_R1] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_BUTTON_R1); }
				if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_DPAD_UP); }
				if (buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_DPAD_RIGHT); }
				if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_DPAD_DOWN); }
				if (buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(FGIBD_DPAD_LEFT); }
				
				// Debug print
				for (std::vector<int> inputBuffer : inputBuffers)
				{
					std::cout << " - ";
					for (int key : inputBuffer)
					{
						std::cout << key;
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
	}

	glfwTerminate();

	return 0;
}