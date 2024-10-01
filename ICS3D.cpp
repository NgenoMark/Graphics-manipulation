#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 pos;
    void main() {
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    }
)";

// Fragment Shader
static const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 color;
    void main() {
        color = vec4(0.0, 1.0, 0.0, 1.0);  // Green color
    }
)";

void createTriangle() {
    // Define the vertices of the triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Bottom left
         0.5f, -0.5f, 0.0f,  // Bottom right
         0.0f,  0.5f, 0.0f   // Top center
    };

    // Create a Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create a Vertex Buffer Object (VBO) and copy the vertex data to it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link vertex data with the shader's "pos" attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void compileShader() {
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link the shaders into a program
    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    // Cleanup shaders as they're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        printf("GLFW initialization failed!\n");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Compile shaders and create a triangle
    compileShader();
    createTriangle();

    // Loop until window is closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Get + Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);  // Red background
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program and draw the triangle
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap buffers
        glfwSwapBuffers(mainWindow);
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
