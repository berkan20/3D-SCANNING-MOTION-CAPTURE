#include <iostream>
#include <fstream>
#include <array>

#include "Eigen.h"

#include "VirtualSensor.h"

struct Vertex
{
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	// position stored as 4 floats (4th component is supposed to be 1.0)
	Vector4f position;
	// color stored as 4 unsigned char
	Vector4uc color;
};

bool WriteMesh(Vertex* vertices, unsigned int width, unsigned int height, const std::string& filename)
{
	float edgeThreshold = 0.01f; // 1cm

	// TODO 2: use the OFF file format to save the vertices grid (http://www.geomview.org/docs/html/OFF.html)
	// - have a look at the "off_sample.off" file to see how to store the vertices and triangles
	// - for debugging we recommend to first only write out the vertices (set the number of faces to zero)
	// - for simplicity write every vertex to file, even if it is not valid (position.x() == MINF) (note that all vertices in the off file have to be valid, thus, if a point is not valid write out a dummy point like (0,0,0))
	// - use a simple triangulation exploiting the grid structure (neighboring vertices build a triangle, two triangles per grid cell)
	// - you can use an arbitrary triangulation of the cells, but make sure that the triangles are consistently oriented
	// - only write triangles with valid vertices and an edge length smaller then edgeThreshold

	// TODO: Get number of vertices
	unsigned int nVertices = 0;
    nVertices = width * height;
	

	// TODO: Determine number of valid faces
	unsigned nFaces = 0;
    for (unsigned int i = 0; i < width*height; ++i) {
        if(i == width*(height-1))break;
        if(i%width == width-1)continue;
        // Extract the 4 vertices of a grid
        Vertex top_left = vertices[i];
        Vertex top_right = vertices[i+1];
        Vertex bottom_left = vertices[i+width];
        Vertex bottom_right = vertices[i+width+1];

        if(top_right.position.x() != MINF && bottom_left.position.x() != MINF ){ //If those vertices are not valid then we dont need to look for anything else
            if(top_left.position.x() != MINF){  // if true then check the left triangle
                // calculate edge lengths of the left triangle
                float edge1 = sqrt(pow(top_left.position.x() - top_right.position.x(), 2) + pow(top_left.position.y() - top_right.position.y(), 2)+
                                   pow(top_left.position.z() - top_right.position.z(), 2));
                float edge2 = sqrt(pow(top_left.position.x() - bottom_left.position.x(), 2) + pow(top_left.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_left.position.z() - bottom_left.position.z(), 2));
                float edge3 = sqrt(pow(top_right.position.x() - bottom_left.position.x(), 2) + pow(top_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_right.position.z() - bottom_left.position.z(), 2));

                if(edge1 < edgeThreshold && edge2 < edgeThreshold && edge3 < edgeThreshold){  //check for thresholds
                    nFaces++;
                }
            }
            if(bottom_right.position.x() != MINF){  // if true then check the right triangle
                // calculate edge lenghts of the right triangle
                float edge1 = sqrt(pow(bottom_right.position.x() - top_right.position.x(), 2) + pow(bottom_right.position.y() - top_right.position.y(), 2)+
                                   pow(bottom_right.position.z() - top_right.position.z(), 2));
                float edge2 = sqrt(pow(bottom_right.position.x() - bottom_left.position.x(), 2) + pow(bottom_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(bottom_right.position.z() - bottom_left.position.z(), 2));
                float edge3 = sqrt(pow(top_right.position.x() - bottom_left.position.x(), 2) + pow(top_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_right.position.z() - bottom_left.position.z(), 2));
                if(edge1 < edgeThreshold && edge2 < edgeThreshold && edge3 < edgeThreshold){
                    nFaces++;
                }
            }

        }
    }

	// Write off file
	std::ofstream outFile(filename);
	if (!outFile.is_open()) return false;

	// write header
	outFile << "COFF" << std::endl;
	outFile << nVertices << " " << nFaces << " 0" << std::endl;

	// TODO: save vertices
    for (unsigned int i = 0; i < width*height; ++i) {
        if(vertices[i].position.x() != MINF) {
            outFile << vertices[i].position.x() << " " << vertices[i].position.y() << " " << vertices[i].position.z()
                    << " " <<
                    (int) vertices[i].color.x() << " " << (int) vertices[i].color.y() << " "
                    << (int) vertices[i].color.z() << " " << (int) vertices[i].color.w() << std::endl;
        }else{
            outFile << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << std::endl;
        }
    }
	// TODO: save valid faces
    for (unsigned int i = 0; i < width*height; ++i) {
        if(i == width*(height-1))break;
        if(i%width == width-1)continue;
        // Extract the 4 vertices of a grid
        Vertex top_left = vertices[i];
        Vertex top_right = vertices[i+1];
        Vertex bottom_left = vertices[i+width];
        Vertex bottom_right = vertices[i+width+1];

        if(top_right.position.x() != MINF && bottom_left.position.x() != MINF ){ //If those vertices are not valid then we dont need to look for anything else
            if(top_left.position.x() != MINF){  // if true then check the left triangle
                // calculate edge lengths of the left triangle
                float edge1 = sqrt(pow(top_left.position.x() - top_right.position.x(), 2) + pow(top_left.position.y() - top_right.position.y(), 2)+
                pow(top_left.position.z() - top_right.position.z(), 2));
                float edge2 = sqrt(pow(top_left.position.x() - bottom_left.position.x(), 2) + pow(top_left.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_left.position.z() - bottom_left.position.z(), 2));
                float edge3 = sqrt(pow(top_right.position.x() - bottom_left.position.x(), 2) + pow(top_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_right.position.z() - bottom_left.position.z(), 2));

                if(edge1 < edgeThreshold && edge2 < edgeThreshold && edge3 < edgeThreshold){  //check for thresholds
                    outFile << 3 << " " << i << " " << i+width << " " << i+1 <<  std::endl;  //write the indices to file
                }
            }
            if(bottom_right.position.x() != MINF){  // if true then check the right triangle
                // calculate edge lenghts of the right triangle
                float edge1 = sqrt(pow(bottom_right.position.x() - top_right.position.x(), 2) + pow(bottom_right.position.y() - top_right.position.y(), 2)+
                                   pow(bottom_right.position.z() - top_right.position.z(), 2));
                float edge2 = sqrt(pow(bottom_right.position.x() - bottom_left.position.x(), 2) + pow(bottom_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(bottom_right.position.z() - bottom_left.position.z(), 2));
                float edge3 = sqrt(pow(top_right.position.x() - bottom_left.position.x(), 2) + pow(top_right.position.y() - bottom_left.position.y(), 2)+
                                   pow(top_right.position.z() - bottom_left.position.z(), 2));
                if(edge1 < edgeThreshold && edge2 < edgeThreshold && edge3 < edgeThreshold){
                    outFile << 3 << " " << i+width << " " << i+width+1 << " " << i+1 <<  std::endl;
                }
            }

        }
    }

	// close file
	outFile.close();

	return true;
}

int main()
{
	// Make sure this path points to the data folder
	std::string filenameIn = "../../data/rgbd_dataset_freiburg1_xyz/";
	std::string filenameBaseOut = "mesh_";

	// load video
	std::cout << "Initialize virtual sensor..." << std::endl;
	VirtualSensor sensor;
	if (!sensor.Init(filenameIn))
	{
		std::cout << "Failed to initialize the sensor!\nCheck file path!" << std::endl;
		return -1;
	}

	// convert video to meshes
	while (sensor.ProcessNextFrame())
	{
		// get ptr to the current depth frame
		// depth is stored in row major (get dimensions via sensor.GetDepthImageWidth() / GetDepthImageHeight())
		float* depthMap = sensor.GetDepth();
		// get ptr to the current color frame
		// color is stored as RGBX in row major (4 byte values per pixel, get dimensions via sensor.GetColorImageWidth() / GetColorImageHeight())
		BYTE* colorMap = sensor.GetColorRGBX();


		// get depth intrinsics
		Matrix3f depthIntrinsics = sensor.GetDepthIntrinsics();
		float fX = depthIntrinsics(0, 0);
		float fY = depthIntrinsics(1, 1);
		float cX = depthIntrinsics(0, 2);
		float cY = depthIntrinsics(1, 2);

		// compute inverse depth extrinsics
		Matrix4f depthExtrinsicsInv = sensor.GetDepthExtrinsics().inverse();

		Matrix4f trajectory = sensor.GetTrajectory();
		Matrix4f trajectoryInv = sensor.GetTrajectory().inverse();

		// TODO 1: back-projection
		// write result to the vertices array below, keep pixel ordering!
		// if the depth value at idx is invalid (MINF) write the following values to the vertices array
		// vertices[idx].position = Vector4f(MINF, MINF, MINF, MINF);
		// vertices[idx].color = Vector4uc(0,0,0,0);
		// otherwise apply back-projection and transform the vertex to world space, use the corresponding color from the colormap
		Vertex* vertices = new Vertex[sensor.GetDepthImageWidth() * sensor.GetDepthImageHeight()];
        for (unsigned int i = 0; i < sensor.GetDepthImageHeight()*sensor.GetDepthImageWidth(); ++i){
            if(depthMap[i] == MINF){
                vertices[i].position = Vector4f(MINF, MINF, MINF, MINF);
                vertices[i].color = Vector4uc(0,0,0,0);
            }else{
                unsigned int row = i / sensor.GetDepthImageWidth();
                unsigned int col = i % sensor.GetDepthImageWidth();
                float X_C = (row - cX)*depthMap[i] / fX;
                float Y_C = (col - cY)*depthMap[i] / fY;

                vertices[i].position = trajectoryInv * depthExtrinsicsInv * Vector4f(X_C, Y_C, depthMap[i], 1);
                vertices[i].color = Vector4uc(colorMap[4*i],colorMap[4*i+1],colorMap[4*i+2],colorMap[4*i+3]);
            }
        }

		// write mesh file
		std::stringstream ss;
		ss << filenameBaseOut << sensor.GetCurrentFrameCnt() << ".off";

		if (!WriteMesh(vertices, sensor.GetDepthImageWidth(), sensor.GetDepthImageHeight(), ss.str()))
		{
			std::cout << "Failed to write mesh!\nCheck file path!" << std::endl;
			return -1;
		}

		// free mem
		delete[] vertices;
	}

	return 0;
}
