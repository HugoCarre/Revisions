<?php

include "inc/bdd.php";

    $sql = "SELECT * FROM rubika_shop";
    $req = $db->prepare($sql);
    $req->execute();
    $data = $req->fetchAll();

    if($req->rowCount() > 0){
        $ue4_Answer = array(
            "IsConnected" => true,
            "ShopData" => array( )
        );
        foreach ($data as $line){ 
            $temp = array();
            array_push($temp,$line["Id"]);
            array_push($temp,$line["Item"]);
            array_push($temp,$line["Description"]);
            array_push($temp,$line["Price"]);
            array_push($temp,$line["Available"]);
            array_push($ue4_Answer["ShopData"],$temp);
        }        
    }else{
        $ue4_Answer = array(
            "IsConnected" => false,
            "Error" => "Problème de connexion"
            );
    }

    header('Content-Type:application/json; charset=utf-8');
    print json_encode($ue4_Answer,JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE);