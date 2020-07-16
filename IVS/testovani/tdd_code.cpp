//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     RADEK SVEC <xsvecr01@stud.fit.vutbr.cz>
// $Date:       $2019-02-23
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author RADEK SVEC
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{ 
    //prochazeni seznamu
    for(Element_t *tmp_e = m_pHead; tmp_e != NULL; tmp_e = tmp_e->pNext)
    {
        //smazani docasneho prvku 'del_e'
        Element_t *del_e = tmp_e;
        delete(del_e);   
    }
}

void PriorityQueue::Insert(int value)
{
    //vytvoreni noveho prvku 'e'
    Element_t *e = new Element_t();
    e->value = value;
    e->pNext = NULL;
    e->pPrev = NULL;

    //kontrola jestli neni seznam prazdny
    if (m_pHead != NULL)
    {
        //prochazeni seznamu
        for (Element_t *tmp_e = m_pHead; tmp_e != NULL; tmp_e = tmp_e->pNext)
        {
            //pokud je value prvku 'tmp_e' seznamu vetsi nez value prvku 'e'
            //prvek 'e' se vlozi pred nej
            if (tmp_e->value >= e->value)
            {
                e->pNext = tmp_e;
                e->pPrev = tmp_e->pPrev;
                
                //seznam ma jen jeden prvek
                if (tmp_e == m_pHead)
                {
                    m_pHead = e;
                }
                else
                {
                    tmp_e->pPrev->pNext = e;
                }
                tmp_e->pPrev = e;
                break;
            }
            //vkladany prvek 'e' ma nejvyssi value -> vklada se na konec seznamu
            else if (tmp_e->pNext == NULL)
            {
                tmp_e->pNext = e;
                e->pPrev = tmp_e;
                break;
            }
        }
    }
    //seznam je prazdny
    else m_pHead = e;
}

bool PriorityQueue::Remove(int value)
{
    //hledani prvku s hodnotou value
    Element_t *e = Find(value);
    //prvek s hodnotou value existuje
    if(e != NULL)
    {
        //prvek je prvni v seznamu
        if(e->pPrev == NULL && e->pNext != NULL)
        {
            e->pNext->pPrev = NULL;
            m_pHead = e->pNext;
        }
        //prvek je posledni v seznamu
        else if(e->pNext == NULL && e->pPrev != NULL)
        {
            e->pPrev->pNext = NULL;
        }
        //seznam ma jen jeden prvek
        else m_pHead = NULL;
        delete(e);
        return true;
    }
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    //prochazeni seznamu
    for (Element_t *tmp_e = m_pHead; tmp_e != NULL; tmp_e = tmp_e->pNext)
    {
        //porovnavani hodnoty
        if (tmp_e->value == value)
        {
            return tmp_e;
        }
    }

    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/
