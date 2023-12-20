#include <iostream>
#include <mpi.h>

using namespace std;

int get_random_number(int rank) {
    srand(rank);
    return rand() % 255;
}

void fill_secret_part(int &secret_part, int rank, int data[]) {
    secret_part = data[rank -1];
}

void print(int secret_code[], int rank, int size) {
    cout << "City " << rank << ": ";
    for (int i = 0; i < size; i++) {
        cout << secret_code[i] << " ";
    }
    cout << endl;
}




#ifdef TESTS
#include <gtest/gtest.h>

TEST(FunctionTesting, check) {
    int temp[] {5435,345,35,24,6546,35,36,546,35326,546,457,35,234,34,645,74,57,568,679};
    int secret_part=0;
    fill_secret_part(secret_part, 17, temp);
    EXPECT_EQ(secret_part, 57);
}

TEST(FunctionTesting2, check2) {
    int temp[] {5435,345,35,24,6546,35,36,546,35326,546,457,35,234,34,645,74,57,568,679};
    int secret_part = 0;
    int rank = 2;
    fill_secret_part(secret_part, rank, temp);
    EXPECT_EQ(temp[rank -1], secret_part);
}

TEST(FunctionTesting3, check3) {
    int temp[] {11, 12, 15};
    int secret_part = 0;
    int array_size = 3;
    int *secret_code;
    secret_code = new int[array_size];
    int *result = new int[array_size]{10, 15, 15};
    secret_code[0] = 10;
    for (int i = 1; i < array_size; i++) {
        fill_secret_part(secret_part, 3, temp);
        secret_code[i] = secret_part;
    }
    for (int i = 0; i < array_size; i++) {
        EXPECT_EQ(secret_code[i], result[i]);
    }
}

TEST(FunctionTesting4, check4) {
    int temp[] {151, 132, 15};
    int secret_part = 0;
    int array_size = 3;
    int *secret_code;
    secret_code = new int[array_size];
    int *result = new int[array_size]{0, 151, 132};
    secret_code[0] = 0;
    for (int i = 1; i < array_size; i++) {
        fill_secret_part(secret_part, i, temp);
        secret_code[i] = secret_part;
    }
    print(secret_code, 0, 3);
    for (int i = 0; i < array_size; i++) {
        EXPECT_EQ(secret_code[i], result[i]);
    }
}

TEST(FunctionTesting5, check5) {
    int temp[] {151, 132, 15};
    int secret_part = 0;
    int array_size = 3;
    int *secret_code;
    secret_code = new int[array_size];
    int *result = new int[array_size]{1, 132, 15};
    secret_code[0] = 0;
    for (int i = 1; i < array_size; i++) {
        fill_secret_part(secret_part, i, temp);
        secret_code[i] = secret_part;
    }
    for (int i = 0; i < array_size; i++) {
        EXPECT_NE(secret_code[i], result[i]);
    }
}

TEST(RandTesting1, check3) {
    int rank = 0;
    int rand_num=get_random_number(0);
    int rand_test= rand() % 255;
    EXPECT_NE(rand_num, rand_test); 
}

TEST(RandTesting2, checkIfEqual) {
    int rank = 0;
    int rand_num1=get_random_number(0);
    int rand_num2=get_random_number(0);
    EXPECT_EQ(rand_num1, rand_num2); 
}

TEST(RandTesting3, check_not_equal) {
    int rand_num1=get_random_number(1);
    int rand_num2=get_random_number(5);
    EXPECT_NE(rand_num1, rand_num2); 
}

#endif




int main(int argc, char** argv) {
    #ifdef TEST
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    #endif
        MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *secret_code;
    int secret_part;
    MPI_Status status;

    if (rank == 0) {
    	secret_code = new int[size];
    	secret_code[0] = 10;
        for (int i = 1; i < size; i++) {
       	    MPI_Recv(&secret_part, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
       	    secret_code[i] = secret_part;
        }
    }
    
    if (rank != 0) {
        secret_code = new int[size];
        int temp[] {5435,345,35,24,6546,35,36,546,35326,546,457,35,234,34,645,74,57,568,679};
        fill_secret_part(secret_part, rank, temp);
        MPI_Send(&secret_part, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Bcast(secret_code, size, MPI_INT, 0, MPI_COMM_WORLD);
        

    MPI_Barrier(MPI_COMM_WORLD);
    print(secret_code, rank, size);

    MPI_Finalize();
    delete [] secret_code;
    return 0;
}