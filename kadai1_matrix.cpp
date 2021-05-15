#include <vector>
#include <random>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#define MAX_VALUE 3 //行列の要素の最大値
void initialize(std::vector<std::vector<int>>&matrix, int N);
void mul(std::vector<std::vector<int>>&matrix1, std::vector<std::vector<int>>&matrix2,std::vector<std::vector<int>>&ans, int N);
void print_equation(std::vector<std::vector<int>>&matrix1, std::vector<std::vector<int>>&matrix2,std::vector<std::vector<int>>&ans, int N);
void print_right_frame(int i, int N);
void print_left_frame(int i, int N);
void print_matrix(std::vector<std::vector<int>>&matrix, int i,int N);
std::vector<int>max_digit(std::vector<std::vector<int>>&ans, int N);
int calc_digit(int num);
void print_matrix_ans(std::vector<std::vector<int>>&matrix, int i, int N, std::vector<int> &space);

double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main(int argc, char** argv){
    if (argc != 2) {
        printf("usage: %s N\n", argv[0]);
        return -1;
    }
    int N = atoi(argv[1]); //行列のサイズ
    std::vector<std::vector<int>> matrix1(N, std::vector<int>(N));
    std::vector<std::vector<int>> matrix2(N, std::vector<int>(N));
    std::vector<std::vector<int>> ans(N, std::vector<int>(N,0));
    
    initialize(matrix1,N);
    initialize(matrix2,N);

    double begin = get_time();

    mul(matrix1, matrix2, ans, N);

    double end = get_time();

    print_equation(matrix1, matrix2, ans, N);

    printf("              \x1b[31mtime: %.9lf sec\033[m\n", end - begin);
    return 0;
}


//行列を乱数を用いて初期化
void initialize(std::vector<std::vector<int>>&matrix, int N){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand10(1, MAX_VALUE);
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            matrix[i][j] = rand10(mt);
        }
    }
}

//行列積の計算
void mul(std::vector<std::vector<int>>&matrix1, std::vector<std::vector<int>>&matrix2, std::vector<std::vector<int>>&ans, int N){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            for (int k=0; k<N; k++){
                ans[i][j]+=matrix1[i][k]*matrix2[k][j];
            }
        }
    }
}

//行列積の計算式を表示
void print_equation(std::vector<std::vector<int>>&matrix1, std::vector<std::vector<int>>&matrix2,std::vector<std::vector<int>>&ans, int N){
    std::vector<int> space(N);
    space = max_digit(ans, N);

    for (int i=0; i<N; i++){
    
        print_matrix(matrix1,i,N);

        if (i==N/2) std::cout << "* " ;
        else std::cout << "  ";

        print_matrix(matrix2,i,N);

        if (i==N/2) std::cout << "= " ;
        else std::cout << "  ";

        print_matrix_ans(ans,i,N,space);

        if (i!=N-1) std::cout << std::endl;
    }

}

//行列の数字を表示
void print_matrix(std::vector<std::vector<int>>&matrix, int i, int N){
    print_left_frame(i,N);
    for (int j=0; j<N; j++){
        std::cout << matrix[i][j] << " ";
    }
    print_right_frame(i,N);
}

//行列の[を表示
void print_left_frame(int i, int N){
    if (!i) std::cout << "/ ";
    else if (i!=N-1) std::cout << "| ";
    else std::cout << "\\ ";
}

//行列の]を表示
void print_right_frame(int i, int N){
    if (!i) std::cout << "\\ ";
    else if (i!=N-1) std::cout << "| ";
    else std::cout << "/ ";
}

//行列積を表示
void print_matrix_ans(std::vector<std::vector<int>>&matrix, int i, int N, std::vector<int> &space){
    print_left_frame(i,N);
    for (int j=0; j<N; j++){
        std::cout << matrix[i][j] << " ";
        for (int k=0; k < space.at(j)-calc_digit(matrix[i][j]); k++){
            std::cout << " ";
        }
    }
    print_right_frame(i,N);
}

//行列の各列の最大の数字の桁数を格納した配列を返す
std::vector<int>max_digit(std::vector<std::vector<int>>&ans, int N) {
    std::vector<int> space(N);
    for (int i=0; i<N; i++){
        int max = 0;
        for (int j=0; j<N; j++){
            if (max<ans[j][i]) max = ans[j][i];
        }
        space.at(i)=calc_digit(max);
    }
    return space;
}

//与えられた数字の桁数を返す
int calc_digit(int num){
    int digit = 0;
    while(num){
        digit++;
        num /= 10;
    }
    return digit;
}
