<?php

$pocet = 15;

for($j = 0; $j < $pocet; $j++){
            $chars = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
            $randID = '';
            for ($i = 0; $i < 2; $i++) {
                $randID .= $chars[rand(0, 61)];
            }
            echo $randID.$pocet. "<br>";
        }


?>