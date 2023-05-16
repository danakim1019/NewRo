#include"BuiltInSphereObj.h"

BuiltInSphere::BuiltInSphere() : mRadius(2.0), mSlices(60),mStacks(60)
{
	mName = "Sphere";
	mObjectType = "Sphere";
	mShaderType = "Shadow";

	setup();
}


BuiltInSphere::~BuiltInSphere()
{
}


//set rad = 1.0  sl, st = 60.
BuiltInSphere::BuiltInSphere(float rad, GLuint sl, GLuint st, int type) :
	mRadius(rad), mSlices(sl), mStacks(st)
{
	setup();
}

void BuiltInSphere::setup() {
	mNumVerts = (mSlices + 1) * (mStacks + 1);  //the number of vertices
	mElements = (mSlices * 2 * (mStacks - 1)) * 3;
	// Vertices
	float* v = new float[3 * mNumVerts];
	// Normals
	float* n = new float[3 * mNumVerts];
	// Tex coords
	float* tex = new float[2 * mNumVerts];
	// Elements
	unsigned int* el = new unsigned int[mElements];

	// Generate the vertex data : this function fill all data into the arrays.
	generateVerts(v, n, tex, el);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mShaderProgram = new ShaderProgram();
	//create vao, vbo, ibo here...
	if (mShaderType == "Diffuse")
	{
		//load shaders
		mShaderProgram->initFromFiles("Shader/Diffuse.vert", "Shader/Diffuse.frag");
	}
	else if (mShaderType == "Shadow") {
		mShaderProgram->initFromFiles("Shader/shadow.vert", "Shader/shadow.frag");
	}

	mShaderProgram->addAttribute("VertexPosition");
	mShaderProgram->addAttribute("VertexNormal");

	mShaderProgram->addUniformGroup(mShaderType);

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mNumVerts, v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//create vbo for colors
	glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mNumVerts, n, GL_STATIC_DRAW);	//VBO에 데이터 저장
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mElements, el, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void BuiltInSphere::RenderPicking() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mElements, GL_UNSIGNED_INT, 0);
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
	float shininess = mMat->getShiness();

	mShaderProgram->use();

	if (mShaderType=="Diffuse")
	{
		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view *lightPos));
		glUniform3fv(mShaderProgram->uniform("Light.La"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		glUniform3fv(mShaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
		glUniform3fv(mShaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

		glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, mMat->getKa());
		glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, mMat->getKd());
		glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, mMat->getKs());
		glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(mShaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(location));

		glUniformMatrix4fv(mShaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(mShaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	}
	else if (mShaderType == "Shadow") {
		glUniformMatrix4fv(mShaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix4fv(mShaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix3fv(mShaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));

		glUniform3fv(mShaderProgram->uniform("Material.Ka"), 1, mMat->getKa());
		glUniform3fv(mShaderProgram->uniform("Material.Kd"), 1, mMat->getKd());
		glUniform3fv(mShaderProgram->uniform("Material.Ks"), 1, mMat->getKs());
		glUniform1fv(mShaderProgram->uniform("Material.Shiness"), 1, &shininess);

		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(view * glm::vec4(lightPosition, 1.0)));
		glUniform3fv(mShaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));

		glUniformMatrix4fv(mShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(mShaderProgram->uniform("shadowMap"), 0);

		glUniform1i(mShaderProgram->uniform("isShadow"), shadow->bIsShadow);
		glUniform1i(mShaderProgram->uniform("shadowType"), shadow->mShadowType);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);

	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mElements, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	mShaderProgram->disable();
	//glDrawArraysInstanced(GL_TRIANGLES, 0, nVerts * 3, 10);

}

void BuiltInSphere::generateVerts(float* verts, float* norms, float* tex,
	unsigned int* el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / mSlices;
	GLfloat phiFac = glm::pi<float>() / mStacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;



	for (GLuint i = 0; i <= mSlices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / mSlices;
		for (GLuint j = 0; j <= mStacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / mStacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = mRadius * nx; verts[idx + 1] = mRadius * ny; verts[idx + 2] = mRadius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;

	for (GLuint i = 0; i < mSlices; i++) {
		GLuint stackStart = i * (mSlices + 1);
		GLuint nextStackStart = (i + 1) * (mStacks + 1);
		for (GLuint j = 0; j < mStacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == mStacks - 1) {
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