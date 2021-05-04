<?php
include "inc/functions.php";

if( !empty($_POST["Token"]) || !empty($_POST) && !empty($_POST['Username']) && !empty($_POST['Password']) )
{
    $transactionError=array();
    $connexion = false;
    $check_MethodByToken = true;

    if( !empty($_POST["Token"]) ){
        $checkToken = CheckToken($_POST["Token"]);
        if($checkToken){
            $connexion = true;
        }else{
            array_push($transactionError,0);
        }
    }
    elseif( !empty($_POST['Username']) && !empty($_POST['Password']) ){
        $check_MethodByToken = false;
        include "inc/bdd.php";

        $sql = "SELECT * FROM rubika_users WHERE Username = :Username";
        $req = $db->prepare($sql);
        $req->execute(array(
            "Username" => $_POST['Username']
        ));
        $data = $req->fetch();

        if($req->rowCount() == 1){
            if( password_verify($_POST["Password"], $data["Password"]) == true ){
                $connexion = true;
            }else{
                array_push($transactionError,1);
            }
        }else{
            array_push($transactionError,1);
        }
    }   

    if($connexion){
        if($check_MethodByToken){    
            $ue4_Answer=array(
                "IsConnected" => true,
                "NewToken" => CreateNewToken_WithToken($_POST["Token"]),        
            );
        }else{
            $ue4_Answer=array(
                "IsConnected" => true,
                "NewToken" => CreateNewToken_WithUsername($_POST["Username"]),        
            );
        }
    }else{
        $ue4_Answer=array(
            "IsConnected" => false,
            "Error" => $transactionError[0],        
        );
    }
    header('Content-Type:application/json; charset=utf-8');
    print json_encode($ue4_Answer,JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);
}



