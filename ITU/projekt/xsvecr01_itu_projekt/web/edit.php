<!--
autor: Radek Švec (xsvecr01)

-->

<?php
    include 'dbh.php';

    if(isset($_POST['id'])){

        $nazev = $_POST['nazev'];
        $uzivatel = $_POST['uzivatel'];
        $umisteni = $_POST['umisteni'];
        $popis = $_POST['popis'];
        $id = $_POST['id'];

        $sql = "UPDATE technika SET nazev='$nazev', uzivatel='$uzivatel', umisteni='$umisteni', popis='$popis' WHERE id='$id'";

        if(mysqli_query($conn, $sql)){
            return 'data updated';
        }
        else{
            echo mysqli_error($conn);
        }
    }
?>