#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "image_loader.h"
#include "model_loader.h"

bool load3DFromFile(string path, model* lmodel)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (!scene)
	{
		cerr << "Couldn't open file: " << path << endl << "\t" << importer.GetErrorString() << endl;
		return false;
	}

	cout << "Loading 3D mesh: " << path << endl;

	glGenBuffers(1, &lmodel->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lmodel->vbo);
	
	
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* aimesh = scene->mMeshes[i];
		mesh mesh;
		mesh.material = aimesh->mMaterialIndex;

		unsigned int startindex = lmodel->vertices.size();

		for (unsigned int j = 0; j < aimesh->mNumFaces; j++)
		{
			mesh.indices.push_back(aimesh->mFaces[j].mIndices[0] + startindex);
			mesh.indices.push_back(aimesh->mFaces[j].mIndices[1] + startindex);
			mesh.indices.push_back(aimesh->mFaces[j].mIndices[2] + startindex);
		}

		glGenBuffers(1, &mesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		lmodel->meshes.push_back(mesh);
		
		for (unsigned int j = 0; j < aimesh->mNumVertices; j++)
		{
			vertex vertex;
			vertex.position = vec3(aimesh->mVertices[j].x, aimesh->mVertices[j].y, aimesh->mVertices[j].z);
			vertex.uv = vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y);
			vertex.normal = vec3(aimesh->mNormals[j].x, aimesh->mNormals[j].y, aimesh->mNormals[j].z);
			lmodel->vertices.push_back(vertex);
		}
	}

	vector<float> vertexbuffer;
	for (unsigned int i = 0; i < lmodel->vertices.size(); i++)
	{
		vertexbuffer.push_back(lmodel->vertices[i].position.x);
		vertexbuffer.push_back(lmodel->vertices[i].position.y);
		vertexbuffer.push_back(lmodel->vertices[i].position.z);

		vertexbuffer.push_back(lmodel->vertices[i].uv.x);
		vertexbuffer.push_back(lmodel->vertices[i].uv.y);

		vertexbuffer.push_back(lmodel->vertices[i].normal.x);
		vertexbuffer.push_back(lmodel->vertices[i].normal.y);
		vertexbuffer.push_back(lmodel->vertices[i].normal.z);
	}

	glBufferData(GL_ARRAY_BUFFER, vertexbuffer.size() * sizeof(float), &vertexbuffer[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* aimaterial = scene->mMaterials[i];
		material material;

		aiColor3D diffuse;
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material.diffuse = vec3(diffuse.r, diffuse.g, diffuse.b);

		aiColor3D specular;
		aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material.specular = vec3(specular.r, specular.g, specular.b);

		aiColor3D ambient;
		aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material.ambient = vec3(ambient.r, ambient.g, ambient.b);

		aimaterial->Get(AI_MATKEY_OPACITY, material.opacity);
		aimaterial->Get(AI_MATKEY_SHININESS, material.shininess);
		aimaterial->Get(AI_MATKEY_SHININESS_STRENGTH, material.shine_strength);

		aiString texpath;
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texpath);
		string texname = texpath.data;

		GLuint texture = 0;
		if (texname != "")
		{
			string stringpath = string(path);
			stringpath = stringpath.substr(0, stringpath.find_last_of("/") + 1);

			while (texname.find("../") != -1)
			{
				texname = texname.substr(3);
				stringpath = stringpath.substr(0, stringpath.find_last_of("/", stringpath.find_last_of("/") - 1) + 1);
			}
			
			string texloadpath = stringpath + texname;
			
			texture = loadTexture2D(texloadpath.c_str());
		}

		material.texture = texture;
		lmodel->materials.push_back(material);
	}

	cout << endl;

	return true;
}

void drawModel(const model* rmodel, Shader* program)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	for (mesh mesh : rmodel->meshes)
	{
		const material* mat = &rmodel->materials[mesh.material];
		
		program->uniform3f(mat->diffuse, "mat.diffuse");
		program->uniform3f(mat->specular, "mat.specular");
		program->uniform3f(mat->ambient, "mat.ambient");

		program->uniform1f(mat->opacity, "mat.opacity");
		program->uniform1f(mat->shininess, "mat.shininess");
		program->uniform1f(mat->shine_strength, "mat.shine_strength");

		program->uniform1i(false, "mat.textured");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void drawModel(const model* rmodel, Shader* program, GLuint sampler, unsigned int texture_handle)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, rmodel->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	for (mesh mesh : rmodel->meshes)
	{
		const material* mat = &rmodel->materials[mesh.material];

		program->uniform3f(mat->diffuse, "mat.diffuse");
		program->uniform3f(mat->specular, "mat.specular");
		program->uniform3f(mat->ambient, "mat.ambient");

		program->uniform1f(mat->opacity, "mat.opacity");
		program->uniform1f(mat->shininess, "mat.shininess");
		program->uniform1f(mat->shine_strength, "mat.shine_strength");

		program->uniform1i(true, "mat.textured");

		glActiveTexture(GL_TEXTURE0 + texture_handle);
		glBindSampler(texture_handle, sampler);
		glBindTexture(GL_TEXTURE_2D, mat->texture);
		program->uniform1i(texture_handle, "mat.texture");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}