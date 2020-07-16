//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     RADEK SVEC <xsvecr01@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author RADEK SVEC
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
protected:
    virtual void SetUp() {
        for(int i = 1; i <= 10; i++)
            tree.InsertNode(i);
    }

    BinaryTree tree;
};



TEST_F(EmptyTree, InsertNode)
{
    EXPECT_TRUE(tree.GetRoot() == NULL);

    EXPECT_TRUE(std::get<0>(tree.InsertNode(10)));
    EXPECT_TRUE(tree.GetRoot() != NULL);
    EXPECT_EQ(tree.GetRoot()->key, 10);

    EXPECT_FALSE(std::get<0>(tree.InsertNode(10)));
    EXPECT_EQ(std::get<1>(tree.InsertNode(10))->key, 10);

}

TEST_F(EmptyTree, DeleteNode)
{
    EXPECT_FALSE(tree.DeleteNode(0));
}

TEST_F(EmptyTree, FindNode)
{
    EXPECT_TRUE(tree.FindNode(0) == NULL);
}



TEST_F(NonEmptyTree, InsertNode)
{
    EXPECT_TRUE(tree.GetRoot() != NULL);

    EXPECT_FALSE(std::get<0>(tree.InsertNode(10)));

    EXPECT_TRUE(std::get<0>(tree.InsertNode(11)));
    EXPECT_EQ(std::get<1>(tree.InsertNode(11))->key, 11);

}

TEST_F(NonEmptyTree, DeleteNode)
{
    EXPECT_FALSE(tree.DeleteNode(0));

    for(int i = 1; i <= 10; i++)
    {
        EXPECT_TRUE(tree.DeleteNode(i));
    }

    EXPECT_FALSE(tree.DeleteNode(1));

}

TEST_F(NonEmptyTree, FindNode)
{
    EXPECT_FALSE(tree.FindNode(0));
    EXPECT_TRUE(tree.FindNode(5));
    
    tree.InsertNode(11);
    EXPECT_TRUE(tree.FindNode(11));
}


/*** Konec souboru black_box_tests.cpp ***/
