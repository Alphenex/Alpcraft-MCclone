#include "Shader.h"
#include <vector>

std::string GetFileContents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}



Shader::Shader(const char* vertShader, const char* fragShader)
{
    //const char* wow = R"END(
    //#version 330 core

    //layout(location = 0) in uint compVertex;

    //out vec2 texCoord;
    //out vec3 lightColor;

    //uniform mat4 cameraMatrix;
    //uniform vec3 chunkPosition;

    //vec2 TexCoordList[4] = vec2[4](
    //    vec2(0.0, 0.0),
    //    vec2(0.125, 0.0),
    //    vec2(0.125, 0.125),
    //    vec2(0.0, 0.125)
    //    );

    //void main()
    //{
    //    float x = float(compVertex & 0x3Fu) + chunkPosition.x;
    //    float y = float((compVertex & 0xFC0u) >> 6u) + chunkPosition.y;
    //    float z = float((compVertex & 0x3F000u) >> 12u) + chunkPosition.z;

    //    uint uvID = uint((compVertex & 0xC0000u) >> 18u);
    //    uint textureXID = uint((compVertex & 0x700000u) >> 20u);
    //    uint textureYID = uint((compVertex & 0x3800000u) >> 23u);

    //    float lightLVL = (compVertex & 0x3C000000u) >> 26u;
    //    float lightLVLC = lightLVL / 15;

    //    gl_Position = cameraMatrix * vec4(x, y, z, 1.0f);
    //    texCoord = TexCoordList[uvID] + vec2(textureXID, textureYID) * vec2(0.125, 0.125);
    //    lightColor = vec3(lightLVLC, lightLVLC, lightLVLC);

    //})END";

    //const char* wow2 = R"END(
    //#version 330 core

    //out vec4 FragColor;

    //in vec2 texCoord;
    //in vec3 lightColor;

    //uniform sampler2D tex0;

    //void main()
    //{
    //    FragColor = texture(tex0, texCoord) * vec4(lightColor, 1.0f);
    //})END";


	std::string vertString = GetFileContents(vertShader);
	std::string fragString = GetFileContents(fragShader);

	const char* vertCode = vertString.c_str();
	const char* fragCode = fragString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertCode, NULL);
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

        printf("%s", errorLog);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vertexShader); // Don't leak the shader.
        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, NULL);
    glCompileShader(fragmentShader);

    isCompiled = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

        printf("%s", errorLog);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fragmentShader); // Don't leak the shader.
        return;
    }

	ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}