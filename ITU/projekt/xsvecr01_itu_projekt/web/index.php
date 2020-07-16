<!--
autor: Radek Švec (xsvecr01)

-->

<?php
    include 'dbh.php';
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">

    <title>EVT - Svec</title>
    <meta name="Radek Svec" content="evidence vypocetni techniky">

    <link rel="icon" type="image/png" sizes="16x16" href="favicon-16x16.png">

    <link rel="stylesheet" href="css/styles.css?v=1.0">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css" integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">
    <style>
        .clickable-row{
            cursor:pointer;
        }
        .custombtn{
            width:80px !important;
        }
        td{
            padding: .1rem .2rem .1rem .2rem !important;
        }
        th{
            text-align:center;
            padding: .4rem !important;
        }
        .hoverblue{
            background-color: #71CDF4;
        }
        .hoverblue:hover {
            background-color: #00A8E3;
        }
        .footer {
            position: relative;
            margin-top: -2rem;
            height: 2rem;
            clear:both;
            background-color: #EAEAEA;
            text-align: center;
        }
        html, body{
            height:100%;
        }
        
        #wrap {
            min-height: 100%;
        }

        #main {
            padding-bottom:2rem;
        }
    </style>

<!--======================================================================-->
<!--==================================JQUERY==============================-->
<!--======================================================================-->

    <script  src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js" integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js" integrity="sha384-wfSDF2E50Y2D1uUdj0O3uMBJnjuUD4Ih7YwaYd1iqfktj0Uod8GCExl3Og8ifwB6" crossorigin="anonymous"></script>
    <script>
    $(document).ready(function(){
        load_data();

    //-----------------------------------------nacteni polozek----------------
        function load_data(sql){
            $.ajax({
                url: "load.php",
                method: "POST",
                data: {sql:sql},
                success: function(data){
                    $('#result').html(data);
                }
            })
        }

    //-----------------------------------------vyhledani polozek----------------
        $('#search_text').keyup(function(){
            var search = $(this).val();
            if(search != ''){
                load_data(search);
            }
            else{
                load_data();
            }
        })

    //-----------------------------------------vkladani polozek----------------
        $('#insert').click(function(event){
            event.preventDefault();
            $.ajax({
                url: "insert.php",
                method: "POST",
                data: $('form').serialize(),
                dataType: "text",
                success:function(){
                    load_data();
                }
            })
        })

    //-----------------------------------------mazani polozek----------------
        //------funkce pro skryvani sloupce------
        $('#delBtn').click(function(){
            $col = $('td:nth-of-type(8)');
            $col1 = $('th:nth-of-type(8)');
            var btn = document.getElementById('delBtn');

            if ($col.css('display') == 'none'){
                $col.css('display', 'block');
                $col1.css('display', 'block');
                btn.innerText="Zrušit";
                btn.className='btn-md btn-secondary custombtn mx-2';
            }

            else if($col.css('display') == 'block'){
                $col.css('display', 'none');
                $col1.css('display', 'none');
                btn.innerText="Odebrat";
                btn.className='btn-md btn-danger custombtn mx-2';
            }
            document.getElementById("doDelete").style.display = $col.css('display');
        })
        //----vymazani-----
        $('#doDelete').click(function(){
            if(confirm("Chcete smazat vybrané položky?")){
                var id = [];

                $(':checkbox:checked').each(function(i){
                    id[i] = $(this).val();
                })

                if(id.length === 0){
                    alert("Musíte vybrat alespoň něco.");
                }
                else{
                    $.ajax({
                        url: "delete.php",
                        method: "POST",
                        data: {id:id},
                        success:function(){
                            load_data();
                            document.getElementById("doDelete").style.display="none";
                            document.getElementById("delBtn").innerText="Odebrat";
                            document.getElementById("delBtn").className='btn-md btn-danger custombtn mx-2';
                        }
                    })
                }
            }
            else{
                return false;
            }
        })

        var status = true;
        $(document).on("click","#vypis-tab #selectall",function(){
            if(status === true){
                $(':checkbox').each(function(){
                this.checked = true;
                status = false;
            })
            }
            else{
                $(':checkbox').each(function(){
                this.checked = false;
                status = true;
            })
            }
        })


        //-----------------------------------------editace----------------
        //-----nacteni hodnot do okna----
        $(document).on("click", "#vypis-tab .clickable-row", function(){
            var id = $(this).data('id');
            var nazev = $('#'+id).children('td[data-target=nazev]').text();
            var umisteni = $('#'+id).children('td[data-target=umisteni]').text();
            var uzivatel = $('#'+id).children('td[data-target=uzivatel]').text();
            var popis = $('#'+id).children('td[data-target=popis]').text();
            var typ = $('#'+id).children('td[data-target=typ]').text();


            $('#nazevMod').val(nazev);
            $('#umisteniMod').val(umisteni);
            $('#uzivatelMod').val(uzivatel);
            $('#popisMod').val(popis);
            $('#idMod').val(id);
            $('#idMod').text(id);
            $('#myModal').modal('toggle');
            $('#typMod').text(typ);

        })

        //-------------------aktualizace hodnot------------
        $('#save').click(function(){
            var id = $('#idMod').val();
            var nazev =  $('#nazevMod').val();
            var umisteni = $('#umisteniMod').val();
            var uzivatel = $('#uzivatelMod').val();
            var popis = $('#popisMod').val();

            $.ajax({
                url: "edit.php",
                method: "POST",
                data: {nazev:nazev, umisteni:umisteni, uzivatel:uzivatel, popis:popis, id:id},
                success: function(response){
                    load_data();
                    $('#myModal').modal('toggle');
                }
            })
        })


    })
    </script>
</head>

<!--=======================================================================-->
<!--=========================================HTML==========================-->
<!--=======================================================================-->

<body>
<nav class="navbar navbar-light" style="background-color:lightgrey">
    <a class="navbar-brand">Evidence Výpočetní Techniky</a>
    <form class="form-inline my-2 col-md-5 col-sm-12">
      <input class="form-control w-100 padding-left: 0.2rem" type="search" id="search_text" name="search_text" placeholder="Vyledávání...">
    </form>
</nav>
<div id="wrap">
<div class="container" id="main">
    <div class="my-2 row col-md-12">
        <div>
            <button id="addBtn" onclick="handleAddBtn()" class="btn-md btn-success custombtn mx-2">Přidat</button>
        </div>
        <div>
            <button id="delBtn" class="btn-md btn-danger custombtn mx-2">Odebrat</button>                
        </div>
        <div>
            <button name="doDelete" id="doDelete" style="display:none" class="btn-md mx-2 btn-danger">Odebrat vybrané</button>
        </div>
    </div>
    <!-----------------------------------------Form pro pridani---------------->
    <div style="display:none" id="myForm">
        <form id="addItem" method="post">
            <div class="form-group">
            <table class="mb-2 w-25">
                <tr>
                    <td>
                    Název:
                    </td>
                    <td>
                    <input class="form-control form-control-sm" type="text" placeholder="Název" id="nazev" name="nazev">
                    </td>
                </tr>
                <tr>
                    <td>
                    Typ:
                    </td>
                    <td>
                    <select class="form-control form-control-sm" name="typ" id="typ">
                        <option value="Počítač">Počítač</option>
                        <option value="Notebook">Notebook</option>
                        <option value="Monitor">Monitor</option>
                        <option value="Tiskárna">Tiskárna</option>
                        <option value="Tablet">Tablet</option>
                        <option value="Telefon">Telefon</option>
                        <option value="Jiné">Jiné</option>
                    </select>
                    </td>
                </tr>
                <tr>
                    <td>
                    Umístění:
                    </td>
                    <td>
                    <input class="form-control form-control-sm" type="text" placeholder="Umístění" id="umisteni" name="umisteni">
                    </td>
                </tr>
                <tr>
                    <td>
                    Uživatel:
                    </td>
                    <td>
                    <input class="form-control form-control-sm" type="text" placeholder="Uživatel" id="uzivatel" name="uzivatel">
                    </td>
                </tr>
                <tr>
                    <td style="vertical-align:top; text-align:left">
                    Popis:
                    </td>
                    <td>
                    <textarea class="form-control form-control-sm" type="text" placeholder="Popis zařízení" id="popis" name="popis"></textarea>
                    </td>
                </tr>
                <tr>
                    <td>
                    Počet:
                    </td>
                    <td>
                    <input class="form-control form-control-sm" type="number" value="1" id="pocet" name="pocet">
                    </td>
                </tr>
            </table>
            <button class="btn-md btn-info mx-2" type="button" onclick="clearForm('addItem')">Vymazat</button>
            <input class="btn-md btn-success mx-2" type="submit" value="Přidat techniku" name="insert" id="insert">
            </div>
        </form>
    </div>


    <!----------------------------------------Vypis---------------->
    <div id="result"></div>



</div>
</div>
<footer class="footer">
    &copy;Radek Švec (xsvecr01) - ITU 2019/2020
</footer>

<!--------------------------------Modalni okno-------------------->
<div id="myModal" class="modal" role="dialog">
  <div class="modal-dialog modal-sm">
    <div class="modal-content">
      <div class="modal-header">
        <h4 class="modal-title">Úprava informací</h4>
        <button type="button" class="close" data-dismiss="modal">&times;</button>
      </div>
      <div class="modal-body">
            <table class="w-100">
            <tr>
                <td>
                ID:
                </td>
                <td>
                <label id="idMod"></label>
                </td>
            </tr>
            <tr>
                <td>
                Název:
                </td>
                <td>
                <input class="form-control form-control-sm" type="text" id="nazevMod">
                </td>
            </tr>
            <tr>
                <td>
                Typ:
                </td>
                <td>
                <label id="typMod"></label>
                </td>
            </tr>
            <tr>
                <td>
                Umístění:
                </td>
                <td>
                <input class="form-control form-control-sm" type="text" id="umisteniMod">
                </td>
            </tr>
            <tr>
                <td>
                Uživatel:
                </td>
                <td>
                <input class="form-control form-control-sm" type="text" id="uzivatelMod">
                </td>
            </tr>
            <tr>
                <td style="vertical-align:top; text-align:left">
                Popis:
                </td>
                <td>
                <textarea class="form-control form-control-sm" type="text" id="popisMod"></textarea>
                </td>
            </tr>
            </table>

      </div>
      <div class="modal-footer">
        <button type="button" class="btn-md btn-secondary pull-left" data-dismiss="modal">Zrušit</button>
        <button type="button" id="save" class="btn-md btn-primary pull-right">Uložit změny</button>
      </div>
    </div>

  </div>
</div>


<!--======================================================================-->
<!--==================================JAVASCRIPT==========================-->
<!--======================================================================-->

<script>
function handleAddBtn(){
    btn = document.getElementById("addBtn");
    formDiv = document.getElementById("myForm");

    if(btn.innerText === "Přidat"){
        formDiv.style.display = "block";
        btn.innerText = "Zavřít";
        btn.className = "btn-md btn-secondary custombtn mx-2"
    }
    else{
        clearForm("addItem")
        formDiv.style.display = "none";
        btn.innerText = "Přidat";
        btn.className = "btn-md btn-success custombtn mx-2"
    }
}

function clearForm(formId){
    document.getElementById(formId).reset();
}
</script>
</body>
</html>