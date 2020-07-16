##
 # @mainpage Projekt 3 - dokumentace
 #
 # @file calc.py
 # @author Radek Svec (xsvecr01@stud.fit.vutbr.cz)
 # @date 2018/19
 # @brief Projekt 2 IVS: kalkulacka
 #
 #

import kivy

from kivy.config import Config
Config.set('input', 'mouse', 'mouse,multitouch_on_demand')
Config.set('kivy','window_icon','icon.png')
Config.set('graphics', 'width', '400')
Config.set('graphics', 'height', '400')

from kivy.properties import StringProperty
from kivy.app import App
from kivy.uix.gridlayout import GridLayout
from kivy.uix.screenmanager import ScreenManager, Screen
kivy.require("1.9.0")

from Math_Lib import Solvation


##
# @class trida pro Screen s hlavnim rozhranim kalkulacky
#

class CalcGridLayout(Screen):

    ##
    # @brief string, do ktereho se uklada vyraz
    #
    expression = StringProperty()
    expression = ""
    
    ##
    # @brief vypocet vyrazu zadaneho v rozhrani kalkulacky
    #
    # vypise na textove pole kalkulacky vysledek
    #
    # @param self rodic funkce (trida CalcGridLayout)
    #
    
    def solve(self):
        try:
            self.expression = str(Solvation.m_solve(self.expression))
            self.display.text = self.expression
        except Exception:
            self.display.text = "Error"
            self.expression = ""


    ##
    # @brief mazani cislice/operace od konce vkladaneho vyrazu
    #
    # @param self rodic funkce (trida CalcGridLayout)
    #
    
    def delete(self):
        if self.expression[-1:].isnumeric():
            self.expression = self.expression[:-1]

        elif self.expression[-1:] == '.':
            self.expression = self.expression[:-1]

        else:
            self.expression = self.expression[:-3]

        if self.display.text[-1:] == 'n':
            self.display.text = self.display.text[:-2]

        elif self.display.text[-1:] == ')':
            self.display.text = self.display.text[:-3]
            
        else:
            self.display.text = self.display.text[:-1]    


    ##
    # @brief prevraceni znamenka u cisla - zaporne/kladne cislo
    #
    # @param self rodic funkce (trida CalcGridLayout)
    #
    
    def invert(self):
        if self.expression[-1:] == '-':
            self.expression = self.expression[:-1]
            self.display.text = self.display.text[:-1]
            
        else:
            self.expression += '-'
            self.display.text += '(-)'


##
# @class trida pro Screen s napovedou kalkulacky
#

class Help(Screen):

    ##
    # @brief string, obsahujici text napovedy
    #
    
    helpText = StringProperty()
    helpText ='''
[size=25][b]Nápověda[/b][/size]

[size=23][b]Funkce[/b][/size]

[size=20][b]x^y[/b][/size]
  Mocnina:
  x - základ mocniny
  y - exponent

[size=20][b]x\u221ay[/b][/size]
  Odmocnina:
  x - odmocnitel
  y - základ odmocniny
  (x-tá odmocnina z y)

[size=20][b]x![/b][/size]
  Faktoriál:
  x - základ
  např. pro x = 5:
  5! = 5*4*3*2*1 = 120

  Není definováno pro záporné čísla

[size=20][b]ln(x)[/b][/size]
  Přirozený logaritmus:
  x - logaritmický základ

  Není definováno pro čísla menší nebo rovna 0

[size=20][b]/[/b][/size]
  Dělení

[size=20][b]*[/b][/size]
  Násobení

[size=20][b]-[/b][/size]
  Odčítání

[size=20][b]+[/b][/size]
  Sčítání

[size=20][b]=[/b][/size]
  Výpočet zadaného výrazu

[size=20][b]AC[/b][/size]
  Vymazání celého příkazu/výsledku

[size=20][b]DEL[/b][/size]
  Mazání poslední zadané číslice/operace

[size=23][b]Poznámky k zadávání výrazů:[/b][/size]
Kalkulačka umí přednost daných operací následovně:
Nejprve se provedou funkce Mocnina, Odmocnina, Faktoriál, Logaritmus;
dále operace násobení a dělení;
a nakonec sčítání a odčítání.
Kalkulačka podporuje "zřetězené" zadávání:
např. 5+5=10 -2=8

Kalkulačka NEumí:
Počítat výraz, který má návaznost funkcí mocniny, odmociny, faktoriálu, logaritmu:
např. ln(5!^2)
Počítat nestandardně zadané výrazy:
např. 5++6; 6*+5; 4/+2
V takových případech vypíše místo výsledku "Error"

[size=20][b]Poznámka[/b][/size]
Při dlouhém výsledku se lze posunout doleva označením výsledku.
'''


##
# @class trida pro celkove sestaveni aplikace
#

class CalculatorApp(App):


    ##
    # @brief funkce sestavi aplikaci
    #
    # @param self rodic funkce (trida CalcGridLayout)
    #
    # @return ScreenManager rozhrani aplikace
    #
    
    def build(self):
        self.title = 'Calculatron'
        sm = ScreenManager()
        sm.add_widget(CalcGridLayout(name='calculator'))
        sm.add_widget(Help(name='help'))
        return sm

calcApp = CalculatorApp()
calcApp.run()
