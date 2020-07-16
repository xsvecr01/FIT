//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     RADEK SVEC <xsvecr01@stud.fit.vutbr.cz>
// $Date:       $2019-02-23
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author RADEK SVEC
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class Matrices : public ::testing::Test
{
protected:
    
    Matrix matrix;
    Matrix matrix4x3 = Matrix(4, 3);
    Matrix matrix20 = Matrix(2, 2);
    Matrix matrix20eq = Matrix(2, 2);
    Matrix matrix21 = Matrix(2, 2);
    Matrix matrix30 = Matrix(3, 3);
    Matrix matrix40 = Matrix(4, 4);
    
    virtual void SetUp()
    {
        matrix4x3.set(std::vector<std::vector< double > > {
                {0, 1, 2, 2},
                {3, 4, 5, 5},
                {6, 7, 8, 8}
                });

        matrix20.set(std::vector<std::vector< double > > {
                {0, 1},
                {2, -3}
                });
        
        matrix20eq.set(std::vector<std::vector< double > > {
                {0, 1},
                {2, -3}
                });
    
        matrix21.set(std::vector<std::vector< double > > {
                {0, 4},
                {8, -12}
                });

        matrix30.set(std::vector<std::vector< double > > {
                {5, 3, 2},
                {1, 7, -8},
                {0, -2, 7}
                });
        
        matrix40.set(std::vector<std::vector< double > > {
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
                });
    }
};



TEST_F(Matrices, Construct)
{
    EXPECT_NO_THROW(Matrix());
    EXPECT_NO_THROW(Matrix(3,3));

    EXPECT_ANY_THROW(Matrix(0,0));
    EXPECT_ANY_THROW(Matrix(0,1));
    EXPECT_ANY_THROW(Matrix(1,0));
}

TEST_F(Matrices, SetGet)
{
    EXPECT_TRUE(matrix.set(0,0,5));
    EXPECT_EQ(matrix.get(0,0), 5);

    EXPECT_TRUE(matrix.set(0,0,0));
    EXPECT_EQ(matrix.get(0,0), 0);

    EXPECT_ANY_THROW(matrix.get(0,1));
    EXPECT_ANY_THROW(matrix.get(1,0));
    EXPECT_ANY_THROW(matrix.get(3,3));

    EXPECT_FALSE(matrix.set(1,0,2));
    EXPECT_FALSE(matrix.set(2,5,0));
}

TEST_F(Matrices, SetGetVector)
{
    EXPECT_FALSE(matrix30.set(std::vector<std::vector< double > > {{1,1,1,1}}));
    
    EXPECT_TRUE(matrix30.set(std::vector<std::vector< double > > {
                {78, 6, 6},
                {4, 5, -5},
                {9, -5, 0}
                }));
    
    EXPECT_EQ(matrix30.get(0,0), 78);
}

TEST_F(Matrices, OperationEqual)
{
    EXPECT_TRUE(matrix20.operator==(matrix20eq));

    EXPECT_FALSE(matrix20.operator==(matrix21));

    EXPECT_ANY_THROW(matrix20.operator==(matrix30));   
}

TEST_F(Matrices, OperationPlus)
{
    EXPECT_ANY_THROW(matrix20.operator+(matrix30));

    EXPECT_EQ((matrix20.operator+(matrix21).get(1,0)), 10);
}

TEST_F(Matrices, OperationMultiply)
{
    EXPECT_ANY_THROW(matrix20.operator*(matrix));

    EXPECT_EQ(matrix20.operator*(4), matrix21);

    EXPECT_EQ((matrix21.operator*(matrix20eq)).get(1,0), -24);
}

TEST_F(Matrices, SolveEquation)
{
    EXPECT_ANY_THROW(matrix.solveEquation(std::vector<double> {1}));

    EXPECT_ANY_THROW(matrix4x3.solveEquation(std::vector<double> {2,2,2}));

    EXPECT_ANY_THROW(matrix20.solveEquation(std::vector<double> {2,1,2}));

    matrix.set(0,0,1);

    EXPECT_EQ(matrix.solveEquation(std::vector<double> {1}), std::vector<double> {1});

    EXPECT_EQ(matrix20.solveEquation(std::vector<double> {1, 1}), std::vector<double> ({2, 1}));

    EXPECT_EQ(matrix30.solveEquation(std::vector<double> {3, 5, 3}), std::vector<double> ({-1, 2, 1}));

    EXPECT_EQ(matrix40.solveEquation(std::vector<double> {1, 1, 1, 1}), std::vector<double> ({1, 1, 1, 1}));
}

/*** Konec souboru white_box_tests.cpp ***/
