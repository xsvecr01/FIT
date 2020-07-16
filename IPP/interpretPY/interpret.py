######################################
# IPP proj2 2020 - interpret.py
# autor: Radek Svec (xsvecr01@stud.fit.vutbr.cz)
######################################

from __future__ import print_function
import sys
import xml.etree.ElementTree as ET
import re



##########################
###                    ###
###  parse argumentu   ###
###                    ###
##########################
# zpracovavani argumentu, vraci polozky s xml reprezentaci kodu a vstupni data

def argparse():
    if len(sys.argv) == 2:
        if sys.argv[1] == '--help':
            print('''Napoveda:
argumenty skritpu:
--help:        vypise napovedu
--source=file: vstupni soubor s XML reprezentaci zdrojoveho kodu
--input=file:  soubor se vstupy pro interpretaci kodu
Alespon jeden z parametru (--source nebo --input) musi byt vzdy zadan. Pokud jeden chybi, nacitaji se odpovidajici data ze standardniho vstupu.''')
            sys.exit(0)

        elif sys.argv[1][:9] == '--source=':
            try:
                f = open(sys.argv[1][9:], 'r')
                sourcef = f.read()
                f.close()
                inputf = ''
                for line in sys.stdin:
                    inputf += line
            except:
                sys.exit(11)

        elif sys.argv[1][:8] == '--input=':
            try:
                f = open(sys.argv[1][8:], 'r')
                inputf = f.read()
                f.close()
                sourcef = ''
                for line in sys.stdin:
                    sourcef += line
            except:
                sys.exit(11)

    elif len(sys.argv) == 3:
        if(sys.argv[1][:9] == '--source=' and sys.argv[2][:8] == '--input='):
            try:
                f = open(sys.argv[1][9:], 'r')
                sourcef = f.read()
                f.close()
                f = open(sys.argv[2][8:], 'r')
                inputf = f.read()
                f.close()
            except:
                sys.exit(11)
        
        elif(sys.argv[1][:8] == '--input=' and sys.argv[2][:9] == '--source='):
            try:
                f = open(sys.argv[2][9:], 'r')
                sourcef = f.read()
                f.close()
                f = open(sys.argv[1][8:], 'r')
                inputf = f.read()
                f.close()
            except:
                sys.exit(11)
    
    else:
        sys.exit(10)

    return(sourcef, inputf)


######################################################
###                                                ###
###        pomocne funkce pro kontrolu typu        ###
###                                                ###
######################################################

#kontroluje pokud je vlozena promenna typu symbol
#vraci True pokud je symbol, jinak False
def issymb(m_type, value):
    if re.match(r'^bool$',m_type) and re.match(r'^true$|^false$', value, re.I):
        return True
    elif re.match(r'^int$',m_type) and re.match(r'^[+|-]?\d+$', value):
        return True
    elif re.match(r'^nil$',m_type) and re.match(r'^nil$', value):
        return True
    elif re.match(r'^string$',m_type):
        return True
    elif isvar(m_type, value):
        return True
    else:
        return False

#kontroluje pokud je vlozena promenna typu label
#vraci boolean
def islabel(m_type, value):
    if re.match(r'^label$', m_type) and re.match(r'^[a-zA-Z_\-$&%!?*][a-zA-Z0-9_\-$&%!?*]*$', value):
        return True
    else:
        return False

#kontrola typu var
#vraci boolean
def isvar(m_type, value):
    if re.match(r'^var$', m_type) and re.match(r'^(LF|TF|GF)@[a-zA-Z_\-$&%!?*][a-zA-Z0-9_\-$&%!?*]*$', value):
        return True
    else:
        return False

#kontrola typu type
#vraci boolean
def istype(m_type, value):
    if re.match(r'^type$', m_type) and re.match(r'^int|bool|string$', value):
        return True
    else:
        return False


######################################################
###                                                ###
###        FUNKCE PRO SYNTAKTICKOU ANALYZU         ###
###                                                ###
######################################################
# kontroluje syntaxi vstupniho xml programu
# instrukce uklada do slovniku ve tvaru {opcode: {arg0, type0}}
# pokud nedoslo k chybe, vraci slovnik s instrukcemi
# v pripade chyby ukonci program s kodem 32

def checksyntax(root, input):
    instdict= {}
    if root.get('language') != "IPPcode20":
        sys.exit(32)

    try:
        checkord = []
        for i in root:
            if i.tag != 'instruction':
                sys.exit(32)

        for i in root.findall('instruction'):
            if int(i.get('order')) < 0:
                sys.exit(32)
            checkord.append(i.get('order'))
        
        if len(checkord) != len(set(checkord)):
            sys.exit(32)

        for instr in root.findall('instruction'):
            opcode = (instr.get('opcode')).upper()

            if instr.tag != 'instruction':
                sys.exit(32)

            #-----------
            if len(list(instr)) == 0 and (opcode == "CREATEFRAME" or opcode == "PUSHFRAME" or opcode == "POPFRAME" or opcode == "RETURN" or opcode == "BREAK"):
                instdict.update({int(instr.get('order')) : {'opcode' : opcode}})

            #-----------symb
            elif len(list(instr)) == 1 and (opcode == "PUSHS" or opcode == "EXIT" or opcode == "DPRINT" or opcode == "WRITE"):
                if issymb(instr[0].get('type'), instr[0].text) and instr[0].tag == 'arg1':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text}})
                else:
                    sys.exit(32)

            #-----------label
            elif len(list(instr)) == 1 and (opcode == "CALL" or opcode == "LABEL" or opcode == "JUMP"):
                if(islabel(instr[0].get('type'), instr[0].text)) and instr[0].tag == 'arg1':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text}})
                else:
                    sys.exit(32)

            #-----------var
            elif len(list(instr)) == 1 and (opcode == "DEFVAR" or opcode == "POPS"):
                if(isvar(instr[0].get('type'), instr[0].text)) and instr[0].tag == 'arg1':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text}})
                else:
                    sys.exit(32)

            #-----------var type
            elif len(list(instr)) == 2 and (opcode == "READ"):
                if(isvar(instr[0].get('type'), instr[0].text)) and (istype(instr[1].get('type'), instr[1].text)) and instr[0].tag == 'arg1' and instr[1].tag == 'arg2':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text, 'type1' : instr[1].get('type'), 'val1' : instr[1].text}})
                else:
                    sys.exit(32)

            #-----------var symb
            elif len(list(instr)) == 2 and (opcode == "MOVE" or opcode == "NOT" or opcode == "INT2CHAR" or opcode == "STRLEN" or opcode == "TYPE"):
                if(isvar(instr[0].get('type'), instr[0].text)) and (issymb(instr[1].get('type'), instr[1].text)) and instr[0].tag == 'arg1' and instr[1].tag == 'arg2':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text, 'type1' : instr[1].get('type'), 'val1' : instr[1].text}})
                else:
                    sys.exit(32)

            #-----------label symb symb
            elif len(list(instr)) == 3 and (opcode == "JUMPIFEQ" or opcode == "JUMPIFNEQ"):
                if(islabel(instr[0].get('type'), instr[0].text) and issymb(instr[1].get('type'), instr[1].text) and issymb(instr[2].get('type'), instr[2].text)) and instr[0].tag == 'arg1' and instr[1].tag == 'arg2' and instr[2].tag == 'arg3':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text, 'type1' : instr[1].get('type'), 'val1' : instr[1].text, 'type2' : instr[2].get('type'), 'val2' : instr[2].text}})
                else:
                    sys.exit(32)

            #-----------var symb symb
            elif len(list(instr)) == 3 and (opcode == "ADD" or opcode == "SUB" or opcode == "MUL" or opcode == "IDIV" or opcode == "LT" or opcode == "GT" or opcode == "EQ" or opcode == "AND" or opcode == "OR" or opcode == "STRI2INT" or opcode == "CONCAT" or opcode == "GETCHAR" or opcode == "SETCHAR"):
                if(isvar(instr[0].get('type'), instr[0].text)) and (issymb(instr[1].get('type'), instr[1].text)) and (issymb(instr[2].get('type'), instr[2].text)) and instr[0].tag == 'arg1' and instr[1].tag == 'arg2' and instr[2].tag == 'arg3':
                    instdict.update({int(instr.get('order')) : {'opcode' : opcode, 'type0' : instr[0].get('type'), 'val0' : instr[0].text, 'type1' : instr[1].get('type'), 'val1' : instr[1].text, 'type2' : instr[2].get('type'), 'val2' : instr[2].text}})
                else:
                    sys.exit(32)
            else:
                sys.exit(32)
    except:
        sys.exit(32)

    return instdict

#Globalni promenne - ramce a zasobniky
GF = {}
LF = []
TF = None
STACK = []
Labels = {}
tmp_pCNT = []
# programovy citac
pCNT = 0

#pomocna funkce pro ziskani promenne a jejiho typu
def get_var(var):
    global GF, LF, TF
    var = re.sub(r'\\\d{3}', escape, var)
    if var[0:2] == 'GF':
        var = GF.get(var[3:])
    elif var[0:2] == 'LF':
        if len(LF) == 0:
            sys.exit(55)
        var = LF[-1].get(var[3:])
    elif var[0:2] == 'TF':
        if TF == None:
            sys.exit(55)
        var = TF.get(var[3:])
    if var != None:
        return (var)
    else:
        sys.exit(54)

#pomocna funkce pro ulozeni promenne
def set_var(var, value, m_type = ''):
    global GF, LF, TF
    var = re.sub(r'\\\d{3}', escape, var)
    if var[0:2] == 'GF':
        GF.update({var[3:] : {'val' : value, 'type' : m_type}})
    if var[0:2] == 'LF':
        if len(LF) == 0:
            sys.exit(55)
        LF[-1].update({var[3:] : {'val' : value, 'type' : m_type}})
    elif var[0:2] == 'TF':
        if TF == None:
            sys.exit(55)
        TF.update({var[3:] : {'val' : value, 'type' : m_type}})


#definice labelu a kontrola duplicity labelu
def check_labels(sor_dict):
    for i in range(len(sor_dict)):
        line = sor_dict.get(i)
        if line['opcode'] == 'LABEL':
            for x in Labels:
                if x == line['val0']:
                    sys.exit(52)
            Labels.update({line['val0'] : i})

#preklad escape sekvence na znak
def escape(var):
    var = var.group()
    return chr(int(var[1:]))



##########################################
########                         #########
########      INTERPRETACE       #########
########                         #########
##########################################
#
# Hlavni funkce, stara se o vykonavani instrukci
#

def run_code(line):
    global GF, LF, TF, pCNT, STACK, tmp_pCNT

    opcode = line['opcode']
    
    try:
        line['val0'] = re.sub(r'\\\d{3}', escape, line['val0'])
        line['val1'] = re.sub(r'\\\d{3}', escape, line['val1'])
        line['val2'] = re.sub(r'\\\d{3}', escape, line['val2'])
    except:
        pass

######################################################

    #prace s datovym zasobnikem

######################################################
    if(opcode == 'POPS'):
        if len(STACK) == 0:
            sys.exit(56)
        valtype = STACK.pop()
        set_var(line['val0'], valtype[0], valtype[1])

    elif(opcode == 'PUSHS'):
        STACK.append([line['val0'], line['type0']])


######################################################

    #prace s ramci, volani funkci

######################################################
    elif(opcode == 'MOVE'):
        if line['type1'] == 'var':
            symb = get_var(line['val1'])
            m_type = symb.get('type')
            symb = symb.get('val')
        else:
            symb = line['val1']
            m_type = line['type1']
        
        set_var(line['val0'], symb, m_type)


    elif(opcode == 'CREATEFRAME'):
        TF = {}

    elif(opcode == 'PUSHFRAME'):
        if TF != None:
            LF.append(TF)
            TF = None
        else:
            sys.exit(55)

    elif(opcode == 'POPFRAME'):
        if len(LF) != 0:
            TF = LF.pop()
        else:
            sys.exit(55)

    elif(opcode == 'DEFVAR'):
        if line['val0'][0:2] == 'GF':
            if line['val0'][3:] in GF.keys():
                sys.exit(52)
        
        elif line['val0'][0:2] == 'LF':
            if LF:
                if line['val0'][3:] in LF[-1].keys():
                    sys.exit(52)
        elif line['val0'][0:2] == 'TF':
            if TF:
                if line['val0'][3:] in TF.keys():
                    sys.exit(52)

        set_var(line['val0'], None, line['type0'])

    elif(opcode == 'CALL'):
        if Labels.get(line['val0']) != None:
            tmp_pCNT.append(pCNT)
            pCNT = int(Labels.get(line['val0']))
        else:
            sys.exit(52)

    elif(opcode == 'RETURN'):
        if len(tmp_pCNT) != 0:
            pCNT = tmp_pCNT.pop()
        else:
            sys.exit(56)


###############################################################

    #aritmeticke, relacni, booleovske a konverzni instrukce

###############################################################
# Na zacatku kazde instrukce v teto skupine se provadi prirazeni a kontrola typu a hodnot

    elif(opcode == 'ADD'):
        type1 = type2 = val1 = val2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'int':
            val1 = get_var(line['val1']).get('val')
            type1 = 'int'
        elif line['type1'] == 'int':
            val1 = line['val1']
            type1 = 'int'
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            val2 = get_var(line['val2']).get('val')
            type2 = 'int'
        elif line['type2'] == 'int':
            val2 = line['val2']
            type2 = 'int'

        if type1 == None or type2 == None:
            sys.exit(53)
        if val1 == None or val2 == None:
            sys.exit(56)
            
        try:
            res = int(val1) + int(val2)
            set_var(line['val0'], res, 'int')
        except:
            sys.exit(53)


    elif(opcode == 'SUB'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'int':
            val1 = get_var(line['val1']).get('val')
            type1 = 'int'
        elif line['type1'] == 'int':
            val1 = line['val1']
            type1 = 'int'
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            val2 = get_var(line['val2']).get('val')
            type2 = 'int'
        elif line['type2'] == 'int':
            val2 = line['val2']
            type2 = 'int'

        if type1 == None or type2 == None:
            sys.exit(53)
        if val1 == None or val2 == None:
            sys.exit(56)

        try:
            res = int(val1) - int(val2)
            set_var(line['val0'], res, 'int')
        except:
            sys.exit(53)

    elif(opcode == 'MUL'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'int':
            val1 = get_var(line['val1']).get('val')
            type1 = 'int'
        elif line['type1'] == 'int':
            val1 = line['val1']
            type1 = 'int'
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            val2 = get_var(line['val2']).get('val')
            type2 = 'int'
        elif line['type2'] == 'int':
            val2 = line['val2']
            type2 = 'int'

        if type1 == None or type2 == None:
            sys.exit(53)
        if val1 == None or val2 == None:
            sys.exit(56)

        try:
            res = int(val1) * int(val2)
            set_var(line['val0'], res, 'int')
        except:
            sys.exit(53)

    elif(opcode == 'IDIV'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'int':
            val1 = get_var(line['val1']).get('val')
            type1 = 'int'
        elif line['type1'] == 'int':
            val1 = line['val1']
            type1 = 'int'
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            val2 = get_var(line['val2']).get('val')
            type2 = 'int'
        elif line['type2'] == 'int':
            val2 = line['val2']
            type2 = 'int'
        
        if type1 == None or type2 == None:
            sys.exit(53)
        if val1 == None or val2 == None:
            sys.exit(56)

        #deleni nulou
        if int(val2) == 0:
            sys.exit(57)

        try:
            res = int(val1) // int(val2)
            set_var(line['val0'], res, 'int')
        except:
            sys.exit(53)

    elif(opcode == 'LT'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') != 'nil':
            val1 = get_var(line['val1']).get('val')
            type1 = get_var(line['val1']).get('type')
        elif line['type1'] != 'nil':
            val1 = line['val1']
            type1 = line['type1']
        if line['type2'] == 'var' and get_var(line['val2']).get('type') != 'nil':
            val2 = get_var(line['val2']).get('val')
            type2 = get_var(line['val2']).get('type')
        elif line['type2'] != 'nil':
            val2 = line['val2']
            type2 = line['type2']
        
        if type1 == None or type2 == None:
            sys.exit(53)
        
        if val1 == None:
            if type1 == 'string':
                val1 = ''
            else:
                sys.exit(56)
        if val2 == None:
            if type2 == 'string':
                val2 = ''
            else:
                sys.exit(56)

        if type1 == 'int':
            val1 = int(val1)
        if type2 == 'int':
            val2 = int(val2)

        if type1 == type2:
            if val1 < val2:
                set_var(line['val0'], 'true', 'bool')
            else:
                set_var(line['val0'], 'false', 'bool')


    elif(opcode == 'GT'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') != 'nil':
            val1 = get_var(line['val1']).get('val')
            type1 = get_var(line['val1']).get('type')
        elif line['type1'] != 'nil':
            val1 = line['val1']
            type1 = line['type1']
        if line['type2'] == 'var' and get_var(line['val2']).get('type') != 'nil':
            val2 = get_var(line['val2']).get('val')
            type2 = get_var(line['val2']).get('type')
        elif line['type2'] != 'nil':
            val2 = line['val2']
            type2 = line['type2']
        
        if type1 == None or type2 == None:
            sys.exit(53)
        
        if val1 == None:
            if type1 == 'string':
                val1 = ''
            else:
                sys.exit(56)
        if val2 == None:
            if type2 == 'string':
                val2 = ''
            else:
                sys.exit(56)


        if type1 == 'int':
            val1 = int(val1)
        if type2 == 'int':
            val2 = int(val2)

        if type1 == type2:
            if val1 > val2:
                set_var(line['val0'], 'true', 'bool')
            else:
                set_var(line['val0'], 'false', 'bool')

    elif(opcode == 'EQ'):
        val1 = val2 = type1 = type2 = None
        if line['type1'] == 'var':
            val1 = get_var(line['val1']).get('val')
            type1 = get_var(line['val1']).get('type')
        else:
            val1 = line['val1']
            type1 = line['type1']
        if line['type2'] == 'var':
            val2 = get_var(line['val2']).get('val')
            type2 = get_var(line['val2']).get('type')
        else:
            val2 = line['val2']
            type2 = line['type2']
        
        if type1 == None or type2 == None:
            sys.exit(53)
        if val1 == None and val2 == None:
            sys.exit(56)
        
        if type1 == 'nil' and type2 != 'nil':
            set_var(line['val0'], 'false', 'bool')
        elif type1 != 'nil' and type2 == 'nil':
            set_var(line['val0'], 'false', 'bool')

        elif type1 == type2:
            try:
                if val1 == val2:
                    set_var(line['val0'], 'true', 'bool')
                else:
                    set_var(line['val0'], 'false', 'bool')
            except:
                sys.exit(53)
        else:
            sys.exit(53)


    elif(opcode == 'AND'):
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'bool':
            val1 = get_var(line['val1']).get('val')
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'bool':
            val2 = get_var(line['val2']).get('val')

        if line['type1'] == 'bool':
            val1 = line['val1']
        if line['type2'] == 'bool':
            val2 = line['val2']
        try:
            if(val1.upper() == 'TRUE' == val2.upper()):
                set_var(line['val0'], 'true', 'bool')
            else:
                set_var(line['val0'], 'false', 'bool')
        except:
            sys.exit(53)

    elif(opcode == 'OR'):
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'bool':
            val1 = get_var(line['val1']).get('val')
        elif line['type1'] == 'bool':
            val1 = line['val1']
        else:
            sys.exit(53)
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'bool':
            val2 = get_var(line['val2']).get('val')
        elif line['type2'] == 'bool':
            val2 = line['val2']
        else:
            sys.exit(53)

        if(val1.upper() == 'FALSE' == val2.upper()):
            set_var(line['val0'], 'false', 'bool')
        else:
            set_var(line['val0'], 'true', 'bool')


    elif(opcode == 'NOT'):
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'bool':
            val1 = get_var(line['val1']).get('val')
        elif line['type1'] == 'bool':
            val1 = line['val1']
        else:
            sys.exit(53)

        if(val1.upper() == 'TRUE'):
            set_var(line['val0'], 'false', 'bool')
        else:
            set_var(line['val0'], 'true', 'bool')


    elif(opcode == 'INT2CHAR'):
        symb = line['val1']

        if line['type1'] == 'var':
            symb = get_var(line['val1']).get('val')

        try:
            symb = chr(int(symb))
        except:
            sys.exit(58)
        
        set_var(line['val0'], symb, 'string')


    elif(opcode == 'STRI2INT'):
        text = pos = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'string':
            text = get_var(line['val1']).get('val')
        elif line['type1'] == 'string':
            text = line['val1']
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            pos = int(get_var(line['val2']).get('val'))
        if line['type2'] == 'int':
            pos = int(line['val2'])
        
        if text == None or pos == None:
            sys.exit(53)
        
        if pos >= len(text) or pos < 0:
            sys.exit(58)

        set_var(line['val0'], ord(text[pos]), 'int')

######################################################

    #vstupne vystupni instrukce

######################################################
    
    elif(opcode == 'READ'):
        pass

    elif(opcode == 'WRITE'):
        var = line['val0']
        type0 = ''
        if line['type0'] == 'var':
            type0 = get_var(var).get('type')
            var = get_var(var).get('val')
        
        if line['type0'] == 'nil' or type0 == 'nil':
            var = ''
        if line['type0'] == 'bool' or type0 == 'bool':
            var = str(var).lower()
        
        if line['type0'] == 'string' or type0 == 'string':
            pass

        print(var, end='')


######################################################

    #prace s retezci

######################################################
    elif(opcode == 'CONCAT'):
        val1 = val2 = None
        if line['type1'] == 'var':
            val1 = str(get_var(line['val1']).get('val'))
            type1 = get_var(line['val1']).get('type')
        else:
            val1 = str(line['val1'])
            type1 = line['type1']
        if line['type2'] == 'var':
            val2 = str(get_var(line['val2']).get('val'))
            type2 = get_var(line['val2']).get('type2')
        else:
            val2 = str(line['val2'])
            type2 = line['type2']
        
        
        if val1 == None or val2 == None or type1 != type2:
            sys.exit(53)

        set_var(line['val0'], val1 + val2, 'string')

    elif(opcode == 'STRLEN'):
        val1 = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'string':
            val1 = get_var(line['val1']).get('val')
            if val1 == None:
                val1 = ''
        elif line['type1'] == 'string':
            val1 = line['val1']
            if val1 == None:
                val1 = ''

        else:
            sys.exit(53)

        set_var(line['val0'], len(val1), 'int')

    elif(opcode == 'GETCHAR'):
        text = pos = None
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'string':
            text = get_var(line['val1']).get('val')
        elif line['type1'] == 'string':
            text = line['val1']
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'int':
            pos = get_var(line['val2']).get('val')
        elif line['type2'] == 'int':
            pos = line['val2']
        
        if text == None or pos == None:
            sys.exit(53)

        pos = int(pos)

        if(pos >= len(text) or pos < 0):
            sys.exit(58)
        set_var(line['val0'], text[pos], 'int')

    elif(opcode == 'SETCHAR'):
        text = pos = char = None
        text = get_var(line['val0']).get('val')
        if line['type1'] == 'var' and get_var(line['val1']).get('type') == 'int':
            pos = get_var(line['val1']).get('val')
        if line['type2'] == 'var' and get_var(line['val2']).get('type') == 'string':
            char = get_var(line['val2']).get('val')

        if line['type1'] == 'int':
            pos = int(line['val1'])
        if line['type2'] == 'string':
            char = line['val2']
        
        if text == None or pos == None or char == None:
            sys.exit(53)
        
        if(pos >= len(text) or pos < 0):
            sys.exit(58)
        else:
            text = list(text)
            text[pos] = char[0]
            text = ''.join(text)

        set_var(line['val0'], text, 'int')


######################################################

    #prace s typy

######################################################
    elif(opcode == 'TYPE'):
        if line['type1'] == 'var':
            type1 = (get_var(line['val1'])).get('type')
        else:
            type1 = line['type1']
        
        if type1 == 'var':
            type1 = ''

        set_var(line['val0'], type1, 'type')


######################################################

    #instrukce pro rizeni toku programu

######################################################
    elif(opcode == 'LABEL'):
        # provadi se v jine funkci
        pass

    elif(opcode == 'JUMP'):
        if Labels.get(line['val0']) != None:
            pCNT = int(Labels.get(line['val0']))
        else:
            sys.exit(52)

    elif(opcode == 'JUMPIFEQ'):
        val1 = line['val1']
        val2 = line['val2']
        type1 = line['type1']
        type2 = line['type2']

        if line['type1'] == 'var':
            val1 = get_var(line['val1'])
            type1 = val1.get('type')
            val1 = val1.get('val')
        if line['type2'] == 'var':
            val2 = get_var(line['val2'])
            type2 = val2.get('type2')
            val2 = val2.get('val')

        if type1 == 'int':
            val1 = int(val1)
        if type2 == 'int':
            val2 = int(val2)

        if type1 == type2 or type1 == 'nil' or type2 == 'nil':
            if val1 == val2:
                if Labels.get(line['val0']) != None:
                    pCNT = int(Labels.get(line['val0']))
                else:
                    sys.exit(52)
        else:
            sys.exit(53)

    elif(opcode == 'JUMPIFNEQ'):
        val1 = line['val1']
        val2 = line['val2']
        type1 = line['type1']
        type2 = line['type2']

        if line['type1'] == 'var':
            val1 = get_var(line['val1'])
            type1 = val1.get('type')
            val1 = val1.get('val')
        if line['type2'] == 'var':
            val2 = get_var(line['val2'])
            type2 = val2.get('type2')
            val2 = val2.get('val')

        if type1 == 'int':
            val1 = int(val1)
        if type2 == 'int':
            val2 = int(val2)

        if type1 == type2 or type1 == 'nil' or type2 == 'nil':
            if val1 != val2:
                if Labels.get(line['val0']) != None:
                    pCNT = int(Labels.get(line['val0']))
                else:
                    sys.exit(52)
        else:
            sys.exit(53)

    elif(opcode == 'EXIT'):
        if line['type0'] != 'int':
            sys.exit(53)
        exC = int(line['val0'])
        if exC >= 0 and exC <= 49: 
            sys.exit(exC)
        else:
            sys.exit(57)


######################################################

    #ladici instrukce

######################################################
    elif(opcode == 'DPRINT'):
        if line['type0'] == 'var':
            val = get_var(line['val0']).get('val')
        else:
            val = line['val0']

        print(val, file=sys.stderr)

    elif(opcode == 'BREAK'):
        print("Instruction Count:", pCNT, file=sys.stderr)


######################################################
####################             #####################
#                      M A I N
####################             #####################
######################################################
def main():
    global pCNT
    #kontrola spravneho tvaru xml
    root = ET.fromstring(argparse()[0])
    if root == None:
        sys.exit(31)
    instd = None
    instd = checksyntax(root, argparse()[1])

    sorted_dict = {}

    if(instd):
        #setrideni instrukci podle orderu 
        j = 0
        for i in sorted(instd):
            sorted_dict.update({j : instd.get(i)})
            j += 1
        
        #vyhledani a ulozeni labelu
        check_labels(sorted_dict)

        #volani funkce run_code pro kazdou instrukci zvlast
        while pCNT < len(sorted_dict):
            x = sorted_dict.get(pCNT)
            run_code(x)
            pCNT += 1

if __name__ == "__main__":
    main()