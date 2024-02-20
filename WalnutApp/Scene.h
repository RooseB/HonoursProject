#pragma once

#include <glm/glm.hpp>

#include <vector>

//MATERIALS
struct Material{

	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;

	//LIGHT SETTINGS
	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0.0f;

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }
};

//SPHERE
struct Sphere{

	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;

	int MaterialIndex = 0;
};

//CUBE theory
//struct Cube{
//	glm::vec3 Position{ 2.0f };
//
//};

struct Scene{

	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};