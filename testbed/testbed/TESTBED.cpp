#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>


/*
float * multiply_4d_matrices(float[] matrix1 float[] matrix2){
	
	return &matrix1;
}
*/

void multiply_4d_matrices(float * matrix1, float * matrix2)
{
  
}


void print_matrix(float matrix[]){
	printf ("%s","\n{");
	int count =0;
	for(int i =0; i<sizeof(matrix)*sizeof(float); i++){
		if(i % 4 == 0){
			printf ("%s","\n");
		}
		printf("%f%s",matrix[i],", ");
	}
	printf ("%s","\n}\n");
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
 }
}





int main () {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit ()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
  } 

	// uncomment these lines if on Apple OS X
  /*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf (stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent (window);
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  
  /* OTHER STUFF GOES HERE NEXT */
  float points[] = {
     0.0f,  0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	 -0.5f, -0.5f,  0.0f

  };



  float colours[] = {
	1.0f, 0.0f,  0.0f,
	0.0f, 1.0f,  0.0f,
	0.0f, 0.0f,  1.0f
	};

	float matrix[] = { 
	 1.0f, 0.0f, 0.0f, 0.0f, // first column 
	 0.0f, 1.0f, 0.0f, 0.0f, // second column 
	 0.0f, 0.0f, 1.0f, 0.0f, // third column 
	 0.0f, 0.0f, 0.0f, 1.0f // fourth column 
	};

    float rotation_matrix[] = { 
	 1.0f, 0.0f, 0.0f, 0.0f, // first column 
	 0.0f, 1.0f, 0.0f, 0.0f, // second column 
	 0.0f, 0.0f, 1.0f, 0.0f, // third column 
	 0.0f, 0.0f, 0.0f, 1.0f // fourth column 
	};

	//matRotationTotal = matRotationX * matRotationY * matRotationZ
	
	
    glm::mat4 glm_matrix;

	GLuint vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);

	GLuint colours_vbo = 0;
	glGenBuffers (1, &colours_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), colours, GL_STATIC_DRAW);


	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (1);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL); 


	const char* vertex_shader =
	"#version 330 core\n"
	"layout (location=0) in vec3 vp;"
	"layout (location=1) in vec3 vc;"
	"uniform mat4 glm_matrix;"
	"out vec3 fcolour;"
	"void main () {"
	 "fcolour = vc;"
	 "gl_Position = glm_matrix * vec4 (vp, 1.0);"
	"}";




	/* fragment shader simply sets each fragment (pixel-side piece of each 2d triangle) to a colour in RGBA
	(red blue green alpha) values from 0.0 to 1.0. one fragment = one fragment shader executed */

	const char* fragment_shader =
	"#version 330 core\n"
	"in vec3 fcolour;"
	"out vec3 frag_colour;"
	"void main () {"
	 "frag_colour = fcolour;"
	"}";




	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);
	printErrorLog("Vertex Shader",vs);
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);
	printErrorLog("Fragment Shader",fs);




	GLuint shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);

	// insert location binding code here
	glBindAttribLocation (shader_programme, 0, "vp");
	glBindAttribLocation (shader_programme, 1, "vc");


	glLinkProgram (shader_programme);

	int matrix_location = glGetUniformLocation (shader_programme, "matrix"); 
	glUseProgram (shader_programme); 
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);




	//main loop

	float speed = 1.0f; // move at 1 unit per second 
	float last_position = 0.0f;
	while (!glfwWindowShouldClose (window)) {

	 // add a timer for doing animation 
	 static double previous_seconds = glfwGetTime (); 
	 double current_seconds = glfwGetTime (); 
	 double elapsed_seconds = current_seconds - previous_seconds; 
	 previous_seconds = current_seconds; 
 
	 // reverse direction when going to far left or right 
	/* if (fabs(last_position) > 1.0f) { 
	 speed = -speed; 
	 }*/ 
 
	 
	

	 // Move the triangle
	if (glfwGetKey (window, GLFW_KEY_A)) {
		printf("A pressed");
		// update the matrix 
		 matrix[12] = elapsed_seconds * -speed + last_position; 
		 last_position = matrix[12]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); 
		 print_matrix(matrix);

	}

	if (glfwGetKey (window, GLFW_KEY_D)) {
		printf("D pressed");
		// update the matrix 
		 matrix[12] = elapsed_seconds * speed + last_position; 
		 last_position = matrix[12]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); 
		 print_matrix(matrix);

	}

	if (glfwGetKey (window, GLFW_KEY_W)) {
		printf("W pressed");
		// update the matrix 
		 matrix[13] = elapsed_seconds * speed + last_position; 
		 last_position = matrix[13]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); 
		 print_matrix(matrix);

	}

	if (glfwGetKey (window, GLFW_KEY_S)) {
		printf("S pressed");
		// update the matrix 
		 matrix[13] = elapsed_seconds * -speed + last_position; 
		 last_position = matrix[13]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); 
		 print_matrix(matrix);

	}
	//don't work
	if (glfwGetKey (window, GLFW_KEY_UP)) {
		printf("UP pressed");
		// update the matrix 
		int num = 14;
		 matrix[num] = elapsed_seconds * -speed + last_position; 
		 last_position = matrix[num]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix); 

		print_matrix(matrix);
	}

	//don't work
	if (glfwGetKey (window, GLFW_KEY_DOWN)) {
		printf("DOWN pressed");
		// update the matrix 
		 matrix[14] = elapsed_seconds * +speed + last_position; 
		 last_position = matrix[14]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
		 print_matrix(matrix);
	}

	
	if (glfwGetKey (window, GLFW_KEY_Q)) {
		printf("Q pressed");
		// update the matrix 
		 
		//set matrix to identity matrix
		/*matrix[0]= 1.0f;
		matrix[1]= 0.0f;
		matrix[2]= 0.0f;
		matrix[3]= 0.0f;
		matrix[4]= 0.0f;
		matrix[5]= 1.0f;
		matrix[6]= 0.0f;
		matrix[7]= 0.0f;
		matrix[8]= 0.0f;
		matrix[9]= 0.0f;
		matrix[10]= 1.0f;
		matrix[11]= 0.0f;
		matrix[12]= 0.0f;
		matrix[13]= 0.0f;
		matrix[14]= 0.0f;
		matrix[15]= 1.0f;
		*/

		//set rotation matrix to identity matrix
		 rotation_matrix[0] = rotation_matrix[0]+ cos(0.0174532925)*elapsed_seconds;
		 rotation_matrix[1] = rotation_matrix[1]+sin(0.0174532925)*elapsed_seconds;
		 rotation_matrix[4] = rotation_matrix[4]-sin(0.0174532925)*elapsed_seconds;
		 rotation_matrix[5] = rotation_matrix[5]+cos(0.0174532925)*elapsed_seconds;

		 //matrix = multiply_4d_matrices(matrix, rotation_matrix);
		 multiply_4d_matrices(matrix, rotation_matrix);

		 last_position = matrix[14]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
		 //printf("%f%s",matrix[14],"\n");
		 print_matrix(matrix);
	}


	if (glfwGetKey (window, GLFW_KEY_E)) {
		printf("E pressed");
		// update the matrix 
		 matrix[14] = elapsed_seconds * +speed + last_position; 
		 last_position = matrix[14]; 
		 glUseProgram (shader_programme); 
		 glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
		 print_matrix(matrix);
	}



	  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glUseProgram (shader_programme);
	  glBindVertexArray (vao);
	  glDrawArrays (GL_TRIANGLES, 0, 3);
	  glfwPollEvents ();
	  glfwSwapBuffers (window);
	  if (glfwGetKey (window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose (window, 1);
	  }
	}

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}