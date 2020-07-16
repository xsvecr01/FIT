<!--
autor: Radek Švec (xsvecr01)

-->

<?php
include 'dbh.php';

if(isset($_POST["id"])){
    foreach($_POST["id"] as $id){
        $sql = "DELETE FROM technika WHERE id = '".$id."'";
        if(mysqli_query($conn, $sql)){
        }
        else{
            echo mysqli_error($conn);
        }
    }
}
?>