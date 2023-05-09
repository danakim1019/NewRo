#include"BuiltInSphereObj.h"

BuiltInSphere::BuiltInSphere()
{
	radius = 2.0;
	slices = 60;
	stacks = 60;

	Ka = glm::vec3(1, 0, 0);
	Kd = glm::vec3(1, 0, 0);
	Ks = glm::vec3(1, 1, 1);
	shiness = 10;

	La = glm::vec3(0.9, 0.9, 0.9);
	Ld = glm::vec3(0.6, 0.6, 0.6);
	Ls = glm::vec3(0.6, 0.6, 0.6);

	lightPos = glm::vec4(0, 0, 300, 1);

	name = "Sphere";
	objectType = "Sphere";
	shaderType = "Shadow";

	setup();
}


BuiltInSphere::~BuiltInSphere()
{
}


//set rad = 1.0  sl, st = 60.
BuiltInSphere::BuiltInSphere(float rad, GLuint sl, GLuint st, int type) :
	radius(rad), slices(sl), stacks(st)
{
	setup();
}

void BuiltInSphere::setup() {
	nVerts = (slices + 1) * (stacks + 1);  //the number of vertices
	elements = (slices * 2 * (stacks - 1)) * 3;
	// Vertices
	float* v = new float[3 * nVerts];
	// Normals
	float* n = new float[3 * nVerts];
	// Tex coords
	float* tex = new float[2 * nVerts];
	// Elements
	unsigned int* el = new unsigned int[elements];

	// Generate the vertex data : this function fill all data into the arrays.
	generateVerts(v, n, tex, el);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	shaderProgram = new ShaderProgram();
	//create vao, vbo, ibo here...
	if (shaderType == "Diffuse")			//diffuse Light
	{
		shaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");

		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");
		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("location");

		shaderProgram->addUniform("model");
		shaderProgram->addUniform("view");
		shaderProgram->addUniform("projection");
	}
	else if (shaderType == "Shadow") {
		shaderProgram->initFromFiles("Shader/shadowNT.vert", "Shader/shadowNT.frag");

		shaderProgram->addAttribute("VertexPosition");
		shaderProgram->addAttribute("VertexNormal");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.Intensity");

		shaderProgram->addUniform("isShadow");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("ProjectionMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("ModelMatrix");
		shaderProgram->addUniform("MVP");

		shaderProgram->addUniform("lightSpaceMatrix");
		shaderProgram->addUniform("shadowMap");
		shaderProgram->addUniform("shadowType");

	}

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nVerts, v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//create vbo for colors
	glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nVerts, n, GL_STATIC_DRAW);	//VBO에 데이터 저장
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements, el, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void BuiltInSphere::RenderPicking() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BuiltInSphere::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec4 lightPos(lightPosition, 1.0);

	//2D 상의 좌표 저장
	float shininess = m_mat->getShiness();

	shaderProgram->use();

	if (shaderType=="Diffuse")
	{
		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view *lightPos));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

		glUniformMatrix4fv(shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	}
	else if (shaderType == "Shadow") {
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, m_mat->getKa());
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, m_mat->getKd());
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, m_mat->getKs());
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
		glUniform3fv(shaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

		glUniformMatrix4fv(shaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(shaderProgram->uniform("shadowMap"), 0);

		glUniform1i(shaderProgram->uniform("isShadow"), shadow->isShadow);
		glUniform1i(shaderProgram->uniform("shadowType"), shadow->shadowType);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->shadowGLuint);

	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shaderProgram->disable();
	//glDrawArraysInstanced(GL_TRIANGLES, 0, nVerts * 3, 10);

}

void BuiltInSphere::generateVerts(float* verts, float* norms, float* tex,
	unsigned int* el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;



	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;

	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}

	}
}

int BuiltInSphere::getVertexArrayHandle()
{
	return this->VAO;
}