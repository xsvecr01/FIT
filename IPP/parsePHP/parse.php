<?php
/*
soubor: parse.php
autor: Radek Svec (xsvecr01)
*/

if($argc == 2)
{
    if($argv[1] == "--help")
    {
        fwrite(STDOUT, "Skript nacita ze standardniho vstupu program v jazyce IPPcode20, provadi kontrolu vstupu a na standardni vystup tiskne XML reprezentaci vstupniho programu.\n");
        exit(0);
    }
    else
    {
        fwrite(STDERR, "Chyba 10\n");
        exit(10);
    }
}

//kontrola jestli se jedna o konstantu
function m_isconst($const) {
    if(preg_match('/^int@[+|-]?\d+$/', $const))
    {
        return "int";
    }
    elseif(preg_match('/^bool@(true|false)$/', $const))
    {
        return "bool";
    }
    elseif(preg_match('/^nil@nil$/', $const))
    {
        return "nil";
    }
    elseif(preg_match('/^string@/', $const))
    {
        return "string";
    }
}
//kontrola jestli se jedna o promennou
function m_isvar($var)
{
    if(preg_match('/^(LF|TF|GF)@[a-zA-Z_\-$&%!?*][a-zA-Z0-9_\-$&%!?*]*$/', $var))
    {
        return "var";
    }
}
//kontrola jestli se jedna o label
function m_islabel($label)
{
    if(preg_match('/^[a-zA-Z_\-$&%!?*][a-zA-Z0-9_\-$&%!?*]*$/', $label))
    {
        return "label";
    }
}
//kontrola jestli se jedna o type
function m_istype($type)
{
    if(preg_match('/^int|bool|string$/', $type))
    {
        return $type;
    }
}
//kontrola jestli se jedna o symbol, vraci typ
function m_issymb($symb)
{
    if(preg_match('/^(LF|TF|GF)@[a-zA-Z_\-$&%!?*][a-zA-Z0-9_\-$&%!?*]*$/', $symb))
    {
        return "var";
    }
    if(preg_match('/^int@[+|-]?\d+$/', $symb))
    {
        return "int";
    }
    if(preg_match('/^bool@(true|false)$/', $symb))
    {
        return "bool";
    }
    if(preg_match('/^nil@nil$/', $symb))
    {
        return "nil";
    }
    if(preg_match('/^string@/', $symb))
    {
        return "string";
    }
    if(m_istype($symb))
    {
        return "type";
    }
    if(m_islabel($symb))
    {
        return "label";
    }
}
//generovani xml kodu
function m_genxml($line, $outputXML, $key)
{
    $instruc = strtoupper($line[0]);
    //<insruction>
    xmlwriter_start_element($outputXML, 'instruction');
    xmlwriter_write_attribute($outputXML, 'order', $key);
    xmlwriter_write_attribute($outputXML, 'opcode', $instruc);
    for($j = 1; $j < count($line); $j++)
    {
        //  <arg>
        xmlwriter_start_element($outputXML, 'arg'. ($j));
        if(($instruc == "JUMP" || $instruc == "LABEL" || $instruc == "JUMPIFEQ" || $instruc == "JUMPIFNEQ") && $j == 1){
            xmlwriter_write_attribute($outputXML, 'type', 'label');
        }
        else{
            xmlwriter_write_attribute($outputXML, 'type', m_issymb($line[$j]));
        }
        $line[$j] = preg_replace('/^string@|int@|bool@|nil@/', '', $line[$j]);
        if($line[$j])
        {
            xmlwriter_text($outputXML, $line[$j]);
        }
        xmlwriter_end_element($outputXML);
        // </arg>
    }
    //</instruction>
    xmlwriter_end_element($outputXML);
}

//odstraneni komentaru
$input = preg_replace('/#.*/', "", trim(stream_get_contents(STDIN)));
//odstraneni prazdnych radku
$input = preg_replace('/  */', " ", (preg_replace('/\t+/', " ", $input)));
$input = preg_replace('/^\n*/', '', $input);

//rozdeleni na radky
$input = explode(PHP_EOL, $input);
$input = preg_replace('/\s+/', ' ', $input);

//rozdeleni radku na slova
for($i = 0; $i < count($input); $i++)
{
    $input[$i] = trim($input[$i]);
    $input[$i] = explode(' ', $input[$i]);
}

//kontrola hlavicky
if(strtoupper($input[0][0]) != ".IPPCODE20"){
    fwrite(STDERR, "Chyba 21\n");
    exit(21);
}
else{
    $outputXML = xmlwriter_open_memory();
    xmlwriter_set_indent($outputXML, 1);
    xmlwriter_set_indent_string($outputXML, ' ');
    xmlwriter_start_document($outputXML, '1.0', 'UTF-8');
    xmlwriter_start_element($outputXML, 'program');
    xmlwriter_write_attribute($outputXML, 'language', 'IPPcode20');
}

$key = 1;
$header = 0;
for($i = 0; $i < count($input); $i++)
{
    if(!$input[$i][0])
    {
        continue;
    }

    $instr = strtoupper($input[$i][0]);

    //3 operandy var symb symb
    if($instr == "ADD" || $instr == "SUB" || $instr == "MUL" || $instr == "IDIV" ||
    $instr == "LT" || $instr == "GT" || $instr == "EQ" || $instr == "AND" || $instr == "OR" ||
    $instr == "STRI2INT" || $instr == "CONCAT" || $instr == "GETCHAR" || $instr == "SETCHAR")
    {
        if(count($input[$i]) == 4 && m_isvar($input[$i][1]) && (m_isconst($input[$i][2]) || m_isvar($input[$i][2])) && (m_isconst($input[$i][3]) || m_isvar($input[$i][3])))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }
    //3 operandy label symb symb
    elseif($instr == "JUMPIFEQ" || $instr == "JUMPIFNEQ"){
        if(count($input[$i]) == 4 && m_islabel($input[$i][1]) && (m_isconst($input[$i][2]) || m_isvar($input[$i][2])) && (m_isconst($input[$i][3]) || m_isvar($input[$i][3])))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
        }
    
    //2 operandy var symb
    elseif($instr == "MOVE" || $instr == "NOT" || $instr == "INT2CHAR" || $instr == "STRLEN" || $instr == "TYPE"){
        if(count($input[$i]) == 3 && m_isvar($input[$i][1]) && (m_isconst($input[$i][2]) || m_isvar($input[$i][2])))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }
    //2 operandy var type
    elseif($instr == "READ"){
        if(count($input[$i]) == 3 && m_isvar($input[$i][1]) && m_istype($input[$i][2]))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }

    //1 operand var
    elseif($instr == "DEFVAR" || $instr == "POPS")
    {
        if(count($input[$i]) == 2 && m_isvar($input[$i][1]))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }
    //1 operand label
    elseif($instr == "CALL" || $instr == "LABEL" || $instr == "JUMP")
    {
        if(count($input[$i]) == 2 && m_islabel($input[$i][1]))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }
    //1 operand symb
    elseif($instr == "PUSHS" || $instr == "EXIT" || $instr == "DPRINT" || $instr == "WRITE")
    {
        if(count($input[$i]) == 2 && (m_isconst($input[$i][1]) || m_isvar($input[$i][1])))
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }

    //0 operandu
    elseif($instr == "CREATEFRAME" || $instr == "PUSHFRAME" || $instr == "POPFRAME" || $instr == "RETURN" || $instr == "BREAK")
    {
        if(count($input[$i]) == 1)
        {
            m_genxml($input[$i], $outputXML, $key);
            $key++;
        }
        else {echo "Chyba 23\n"; exit(23);}
    }

    //kontrola duplicity hlavicky
    elseif($header == 0 && $instr == ".IPPCODE20")
    {
        $header++;
    }
    else{
        echo "Chyba 22\n";
        exit(22);
    }
}

xmlwriter_end_element($outputXML);
xmlwriter_end_document($outputXML);
echo xmlwriter_output_memory($outputXML);
?>