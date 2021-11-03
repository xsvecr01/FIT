/**
 * @file FileParser.hpp
 * @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 * @date 2020-11-17
 * 
 * @brief Hlavickovy soubor
 * 
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>

/**
 * @brief Trida pro operace se souborem
 * 
 */
class FileParser
{
public:

    /**
     * @brief Defaultni konstruktor
     * 
     */
    FileParser();

    /**
     * @brief Konstuktor pro FileParser
     * 
     * @param filename Nazev souboru
     */
    FileParser(std::string filename);

    /**
     * @brief Pomocna funkce pro kontrolu domen v souboru
     * 
     * @return true Nebyla nalezena zadna domena
     * @return false Domeny jsou v poradku
     */
    bool isEmpty();

    /**
     * @brief Kontroluje, zda se domena nachazi na blacklistu
     * 
     * @param address Kontrolovana domena
     * @return true Domena je zakazana
     * @return false Domena neni zakazana
     */
    bool isBlacklisted(std::string address);

private:
    std::string _filename;
    std::unordered_set<std::string> _domains;

    /**
     * @brief Funkce uklada domeny ze souboru do hash setu. Zaroven odstranuje prazdne radky a komentare
     * 
     * @return std::unordered_set<std::string> Hash set s domenama
     */
    std::unordered_set<std::string> getList();

    /**
     * @brief Kontrolni funkce pro existenci souboru
     * 
     * @return true Soubor existuje
     * @return false Soubor nebyl nalezen
     */
    bool fileExists();

    /**
     * @brief Pomocna funkce pro odstraneni mezer
     * 
     * @param str Zpracovavany string
     * @return std::string Vystup bez mezer
     */
    std::string removeSpaces(std::string str);
};