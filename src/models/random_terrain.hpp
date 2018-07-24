#ifndef RANDOM_TERRAIN_H
#define RANDOM_TERRAIN_H

#include <math.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

struct Index {
    Index(int _i, int _j) {
        i = _i;
        j = _j;
    }
    int i;
    int j;
};

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

//      a--b--c
//      |\ |\ |
//      | \| \|
//      d--e--f
//      |\ |\ |
//      | \| \|
//      g--h--i

    std::vector<float> getVertsXYZNT() {
        std::vector<float> verts;

        for(int i = 0;i < size - 1;i++) {
            for(int j = 0;j < size - 1;j++) {
                float ax_val, az_val,bx_val, bz_val, ex_val, ez_val, dx_val, dz_val;
                ax_val = getVal(i); az_val = getVal(j);
                bx_val = getVal(i + 1); bz_val = az_val;
                ex_val = bx_val; ez_val = getVal(j + 1);
                dx_val = ax_val; dz_val = ez_val;

                // A
                addVertXYZ(verts, i, j, ax_val, az_val);
                addVertN(verts, i, j, ax_val, az_val);
                verts.push_back(0.0f);
                verts.push_back(0.0f);

                // B
                addVertXYZ(verts, i + 1, j, bx_val, bz_val);
                addVertN(verts, i + 1, j, bx_val, bz_val);
                verts.push_back(1.0f);
                verts.push_back(0.0f);

                // E
                addVertXYZ(verts, i + 1, j + 1, ex_val, ez_val);
                addVertN(verts, i + 1, j + 1, ex_val, ez_val);
                verts.push_back(1.0f);
                verts.push_back(1.0f);

                // A
                addVertXYZ(verts, i, j, ax_val, az_val);
                addVertN(verts, i, j, ax_val, az_val);
                verts.push_back(0.0f);
                verts.push_back(0.0f);

                // E
                addVertXYZ(verts, i + 1, j + 1, ex_val, ez_val);
                addVertN(verts, i + 1, j + 1, ex_val, ez_val);
                verts.push_back(1.0f);
                verts.push_back(1.0f);

                // D
                addVertXYZ(verts, i, j + 1, dx_val, dz_val);
                addVertN(verts, i, j + 1, dx_val, dz_val);
                verts.push_back(0.0f);
                verts.push_back(1.0f);


                // glm::vec3 b = glm::vec3(getVal(i + 1), grid[i+1][j], getVal(j));

                // float x_val = getVal(i);
                // float z_val = getVal(j);
                // addVertXYZ(verts, i, j, x_val, z_val);
                //
                // addVertN(verts, i, j, x_val, z_val);

            }
        }
        return verts;
    }

    std::vector<float> getVertsXYZN() {
        std::vector<float> verts;

        for(int i = 0;i < size;i++) {
            for(int j = 0;j < size;j++) {
                float x_val = getVal(i);
                float z_val = getVal(j);

                addVertXYZ(verts, i, j, x_val, z_val);
                addVertN(verts, i, j, x_val, z_val);
            }
        }
        return verts;
    }



    std::vector<float> getVertsXYZ() {
        std::vector<float> verts;

        for(int i = 0;i < size;i++) {
            for(int j = 0;j < size;j++) {
                addVertXYZ(verts, i, j, getVal(i), getVal(j));
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

    float getMaxValue() {
        return getExtrema(-9999.0f, greater);
    }

    float getMinValue() {
        return getExtrema(9999.0f, less);
    }

private:
    float** grid;
    int size;
    float range;
    float min_val;
    float max_val;
    float random_range;

    void addVertXYZ(std::vector<float> &verts, int i, int j, float x_val, float z_val) {
        verts.push_back(x_val);
        verts.push_back(grid[i][j]);
        verts.push_back(z_val);
    }

    void addVertN(std::vector<float> &verts, int i, int j, float x_val, float z_val) {
        std::vector<glm::vec3> vecs;
        Index a(i-1, j-1);
        Index b(i-1, j);
        Index c(i-1, j+1);
        Index d(i, j-1);
        Index e(i, j);
        Index f(i, j+1);
        Index g(i+1, j-1);
        Index h(i+1, j);
        Index k(i+1, j+1);
        addIndex(vecs, a, b, e);
        addIndex(vecs, b, f, e);
        addIndex(vecs, b, c, f);
        addIndex(vecs, a, e, d);
        addIndex(vecs, d, e, h);
        addIndex(vecs, d, h, g);
        addIndex(vecs, e, k, h);
        addIndex(vecs, e, f, k);

        glm::vec3 normal = interpNormals(vecs);
        verts.push_back(normal.x);
        verts.push_back(normal.y);
        verts.push_back(normal.z);
    }

    glm::vec3 interpNormals(std::vector<glm::vec3> normals) {
        glm::vec3 avg(0, 0, 0);
        for(int i = 0; i < normals.size();i++) {
            avg += normals[i];
        }
        return avg / (float)normals.size();
    }

    glm::vec3 triNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        glm::vec3 U = b - a;
        glm::vec3 V = c - a;
        return glm::cross(U, V);
    }

    glm::vec3 getVertex(Index i) {
        return glm::vec3(getVal(i.i), grid[i.i][i.j], getVal(i.j));
    }

    void addIndex(std::vector<glm::vec3> &vec, Index a, Index b, Index c) {
        if(inBounds(a) && inBounds(b)&& inBounds(c)) {
            vec.push_back(triNormal(getVertex(a), getVertex(b), getVertex(c)));
        }
    }

    bool inBounds(Index i) {
        return i.i >= 0 && i.j >= 0 && i.i < size && i.j < size;
    }

    float getVal(float idx) {
        float offset = 2.0f / (float)size;
        return -1.0f + idx * offset;
    }

    static bool less(float a, float b) {
        return a < b;
    }

    static bool greater(float a, float b) {
        return a > b;
    }

    float getExtrema(float init, bool (*comparater)(float, float)) {
        float extreme = init;
        for(int i = 0;i < size;i++) {
            for(int j = 0;j < size;j++) {
                if(comparater(grid[i][j], extreme)) {
                    extreme = grid[i][j];
                }
            }
        }
        return extreme;
    }

    void corners() {
        grid[0][0] = 0;
        grid[0][size - 1] = 0;
        grid[size - 1][0] = 0;
        grid[size - 1][size - 1] = 0;
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
