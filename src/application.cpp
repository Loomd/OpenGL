
#include <iostream>
#include <string>
#include <GL/glew.h>        // OpenGL Extension Wrangler Library. Provides access to modern OpenGL functions beyond OpenGL 1.1.
#include <GLFW/glfw3.h>     // GLFW library. Handles window creation, input, and OpenGL context management.

// Vertex Shader Source Code
const char* vertexShader = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader Source Code
const char* fragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

// Yellow Fragment Shader Source Code
const char* yellowFragmentShader = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n";

// Function to Compile Individual Shaders
// Provide OpenGL with a shader source code (text), compile the shader, and return a unique ID.
static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
    unsigned int id = glCreateShader(type); // Provide the type of shader to create.
    const char* src = source.c_str();       // Provide the actual code.
    glShaderSource(id, 1, &src, nullptr);   // Associate the source code with the shader object.
    glCompileShader(id);                    // Compile the shader.

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader.\n"
            << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;  // Return the compiled shader.
}

// Function to Create Shader Program
// Combine and link compiled vertex and fragment shaders into a single shader program.
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram(); // Create a program and return its ID.
    unsigned int vertShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertShader); // Attach the vertex shader to the program.
    glAttachShader(program, fragShader); // Attach the fragment shader to the program.
    glLinkProgram(program);              // Link the shaders into a complete program.
    glValidateProgram(program);          // Validate the program.

    // Clean up compiled shaders as they are no longer needed after linking.
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;  // Return the shader program ID.
}

bool isYellow = false; // Toggle between yellow and orange
unsigned int shaderProgram, yellowShaderProgram;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        isYellow = !isYellow; //Flip the state, it was toggled.
        std::cout << "Spacebar pressed!" << std::endl;
        //add more here, update logic
        if (isYellow)
        {
            glUseProgram(yellowShaderProgram);
        }
        else
        {
            glUseProgram(shaderProgram);
        }

    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Left mouse button clicked!" << std::endl;
        //updat logic for mouse click.
    }
}

int main() 
{

    // The GLFWwindow object represents the window and its associated OpenGL context.
    GLFWwindow* window;
    // Initialize GLFW library. This must be called before any other GLFW function.
    if (!glfwInit())
        return -1;
    // Create a GLFW window with specified dimensions (width=640, height=480) and a title ("Triangle!").
    window = glfwCreateWindow(640, 480, "Triangle!", NULL, NULL);
    if (!window) 
    {
        glfwTerminate(); // Terminate GLFW and exit with an error if window creation fails.
        return -1;
    }
    glfwMakeContextCurrent(window); // Make the window's context current.
    // Initialize GLEW. This library manages OpenGL function pointers and must
    // be initialized after creating a valid OpenGL context.
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR: Failed to initialize GLEW!" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


    // Triangle Data
    float triangle_one[6] = {
        -1.0f, 0.0f, 
        -0.9f, 0.5f, 
        -0.8f, 0.0f
    };
    float triangle_two[6] = {
        0.0f, 0.0f, 
        0.1f, 0.5f, 
        0.2f, 0.0f
    };
    // VAO and VBO for Triangle One
    unsigned int vertexArrayObject1, vertexBufferObject1;
    glGenVertexArrays(1, &vertexArrayObject1); // Generate a Vertex Array Object.
    glGenBuffers(1, &vertexBufferObject1);     // Generate a Buffer Object.
    glBindVertexArray(vertexArrayObject1);     // Bind the VAO to store configuration.
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject1); // Bind the VBO to the ARRAY_BUFFER target.
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), triangle_one, GL_STATIC_DRAW); // Upload triangle data to GPU.
    // Specify how the vertex data is organized in the buffer.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0.
    // Unbind the VBO and VAO to avoid accidental modifications.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // VAO and VBO for Triangle Two
    unsigned int vao2, buffer2;
    glGenVertexArrays(1, &vao2);
    glGenBuffers(1, &buffer2);
    glBindVertexArray(vao2);
    glBindBuffer(GL_ARRAY_BUFFER, buffer2);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), triangle_two, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create and Use Shader Program
    yellowShaderProgram = CreateShader(vertexShader, yellowFragmentShader);
    shaderProgram = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    
    //Register keyCallback() and mouseButtonCallback() 
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Rendering Loop: Continues until glfwWindowShouldClose() returns true.
    while (!glfwWindowShouldClose(window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with a preset color.

        // Draw Triangle One
        glBindVertexArray(vertexArrayObject1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Draw Triangle Two
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // Swap front and back buffers.
        glfwPollEvents();        // Poll for and process events.
    }

    // Clean up and release resources before exiting the program.
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vertexArrayObject1);
    glDeleteBuffers(1, &vertexBufferObject1);
    glDeleteVertexArrays(1, &vao2);
    glDeleteBuffers(1, &buffer2);
    glfwTerminate();

    return 0;
}