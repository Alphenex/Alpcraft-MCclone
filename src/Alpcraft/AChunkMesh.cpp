#include "AChunkMesh.h"

ChunkMesh::ChunkMesh(glm::ivec3 _ChunkWorldPos)
{
	ParentChunkPosition = _ChunkWorldPos;
}

ChunkMesh::~ChunkMesh()
{
	MeshVAO.Delete();
}

static const glm::ivec3 BlockVertex[6][4] = {
	{
		{1, 1, 1}, // FRONT FACE
		{0, 1, 1},
		{0, 0, 1},
		{1, 0, 1},
	},
	{

		{0, 1, 0}, // BACK FACE
		{1, 1, 0},
		{1, 0, 0},
		{0, 0, 0},

	},
	{

		{1, 1, 0}, // RIGHT FACE
		{1, 1, 1},
		{1, 0, 1},
		{1, 0, 0},

	},
	{
		{0, 1, 1}, // LEFT FACE
		{0, 1, 0},
		{0, 0, 0},
		{0, 0, 1},
	},
	{
		{1, 1, 0}, // UP FACE
		{0, 1, 0},
		{0, 1, 1},
		{1, 1, 1},
	},
	{
		{0, 0, 0}, // DOWN FACE
		{1, 0, 0},
		{1, 0, 1},
		{0, 0, 1},
	}
};

void ChunkMesh::AddFace(GLubyte _Side, glm::ivec3 _BlockPosition, glm::ivec2 _TexturePos, unsigned int LightLevel)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		glm::ivec3 vertices = BlockVertex[_Side][i] + _BlockPosition;
		GLubyte x = (GLubyte)vertices.x;
		GLubyte y = (GLubyte)vertices.y;
		GLubyte z = (GLubyte)vertices.z;

		// 33 bit usage.
		// X, Y, Z Positions
		// UV ID
		// Texture Positions
		// Light Level
		GLuint compressedinfo = x | y << 6 | z << 12 | i << 18 | _TexturePos.x << 20 | _TexturePos.y << 23 | LightLevel << 26;

		MeshVertex.push_back(compressedinfo);
	}

	MeshIndex.push_back(PseudoIndCount);
	MeshIndex.push_back(PseudoIndCount + 1);
	MeshIndex.push_back(PseudoIndCount + 2);
	MeshIndex.push_back(PseudoIndCount + 2);
	MeshIndex.push_back(PseudoIndCount + 3);
	MeshIndex.push_back(PseudoIndCount);
	PseudoIndCount += 4;
	IndCount += 6;
}

void ChunkMesh::DeleteMesh()
{
	MeshVAO.UnBind();
	MeshVAO.Delete();
}

void ChunkMesh::PrepareMesh()
{
	if (IsArrayValid)
	{
		MeshVAO.Delete();
		MeshVAO.GenVertexArray();
	}

	IsArrayValid = true;

	MeshVAO.Bind();

	VBO TempVBO(MeshVertex.data(), MeshVertex.size() * sizeof(GLuint));
	EBO TempEBO(MeshIndex.data(), MeshIndex.size() * sizeof(GLuint));

	MeshVAO.LinkAttribI(TempVBO, 0, 1, GL_UNSIGNED_INT, 0, (void*)0);

	MeshVAO.UnBind();

	TempEBO.UnBind();
	TempVBO.UnBind();

	TempVBO.Delete();
	TempEBO.Delete();

	MeshVertex.clear();
	MeshIndex.clear();
}

void ChunkMesh::RenderMesh(GLuint& _ShaderID)
{
	if (&MeshVAO != nullptr)
	{
		MeshVAO.Bind();
		//This uniform shouldn't be updated in a loop but I got lazy so stfu
		glUniform3f(glGetUniformLocation(_ShaderID, "chunkPosition"),
			ParentChunkPosition.x,
			ParentChunkPosition.y,
			ParentChunkPosition.z);
		glDrawElements(GL_TRIANGLES, IndCount, GL_UNSIGNED_INT, 0);
		MeshVAO.UnBind();
	}
}