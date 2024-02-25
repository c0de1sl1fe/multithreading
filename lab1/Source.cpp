#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>





void printMatrix(int** arr, int row, int col)
{
    if (arr == NULL)
    {
        std::cout << ":(";
        return;
    }
    for (int i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
int** createMatrix(int row, int col)
{
    int** a = new int* [row];
    for (size_t i = 0; i < row; i++)
    {
        a[i] = new int[col] {};
    }
    return a;
}
void freeMatrix(int**a, int row, int col)
{
    for (size_t i = 0; i < row; i++)
    {
        delete[]a[i];
    }
    delete[]a;
    return;
}

void feelSmth(int** arr, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            arr[i][j] = 1;
        }
    }
}
struct matrix
{
    int row;
    int col;
    int** arr;
    void operator= (const matrix& src)
    {

        if (this->arr != NULL)
        {
            freeMatrix(this->arr, this->row, this->col);
        }
        this->row = src.row;
        this->col = src.col;
        this->arr = createMatrix(this->row, this->col);
        for (int i = 0; i < this->row; i++)
        {
            for (size_t j = 0; j < this->col; j++)
            {
                this->arr[i][j] = src.arr[i][j];
            }
        }
    }
    ~matrix()
    {
        if (!this->arr) {
            freeMatrix(this->arr, this->row, this->col);
        }
        this->arr = NULL;
        this->row = 0;
        this->col = 0;
    }
};
int** mulMatrix(int** a, int** b, int row1, int col1, int row2, int col2)
{
    if(col1!=row2)
    {
        return NULL;
    }

    int row = row1, col = col2;
    int** res = createMatrix(row, col);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            for (size_t k = 0; k < row2; k++)
            {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return res;
}

struct stats
{
    int elements;
    int time;
};
stats test(matrix mt1, matrix mt2, std::function<int**(int** a, int** b, int row1, int col1, int row2, int col2)> matMul)
   {
    stats res;
    matrix c;  
    c.row = mt1.row;
    c.col = mt2.col;
    auto start = std::chrono::high_resolution_clock::now();
    c.arr = matMul(mt1.arr, mt2.arr, mt1.row, mt1.col, mt2.row, mt2.col);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    res.elements = c.row * c.col;
    res.time = duration.count();
    std::cout << res.time;
    return res;
}

//std::vector<std::vector<int>> readMatrix(std::string path)
//{
//    std::fstream file(path, std::ios::in);
//    std::string buffer;
//    std::vector<std::vector<int>> mt;
//    int row = 0;
//    int col = 0;
//    file >> row;
//    file >> col;
//    mt.resize(row);
//    for (auto &i : mt)
//    {
//        i.resize(col);
//        for (auto& j : i)
//        {
//            file >> j;
//        }
//    }
//    file.close();
//    return mt;
//}

matrix readMatrix(std::string path)
{
    std::fstream file(path, std::ios::in);
    matrix mt;
    file >> mt.row;
    file >> mt.col;
    
    mt.arr = createMatrix(mt.row, mt.col);
    for (size_t i = 0; i < mt.row; i++)
    {
        for (size_t j = 0; j < mt.col; j++)
        {
            file >> mt.arr[i][j];
        }
    }
    file.close();
    return mt;
}

void writeMatrix(matrix& mt, std::string path)
{
    std::fstream file(path, std::ios::out);
    file << mt.row << " " << mt.col << std::endl;
    for (size_t i = 0; i < mt.row; i++)
    {
        for (size_t j = 0; j < mt.col; j++)
        {
            file << mt.arr[i][j]<< " ";
        }
        file << std::endl;
    }
}

void writeStats(stats res, std::string path)
{
    std::fstream file(path, std::ios::app);
    if (file.is_open())
    {
        file << res.elements << " " << res.time << std::endl;
    }
    else
    {
        std::cout << ":(";
    }

}


int main()
{
    std::string path = "D:/Git/multithreading/stats.txt";
    std::string pathRes = "D:/Git/multithreading/res_cpp.txt";
    matrix a = readMatrix("D:/Git/multithreading/matrix1.txt");
    matrix b = readMatrix("D:/Git/multithreading/matrix2.txt");
    matrix c;
    c.row = a.row;
    c.col = b.col;
    c.arr = mulMatrix(a.arr, b.arr, a.row, a.col, b.row, b.col);
    writeMatrix(c, pathRes);
    writeStats(test(a, b, mulMatrix), path);
    return 0;

}