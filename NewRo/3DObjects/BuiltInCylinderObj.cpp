#include"BuiltInCylinderObj.h"

const float pi = 3.14159265358979323846;
const float twoPi = 2.0 * pi;

BuiltInCylinder::BuiltInCylinder() :mRadius(1.0f), mLength(2.0f), mRings(10), mSlices(30) {

	mName = "Cylinder";
	mObjectType = "Cylinder";
	mShaderType = "Shadow";

	setup();

}

BuiltInCylinder::BuiltInCylinder(float radius, float length, int rings, int slices) :mRadius(radius), mLength(length), mRings(rings), mSlices(slices)
{
	mName = "Cylinder";
	mObjectType = "Cylinder";
	mShaderType = "Shadow";

	setup();
}

void BuiltInCylinder::generateVertexData()
{
	mFaces = mSlices * mRings;           // Number of "rectangular" faces
	mNumVerts = (mSlices + 1) * mRings  // Sides
		+ 2 * (mSlices + 1);       // 2 x endcaps

	// Resize vector to hold our data
	mVertexPositions.resize(3 * mNumVerts);
	mVertexNormals.resize(3 * mNumVerts);
	mVertexIndices.resize(6 * mFaces + 2 * 3 * mSlices);

	// The side of the cylinder
	const float dTheta = twoPi / static_cast<float>(mSlices);
	const float dy = mLength / static_cast<float>(mRings - 1);

	// Iterate over heights (rings)
	mIndex = 0;

	for (int ring = 0; ring < mRings; ++ring)
	{
		const float y = -mLength / 2.0f + static_cast<float>(ring) * dy;

		// Iterate over slices (segments in a ring)
		for (int slice = 0; slice < mSlices + 1; ++slice)
		{
			float theta = static_cast<float>(slice) * dTheta;
			float cosTheta = cosf(theta);
			float sinTheta = sinf(theta);

			mVertexPositions[mIndex] = mRadius * cosTheta;
			mVertexPositions[mIndex + 1] = y;
			mVertexPositions[mIndex + 2] = mRadius * sinTheta;

			mVertexNormals[mIndex] = cosTheta;
			mVertexNormals[mIndex + 1] = 0.0f;
			mVertexNormals[mIndex + 2] = sinTheta;

			mIndex += 3;
		}
	}

	int elIndex = 0;
	for (int i = 0; i < mRings - 1; ++i)
	{
		const int ringStartIndex = i * (mSlices + 1);
		const int nextRingStartIndex = (i + 1) * (mSlices + 1);

		for (int j = 0; j < mSlices; ++j)
		{
			// Split the quad into two triangles
			mVertexIndices[elIndex] = ringStartIndex + j;
			mVertexIndices[elIndex + 1] = ringStartIndex + j + 1;
			mVertexIndices[elIndex + 2] = nextRingStartIndex + j;
			mVertexIndices[elIndex + 3] = nextRingStartIndex + j;
			mVertexIndices[elIndex + 4] = ringStartIndex + j + 1;
			mVertexIndices[elIndex + 5] = nextRingStartIndex + j + 1;

			elIndex += 6;
		}
	}

	// Generate the vertex data and indices for the two end caps
	generateEndCapVertexData(mLength / 2.0f, mIndex, elIndex);
	generateEndCapVertexData(-mLength / 2.0f, mIndex, elIndex);
}

void BuiltInCylinder::generateEndCapVertexData(float y, int& index, int& elIndex)
{
	// Make a note of the vertex index for the center of the end cap
	int endCapStartIndex = index / 3;

	mVertexPositions[index] = 0.0f;
	mVertexPositions[index + 1] = y;
	mVertexPositions[index + 2] = 0.0f;

	mVertexNormals[index] = 0.0f;
	mVertexNormals[index + 1] = (y >= 0.0f) ? 1.0f : -1.0;
	mVertexNormals[index + 2] = 0.0f;

	index += 3;

	const float dTheta = twoPi / static_cast<float>(mSlices);
	for (int slice = 0; slice < mSlices; ++slice)
	{
		const float theta = static_cast<float>(slice) * dTheta;
		const float cosTheta = cosf(theta);
		const float sinTheta = sinf(theta);

		mVertexPositions[index] = mRadius * cosTheta;
		mVertexPositions[index + 1] = y;
		mVertexPositions[index + 2] = mRadius * sinTheta;

		mVertexNormals[index] = 0.0f;
		mVertexNormals[index + 1] = (y >= 0.0f) ? 1.0f : -1.0;
		mVertexNormals[index + 2] = 0.0f;

		index += 3;
	}

	for (int i = 0; i < mSlices; ++i)
	{
		// Use center point and i+1, and i+2 vertices to create triangles
		if (i != mSlices - 1)
		{
			mVertexIndices[elIndex] = endCapStartIndex;
			mVertexIndices[elIndex + 1] = endCapStartIndex + i + 1;
			mVertexIndices[elIndex + 2] = endCapStartIndex + i + 2;
		}
		else
		{
			mVertexIndices[elIndex] = endCapStartIndex;
			mVertexIndices[elIndex + 1] = endCapStartIndex + i + 1;
			mVertexIndices[elIndex + 2] = endCapStartIndex + 1;
		}

		elIndex += 3;
	}
}

void BuiltInCylinder::setup()
{
	generateVertexData();

	mShaderProgram = new ShaderProgram();

	//load shaders
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

	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertexPositions.size(), mVertexPositions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3,                 // number of elements per vertex, here (x,y,z,1)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);


	//create vbo for colors
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mVertexNormals.size(), mVertexNormals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (R,G,B)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);


	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mVertexIndices.size(), mVertexIndices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);

}

void BuiltInCylinder::RenderPicking() {
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BuiltInCylinder::RenderModel(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& location,
	glm::vec3 camPosition, glm::vec3 lightPosition, glm::mat4& lightSpace, Shadow* shadow, Animation* animation)
{

	glm::mat4 mview = view * model;
	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));
	glm::mat4 mvp = projection * view * model;

	glm::vec4 lightPos(0, 0, 300, 1);
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	float shininess = mMat->getShiness();

	mShaderProgram->use();
	//simple
	if (mShaderType == "Diffuse")
	{
		glUniform4fv(mShaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(mShaderProgram->uniform("Light.La"), 1, glm::value_ptr(objectColor));
		glUniform3fv(mShaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(objectColor));
		glUniform3fv(mShaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(objectColor));

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
		glUniform3fv(mShaderProgram->uniform("Light.Intensity"), 1, glm::value_ptr(objectColor));

		glUniformMatrix4fv(mShaderProgram->uniform("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));

		glUniform1i(mShaderProgram->uniform("shadowMap"), 0);

		glUniform1i(mShaderProgram->uniform("isShadow"), shadow->bIsShadow);
		glUniform1i(mShaderProgram->uniform("shadowType"), shadow->mShadowType);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow->mShadowGLuint);
	}

	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, 0);

	mShaderProgram->disable();

}