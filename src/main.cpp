 #include <GLFW/glfw3.h>

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Fighting Game Input Buffering Demo", NULL, NULL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}