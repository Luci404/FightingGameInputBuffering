#include <iostream>
#include <array>
#include <GLFW/glfw3.h>

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
			
			int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
			if (present == 1)
			{
				int buttonCount;
				const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

				if (buttonCount >= 0 && buttons[0] == GLFW_PRESS) { std::cout << "A" << std::endl; }
				else if (buttonCount >= 1 && buttons[1] == GLFW_PRESS) { std::cout << "B" << std::endl; }
				else if (buttonCount >= 2 && buttons[2] == GLFW_PRESS) { std::cout << "X" << std::endl; }
				else if (buttonCount >= 3 && buttons[3] == GLFW_PRESS) { std::cout << "Y" << std::endl; }
				else if (buttonCount >= 4 && buttons[4] == GLFW_PRESS) { std::cout << "L1" << std::endl; }
				else if (buttonCount >= 5 && buttons[5] == GLFW_PRESS) { std::cout << "R1" << std::endl; }
				else if (buttonCount >= 14 && buttons[14] == GLFW_PRESS) { std::cout << "UP" << std::endl; }
				else if (buttonCount >= 15 && buttons[15] == GLFW_PRESS) { std::cout << "RIGHT" << std::endl; }
				else if (buttonCount >= 16 && buttons[16] == GLFW_PRESS) { std::cout << "DOWN" << std::endl; }
				else if (buttonCount >= 17 && buttons[17] == GLFW_PRESS) { std::cout << "LEFT" << std::endl; }
			}
		}
	}

	glfwTerminate();

	return 0;
}