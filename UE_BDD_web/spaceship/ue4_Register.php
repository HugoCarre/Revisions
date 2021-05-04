<?php

$messageAlert='';

if(!empty($_POST) && !empty($_POST['Username']) && !empty($_POST['Password'] && !empty($_POST['PasswordVerif'])))
{
    include 'inc/bdd.php';

    $requestError=array();

    // Check password twice good
    if($_POST['Password'] != $_POST['PasswordVerif']){
        array_push($requestError,'Erreur password');
    };

    $Sql = 'SELECT * FROM rubika_users WHERE Username = :Username';
    $req = $db->prepare($Sql);
    $req->execute(array(
        'Username' => $_POST['Username']
    ));

    $data = $req->fetch();
    if($req->rowCount() > 0){
        array_push($requestError,'Ce Username existe déjà !');
    };

    if(count($requestError) != 0){
        foreach($requestError as $error){
            $messageAlert = $messageAlert.'<br/>'.$error;
        }
    }else{
        $now=date_format(new DateTime('now', new DateTimeZone('Europe/Paris')),'Y-m-d H:i::s');
        
        print $now;
        $Sql = 'INSERT INTO rubika_users(Username, Password, LastConnectionDate) VALUES (:Username, :Password, :LastConnectionDate)';
        $req = $db->prepare($Sql);
        $req->execute(array(
            'Username' => $_POST['Username'],
            'Password' => password_hash($_POST['Password'],PASSWORD_BCRYPT),
            'LastConnectionDate' => $now
        ));
        $messageAlert = 'Joueur enregistré !';
    };
}

if($messageAlert != '')
{
    print $messageAlert;
};
