##
 # 
 # @file Math_Lib.py
 # @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 # @date 2018/19
 # @brief Projekt 2 IVS: kalkulacka
 #
 #

import math

##
# @class trida pro operace z knihovny Math Library
#
class Operations:

    ##
    # @brief soucet dvou cisel
    #
    # @param x prvni scitanec
    # @param y druhy scitanec
    #
    # @return soucet cisel a a b

    @staticmethod
    def m_add(x, y):
        pass
        return float(x + y)
    
    
    ##
    # @brief rozdil dvou cisel
    #
    # @param x prvni odcitanec
    # @param y druhy odcitanec
    #
    # @return rozdil cisel a a b

    @staticmethod
    def m_sub(x, y):
        pass
        return float(x - y)
    
    
    ##
    # @brief nasobeni dvou cisel
    #
    # @param x prvni nasobenec
    # @param y druhy nasobenec
    #
    # @return nasobek cisel a a b

    @staticmethod
    def m_mul(x, y):
        pass
        return float(x * y)
    
    
    ##
    # @brief deleni dvou cisel
    #
    # @param x prvni delenec
    # @param y druhy delenec
    #
    # @pre y nesmi byt 0
    #
    # @return vysledek po deleni cisel a a b

    @staticmethod
    def m_div(x, y):
        pass
        if y:
            return float(x / y)
        else:
            raise ZeroDivisionError('DivideByZero')

    
    ##
    # @brief mocnina dvou cisel
    #
    # @param x zaklad mocniny
    # @param y exponent
    #
    # @return vysledek po a na b

    @staticmethod
    def m_pow(x, y):
        pass
        return math.pow(x, y)

    
    ##
    # @brief odmocnina dvou cisel
    #
    # @param x odmocnitel
    # @param y zaklad odmocniny
    #
    # @pre y nesmi byt 0
    #
    # @return x-ta odmocnina z y

    @staticmethod
    def m_root(x, y):
        pass
        if y:
            return math.pow(x, 1/y)
        else:
            raise ValueError('ZeroRoot')

    
    ##
    # @brief prirozeny logaritmus
    #
    # @param x zaklad
    #
    # @pre x musi byt vetsi nez 0 
    #
    # @return prirozeny logaritmus z a

    @staticmethod
    def m_log(x):
        pass
        return math.log(x)

    
    ##
    # @brief faktorial z cisla
    #
    # @param x cislo pro faktorial
    #
    # @pre x nesmi byt mensi nez 0
    #
    # @return faktorial z cisla a

    @staticmethod
    def m_fact(x):
        pass
        if x == 0:
            return 1

        elif x > 170:
            return float("inf")

        elif x > 0:
            f = 1
            i = 1

            while i <= x:
                f = f * i
                i += 1

            if x <= 18:
                return f

            else:
                return float('{:.6e}'.format(f))

        else:
            raise ValueError('NegativeFactorial')

##
# @class trida pro vypocet vyrazu
#     

class Solvation:

    ##
    # @brief Vypocet vyrazu
    #
    # @param text string obsahujici vyraz
    # string musi byt ve formatu kde kazda operace ma z obou stran mezeru 
    #
    # @return vypocitany vysledek zadaneho vyrazu
    
    def m_solve(text):
        expression = text.replace('  ', ' ')
        expression = expression.split(' ')
        # string je rozdelen podle mezer, v temp jsou jednotlive cisla a operace

        #prvni projiti vyrazu - provadi se funkce fact, pow, root a log
        def first_run(div_exp):
            for pos, temp in enumerate(div_exp):
                if temp == 'f':
                    if div_exp[pos-1] == None:
                        div_exp[pos-1] = 0
                    div_exp[pos] = Operations.m_fact(float(div_exp[pos-1]))
                    del div_exp[pos-1]
                    first_run(div_exp)

                elif temp == 'p':
                    div_exp[pos-1] = Operations.m_pow(float(div_exp[pos-1]), float(div_exp[pos+1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    first_run(div_exp)

                elif temp == 'r':
                    div_exp[pos-1] = Operations.m_root(float(div_exp[pos+1]), float(div_exp[pos-1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    first_run(div_exp)

                elif temp == 'l':
                    div_exp[pos] = Operations.m_log(float(div_exp[pos+1]))
                    del div_exp[pos+1]
                    first_run(div_exp)

                    
        #druhe projiti vyrazu - provadi se funkce mul, div
        def second_run(div_exp):
            for pos, temp in enumerate(div_exp):
                if temp == '*':
                    div_exp[pos-1] = Operations.m_mul(float(div_exp[pos-1]), float(div_exp[pos+1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    second_run(div_exp)

                if temp == '/':
                    div_exp[pos-1] = Operations.m_div(float(div_exp[pos-1]), float(div_exp[pos+1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    second_run(div_exp)


        #treti projiti vyrazu - provadi se funkce add, sub
        def third_run(div_exp):
            for pos, temp in enumerate(div_exp):
                if temp == '+':
                    div_exp[pos-1] = Operations.m_add(float(div_exp[pos-1]), float(div_exp[pos+1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    third_run(div_exp)

                if temp == '-':
                    div_exp[pos-1] = Operations.m_sub(float(div_exp[pos-1]), float(div_exp[pos+1]))
                    del div_exp[pos]
                    del div_exp[pos]
                    third_run(div_exp)

        #volani pruchodu vyrazu
        first_run(expression)
        second_run(expression)
        third_run(expression)

        for i in expression:
            if i != '0' and i != '':
                return float(i)

        return(float(expression[0]))
