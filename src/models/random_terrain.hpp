#ifndef RANDOM_TERRAIN_H
#define RANDOM_TERRAIN_H

#include <math.h>
#include <vector>
#include <iostream>

class RandomTerrain {
public:
    RandomTerrain(int n, float r) {
        size = pow(2, n) + 1;
        range = r;


        grid = new float*[size];
        for(int i = 0;i < size;i++) {
            grid[i] = new float[size];
        }

        corners();
        for(int sideLength = size - 1;sideLength >= 2;sideLength /=2,range/=2) {
            int halfSide = sideLength / 2;
            squareStep(sideLength, halfSide);
            diamondStep(sideLength, halfSide);
        }
    }
    ~RandomTerrain() {
        for(int i = 0;i < size;i++) {
            delete [] grid[i];
        }
        delete [] grid;
    }

    std::vector<float> getVertsXYZ() {
        std::vector<float> verts;

        float x_offset = 2.0f / (float)size;
        float z_offset = 2.0f / (float)size;
        float x = -1.0f;
        float z = -1.0f;
        for(int i = 0;i < size;i++) {
            for(int j = 0;j < size;j++) {
                verts.push_back(x + ((float) i) * x_offset);
                verts.push_back(grid[i][j]);
                verts.push_back(z * ((float) j) * z_offset);
            }
        }
        return verts;
    }

    std::vector<int> getFaces() {
        std::vector<int> faces;
        for(int i = 0;i < size - 1;i++) {
            for(int j = 0;j < size - 1;j++) {
                int idx = i * size + j;
                faces.push_back(idx);
                faces.push_back(idx + size + 1);
                faces.push_back(idx + size);
                faces.push_back(idx + 1);
                faces.push_back(idx + size + 1);
                faces.push_back(idx);
            }
        }
        return faces;
    }

private:
    float** grid;
    int size;
    float range;
    float min_val;
    float max_val;
    float random_range;

    void corners() {
        grid[0][0] = 0;//randomValue(-range, range);
        grid[0][size - 1] = 0;//randomValue(-range, range);
        grid[size - 1][0] = 0;//randomValue(-range, range);
        grid[size - 1][size - 1] = 0;//randomValue(-range, range);
    }
    void diamondStep(int sideLength, int halfSide) {
        for(int x = 0;x < size-1;x+=halfSide) {
            for(int y = (x + halfSide) % sideLength;y < size - 1;y+=sideLength) {
                float avg = grid[(x-halfSide+size-1)%(size-1)][y] +
                    grid[(x + halfSide)%(size-1)][y] +
                    grid[x][(y+halfSide)%(size-1)] +
                    grid[x][(y-halfSide + size-1)%(size-1)];
                avg /= 4.0f + randomValue(-range, range);
                grid[x][y] = avg;

                if(x == 0) grid[size-1][y] = avg;
                if(y == 0) grid[x][size-1] = avg;
            }
        }
    }
    void squareStep(int sideLength, int halfSide) {
        for(int x = 0; x < size-1;x+=sideLength) {
            for(int y = 0;y < size-1;y += sideLength) {
                float avg = grid[x][y] + grid[x+sideLength][y] + grid[x][y+sideLength] + grid[x+sideLength][y+sideLength];
                avg /= 4.0f;
                grid[x+halfSide][y+halfSide] = avg + randomValue(-range, range);
            }
        }
    }


    float randomValue(float f_min, float f_max) {
        float f = (float)rand() / RAND_MAX;
        return f_min + f * (f_max - f_min);
    }
};

#endif
