<!--
autor: Radek Švec (xsvecr01)

-->

<?php
include 'dbh.php';

$output = '';
$output0 = '';

$output0 .= '
        <table id="vypis-tab" class="table table-bordered table-striped table-hover">
            <tr>
                <th style="min-width:4rem">ID</th>
                <th style="min-width:10rem">Název</th>
                <th style="min-width:6rem">Typ</th>
                <th style="min-width:7rem">Umístění</th>
                <th style="min-width:11rem">Uživatel</th>
                <th style="word-wrap:break-word; width:90%; white-space:normal">Popis</th>
                <th style="min-width:8rem">Datum přidání</th>
                <th id="selectall" name="selectall" class="border-left-0 border-top-0 hoverblue" style="cursor:pointer; min-width:7rem; display:none">Vybrat vše</th>
            </tr>
        ';

if(isset($_POST["sql"])){
    $search = mysqli_real_escape_string($conn, $_POST["sql"]);
    $sql = "
    SELECT * FROM technika WHERE
    id LIKE '%".$search."%'
    OR nazev LIKE '%".$search."%'
    OR typ LIKE '%".$search."%'
    OR umisteni LIKE '%".$search."%'
    OR uzivatel LIKE '%".$search."%'
    OR popis LIKE '%".$search."%'
    OR datum LIKE '%".$search."%'
    ORDER BY id DESC
    ";
}
else{
    $sql = "SELECT * FROM technika ORDER BY id DESC";
}

if($result = mysqli_query($conn, $sql)){
    echo $output0;
    if(mysqli_num_rows($result) > 0)
    {
        while ($row = mysqli_fetch_array($result))
        {
        $output.='
        <tr id="'.$row["id"].'">
            <td class="text-center">'.$row["id"].'</td>
            <td data-target="nazev" class="clickable-row" data-id="'.$row["id"].'">'.$row["nazev"].'</td>
            <td data-target="typ" class="text-center">'.$row["typ"].'</td>
            <td data-target="umisteni" class="text-center">'.$row["umisteni"].'</td>
            <td data-target="uzivatel" >'.$row["uzivatel"].'</td>
            <td data-target="popis">'.$row["popis"].'</td>
            <td class="text-center">'.$row["datum"].'</td>
            <td style="display:none" class="border-bottom border-top-0 border-left-0 border-right-0 text-center"><input type="checkbox" name="deleteId[]" value="'.$row["id"].'"></td>
        </tr>
        ';
    }
    echo $output;
    }
    else
    {
        echo '<tr><td colspan = "8">Nic tady není &#9785;</td><tr>';
    }
}
else {
    echo mysqli_error($conn);
}
?>