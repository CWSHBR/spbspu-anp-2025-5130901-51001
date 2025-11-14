#include <iostream>
#include <fstream>
#include <string>

namespace krivoshapov {

//общие ошики для двух вариантов

int main(int argc, char* argv[] ){
    
    if (argc!=4){
        std::cerr << "Ошибка: неверное количество аргументов \n";
        return 1;
    }
    std::string modestr = argv[1];
    int mode = 0;
    if (modestr == "1") mode = 1;
    else if (modestr == "2") mode = 2;
    else {
        std::cerr << "Ошибка: параметр должен быть 1 или 2 \n";
        return 1;
    }
    
    const char* inputfile = argv[2];
    const char* outputfile = argv[3];
    
    std::ifstream fin (inputfile);
    if (!fin){
        std::cerr << "Ошибка: не удалось открыть исхожный файл \n";
        return 2;
    }
    
    int rows, cols;
    if (!(fin >> rows >> cols)){
        std::cerr << "Ошибка: не удалось прочитать размер матрицы \n";
        return 2;
    }
    if (rows < 0 || cols < 0){
        std::cerr << "Ошибка: размеры матрицы не моугт быть отрицательными \n";
        return 2;
    }
    if (rows == 0 && cols == 0){
        std::ofstream fout(outputfile);
        if (!fout){
            std::cerr << "Ошибка: не удалось открыть выходной файл файл \n";
            return 2;
            
        }
        fout << 0 << "\n" << 0 << "\n";
        return 0;
    }
    
    const int MaxSize = 10000;
    int total = rows * cols;
    if (mode == 1 && total > MaxSize){
        std::cerr << "Ошибка: слишком большой массив \n";
        return 2;
    }
    
    int* data = 0;
    int staticArr [MaxSize];
    
    if (mode == 1){
        data = staticArr;
    } else {
        data = new int [total];
    }
    
    for (int i=0; i < total; i++){
        if (!(fin >> data[i])) {
            if (mode == 2) delete[] data;
            std::cerr << "Ошибка: файл не содержит достаточного количества чисел \n";
            return 2;
        }
    }
    
//ВАРИНАТ 11
    
    int bestcol = 1;
    int bestlen = 0;
    
    for (int c = 0; c < cols; c++){
        int maxRun = 1;
        int curRun = 1;
        for (int r = 1; r < rows; c++){
            int prev = data [(r-1) * cols + c ];
            int cur = data[r * cols +c];
            if (cur == prev){
                curRun ++;
            } else {
                if (curRun > maxRun) maxRun = curRun;
                curRun=1;
            }
        }
        if (curRun > maxRun) maxRun = curRun;
        
        if (maxRun > bestlen) {
            bestlen = maxRun;
            bestcol = c+1;
        }
    }
    
// ВАРИАНТ 1
    
    int rowmin [200];
    int colmax [200];
    
    for (int r=0; r < rows; r++){
        rowmin[r] = data [r * cols + 0];
    }
    for (int c = 0; c < cols; c++){
        colmax[c] = data [0 * cols + c];
    }
    
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            int val = data[r * cols + c];
            if (val < rowmin[r]) rowmin[r] = val;
            if (val > colmax[c]) colmax[c] = val;
        }
    }
    
    int saddle = 0;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            int val = data[r * cols + c];
            if (val == rowmin[r] && val == colmax[c]) saddle++;
        }
    }
    
    std::ofstream fout(outputfile);
    if(!fout){
        if (mode == 2) delete[] data;
        std::cerr << "Ошибка: не удалось открыть выходной файл \n";
        return 2;
    }
    fout << bestcol << "\n" << saddle << "\n";
    
    if (mode == 2) delete[] data;
    return 0;
}
}

int main (int argc, char* argv[]){
    return krivoshapov::main(argc,argv);
}

