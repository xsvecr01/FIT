<!--
autor: Radek Švec (xsvecr01)

-->

<?php
include 'dbh.php';

$nazev = $_POST['nazev'];
$uzivatel = $_POST['uzivatel'];
$umisteni = $_POST['umisteni'];
$typ = $_POST['typ'];
$popis = $_POST['popis'];
$pocet = $_POST['pocet'];
$datum = date("Y/m/d");

for($j = 0; $j < $pocet; $j++){
        $sql = "INSERT INTO technika VALUES(null, '$nazev', '$uzivatel', '$umisteni', '$typ', '$popis', '$datum')";
    if(mysqli_query($conn, $sql)){
    }
    else{
        echo mysqli_error($conn);
    }
}

?>