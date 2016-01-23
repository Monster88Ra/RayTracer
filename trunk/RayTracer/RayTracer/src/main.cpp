#include <iostream>
#include <istream>
#include <cstdlib>
#include <time.h>

#include "Vector2.h"
#include "Scene.h"

int main()
{
	std::srand((unsigned int)time(nullptr));
	clock_t t1, t2;
	t1 = clock();
	std::filebuf fb;
	//D:/Codes/Ray Tracer/RayTracer/RayTracer/trunk/RayTracer/RayTracer/
	if (fb.open("ImageConfig.txt", std::ios::in))
	{
		// read in config data
		std::istream ConfigStream(&fb);
		std::string String;

		std::string OutputName, SceneFile;
		uint16_t ShadowSamples = 1;
		uint16_t SuperSampling = 1;
		Vector2i Resolution(1000, 600);

		ConfigStream >> String;
		while (ConfigStream.good())
		{
			if (String == "Resolution:")
			{
				ConfigStream >> Resolution.x >> Resolution.y;
			}
			else if (String == "SuperSampling:")
			{
				ConfigStream >> SuperSampling;
			}
			else if (String == "ShadowSamples:")
			{
				ConfigStream >> ShadowSamples;
			}
			else if (String == "OutputImage:")
			{
				ConfigStream >> OutputName;
			}
			else if (String == "SceneFile:")
			{
				ConfigStream >> SceneFile;
			}
			ConfigStream >> String;
		}
		fb.close();

		Scene scene(OutputName, Resolution, ShadowSamples, SuperSampling);
		if (fb.open(SceneFile, std::ios::in))
		{
			std::istream SceneStream(&fb);
			scene.BuildScene(SceneStream);
			scene.RenderScene();
		}
		else
		{
			std::cout << SceneFile << " scene file could not be opened.";
		}
	}
	else
	{
		std::cout << "ImageConfig.scn file could not be opened.";
	}
	t2 = clock();
	std::cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << std::endl;

	return 0;
}