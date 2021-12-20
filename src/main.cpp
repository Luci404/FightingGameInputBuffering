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

#define FGIBD_CONFIG_INPUT_BUFFER_COUNT 30

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

typedef std::vector<std::vector<EInputs>> InputSequence;

// Circular buffer containg frame input data from most recent to oldest.
InputSequence inputBuffers(FGIBD_CONFIG_INPUT_BUFFER_COUNT);

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

void FlushInputBuffers()
{
	for (std::vector<EInputs>& frameInputs : inputBuffers)
	{
		frameInputs = {};
	}
}

bool IsSequenceInInputBuffer(const InputSequence& inputSequence)
{
	/*for (size_t i = 0; i < inputBuffers.size(); i++)
	{
		// If we are out of the move input sequence without a fail, the move is succeeded.
		if (i >= inputSequence.size()) { return true; }

		for (EInputs sequenceInput : inputSequence[inputSequence.size() - 1 - i])
		{
			// If the input is not pressed, the move is failed.
			if (!std::count(inputBuffers[i].begin(), inputBuffers[i].end(), sequenceInput))
			{
				return false;
			}
		}
	}*/

	// Iterate through the input buffers, starting with the current frame and going back in time, and check if the inputs match inputSequence.
	uint8_t currentFrame = 0;
	for (size_t i = 0; i < inputBuffers.size(); i++)
	{
		// If the number of correct frames is equal to the number of frames in the sequence, the sequence was correctly performed.
		if (currentFrame >= inputSequence.size())
		{
			return true;
		}

		// Loop over the current frame, if it matches the current frame, or the frame before it, in the input sequence, continue, if not, the sequence was incorrectly performed.
		for (EInputs input : inputSequence[inputSequence.size() - 1 - currentFrame])
		{
			if (std::count(inputBuffers[i].begin(), inputBuffers[i].end(), input) > 0)
			{
				currentFrame++;
				continue;
			}
			else
			{
				for (EInputs input2 : inputSequence[inputSequence.size() - 1 - currentFrame])
				{
					if (i > 0 && std::count(inputBuffers[i-1].begin(), inputBuffers[i-1].end(), input2) > 0)
					{
						continue;
					}
					else
					{
						return false;
					}
				}
			}
		}

	}

	return false;
}

bool TryPerformMove(const InputSequence& inputSequence)
{
	if (IsSequenceInInputBuffer(inputSequence))
	{
		FlushInputBuffers();
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		return -1;
	}

	// Moves
	InputSequence quaterCircleForwardSlashInputSequence = { { EInputs::DPad5 }, { EInputs::DPad2 }, { EInputs::DPad3 } };
	InputSequence quaterCircleBackwardSlashInputSequence = { { EInputs::DPad5 }, { EInputs::DPad2 }, { EInputs::DPad1 }, { EInputs::DPad4, EInputs::Slash } };

	// Console handle used for console input widget.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	// GLFW stuff
	GLFWwindow* window = glfwCreateWindow(640, 480, "Fighting Game Input Buffering Demo", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwDefaultWindowHints();
	glfwShowWindow(window);

	// Framerate limit data
	float framerate = 60.0;
	double currentTime = glfwGetTime();
	double lastTime = currentTime;

	// Run loop
	while (!glfwWindowShouldClose(window))
	{
		// Framerate limit, the thread should ideally be put to sleep; but an if statement work for now...
		currentTime = glfwGetTime();
		if (currentTime - lastTime >= 1.0 / framerate)
		{
			lastTime = currentTime;

			// Handle Inputs
			{
				glfwPollEvents();

				int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
				if (present == 1)
				{
					// Get pressed buttons from GLFW
					int buttonCount;
					const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

					// Shift the array; this is not the optimal solution, it should be using some sort of current index variable that is incremented each frame, but this is easier... soooooo....
					std::rotate(inputBuffers.begin(), inputBuffers.begin() + (inputBuffers.size() - 1), inputBuffers.end());

					// Clear up current frame history, since the buffers are reused
					inputBuffers[0].clear();

					// Add buttons inputs
					if (buttons[FGIBD_BUTTON_A] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Punch); }
					if (buttons[FGIBD_BUTTON_B] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Kick); }
					if (buttons[FGIBD_BUTTON_X] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::HeavySlash); }
					if (buttons[FGIBD_BUTTON_Y] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Slash); }
					if (buttons[FGIBD_BUTTON_R1] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::Dust); }

					// Add dpad inputs
					if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS && buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad1); }
					else if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS && buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad3); }
					else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS && buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad7); }
					else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS && buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad9); }
					else if (buttons[FGIBD_DPAD_DOWN] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad2); }
					else if (buttons[FGIBD_DPAD_LEFT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad4); }
					else if (buttons[FGIBD_DPAD_RIGHT] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad6); }
					else if (buttons[FGIBD_DPAD_UP] == GLFW_PRESS) { inputBuffers[0].push_back(EInputs::DPad8); }
					else { inputBuffers[0].push_back(EInputs::DPad5); }
				}
			}

			// Print input info to console, this should be rendererd using openGL in the future.
			{
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

			// Render stuff to the window.
			{
				if (TryPerformMove(quaterCircleForwardSlashInputSequence)) { glClearColor(0.0f, 1.0f, 0.0f, 1.0f); }
				else if (TryPerformMove(quaterCircleBackwardSlashInputSequence)) { glClearColor(0.0f, 1.0f, 0.0f, 1.0f); }
				else { glClearColor(0.0f, 0.0f, 0.0f, 1.0f); }
				glClear(GL_COLOR_BUFFER_BIT);
				glfwSwapBuffers(window);
			}
		}
	}

	glfwTerminate();

	return 0;
}