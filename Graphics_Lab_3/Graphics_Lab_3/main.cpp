#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>


class Transform
{
	public:
		// Default constructor:
		Transform();

		// Compute and return the transformation matrix:
		const glm::mat4& getMatrix();

		// Modifiy the transformation:
		void setLocation(const glm::vec3& p_location);
		void setRotationAxes(const glm::vec3& p_rotation_axes);
		void setRotationValue(float p_rotation_value, bool p_as_degrees = true);
		void setScale(const glm::vec3& p_scale);

		// Getter functions:
		const glm::vec3& getLocation();
		const glm::vec3& getRotationAxes();
		float getRotationValue(bool p_as_degrees = true);
		const glm::vec3& getScale();
	private:
		glm::vec3 m_location,
				  m_rotation_axes,
				  m_scale;

		float m_rotation_value;
		glm::mat4 m_cached_transform_matrix;
		bool m_was_modified;
};

Transform::Transform() :
	m_location(0.f),
	m_rotation_axes(1.f, 0.f, 0.f),
	m_scale(1.f),
	m_rotation_value(0.f),
	m_cached_transform_matrix(1.f),
	m_was_modified(true)
{
	// Default initialisation...
}


const glm::mat4& Transform::getMatrix()
{
	if (m_was_modified)
	{
		m_cached_transform_matrix = glm::translate(m_location) *
									glm::rotate(m_rotation_value, m_rotation_axes) *
									glm::scale(m_scale);
		m_was_modified = false;
	}

	return m_cached_transform_matrix;
}


void Transform::setLocation(const glm::vec3& p_location)
{
	m_location = p_location;
	m_was_modified = true;
}


void Transform::setRotationAxes(const glm::vec3& p_rotation_axes)
{
	m_rotation_axes = p_rotation_axes;
	m_was_modified = true;
}


void Transform::setRotationValue(float p_rotation_value, bool p_as_degrees)
{
	m_rotation_value = ((p_as_degrees) ? glm::radians(p_rotation_value) : p_rotation_value);
	m_was_modified = true;
}


void Transform::setScale(const glm::vec3& p_scale)
{
	m_scale = p_scale;
	m_was_modified = true;
}


const glm::vec3& Transform::getLocation()
{
	return m_location;
}


const glm::vec3& Transform::getRotationAxes()
{
	return m_rotation_axes;
}


float Transform::getRotationValue(bool p_as_degrees)
{
	return ((p_as_degrees) ? glm::degrees(m_rotation_value) : m_rotation_value);
}


const glm::vec3& Transform::getScale()
{
	return m_scale;
}



/* a function load a BMP image and create a texture from it */
GLuint loadBMP( char* filepath){


	printf("filepath ='");
	for (int j = 0; j < sizeof(filepath); j++ )
	{

        printf("%c", filepath[j] );
    }
	printf("'\n");

	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(filepath, "rb");

	if (!file){
		printf("Image %s could not be opened\n", filepath); return 0;
	}else{
		printf("Image %s was successfully opened\n", filepath);
	}
	
    

	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
	}

	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	printf("Texture loaded\n");
	return textureID;

}





//Checks compilation errors for shaders
void printErrorLog(char *title,GLuint shader)
{
 printf ("%s%s%s \n", "printErrorLog called for '",title,"':");
GLint status;
 glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
 if(status==GL_FALSE)
 { 
                char buffer[512]; 
                glGetShaderInfoLog(shader,512,NULL,buffer); 
                printf("%s:\n %s\n",title,buffer);
 }else{
	printf ("  %s\n", "No error.");
 }
}


int main () {

	
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: opening OS window\n");
		return 1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION); /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	glDepthFunc(GL_LESS); /*depth-testing interprets a smaller value as "closer"*/

	GLfloat points[] = {
		-0.1f, -0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f,
		0.1f, -0.1f, 0.0f,
		-0.1f, 0.1f, 0.0f,
		0.1f, 0.1f, 0.0f,
		0.1f, -0.1f, 0.0f
	};

	GLuint vao;
	GLuint vbo;
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	/* the UV values for telling the shader where the texture is meant to go */
	GLfloat UV[] = {
		0.0f, 0.0f, // bottom left
		0.0f, 1.0f, // upper left
		1.0f, 0.0f, //  bottom right
		0.0f, 1.0f, //  upper left
		1.0f, 1.0f, // upper right
		1.0f, 0.0f // bottom right
	};




	//glm::mat4 glm_matrix(1.f);


	GLuint uvVBO;
	glGenBuffers(1, &uvVBO);

	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UV), UV, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint Texture = loadBMP("texture.bmp");

	const char* vertex_shader =
		"#version 330 core\n" 
		"layout(location = 0) in vec3 vp;"
		"layout(location = 1) in vec2 vertexUV;"
		"out vec2 UV;"
		"uniform mat4 my_transform;"
		"void main () {"
		"  gl_Position =  my_transform * vec4 (vp * vec3(5.0f), 1.0);"
		"  UV = vertexUV;"
		"}";
			//"  gl_Position =  gl_Position * matrix;"
	//"uniform mat4 MVP;"

	const char* fragment_shader =
		"#version 330 core\n"
		"in vec2 UV;"
		"out vec3 colour;"
		"uniform sampler2D myTextureSampler;"
		"void main () {"
		"  colour = texture(myTextureSampler, UV).rgb;"
		"}";

	GLuint vs, fs;
	GLuint shader_programme;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	printErrorLog("Vertex Shader",vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	printErrorLog("Fragment Shader",fs);
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);


	// Setup transformations:
	Transform obj_trans;
	obj_trans.setRotationAxes(glm::vec3(1.f, 1.f, 1.f));
	double last_frame = glfwGetTime(),
		   current_frame = 0.f,
		   delta_time = 0.f;
	float rval=0;
	while (!glfwWindowShouldClose(window)) {
		// Control delta time:
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		//rotate the square
		rval += 1000.f * static_cast<float>(delta_time);
		if (rval >= 1000*360.f) rval = 0.f;
		obj_trans.setRotationValue(rval);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		GLint uniform_pos = glGetUniformLocation(shader_programme, "my_transform"); // or whatever you name the uniform shader variable
		glUniformMatrix4fv(uniform_pos, 1, GL_FALSE, glm::value_ptr(obj_trans.getMatrix()));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;

}