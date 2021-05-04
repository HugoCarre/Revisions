<?php include "inc/header.php" ?>

<h1>Debug</h1>

<form action="ue4_Register.php" method="POST">
    <div class="form-group" style="margin-top: 40px;">
        <label for="">Username*</label>
        <input type="text" name="Username" class="form-control" required> 
        <label for="">Password*</label>
        <input type="text" name="Password" class="form-control" required> 
        <label for="">Password Verif*</label>
        <input type="text" name="PasswordVerif" class="form-control" required> 
    </div>
    <button type="submit" class="btn btn-primary">ue4_Register.php</button>
</form>

<form action="ue4_Login.php" method="POST">
    <div class="form-group" style="margin-top: 40px;">
        <label for="">Username*</label>
        <input type="text" name="Username" class="form-control" required> 
        <label for="">Password*</label>
        <input type="text" name="Password" class="form-control" required> 
    </div>
    <button type="submit" class="btn btn-primary">ue4_Login.php</button>
</form>

<form action="ue4_PlayerData.php" method="POST">
    <div class="form-group" style="margin-top: 40px;">
        <label for="">Token*</label>
        <input type="text" name="Token" class="form-control" required> 
    </div>
    <button type="submit" class="btn btn-primary">ue4_PlayerData.php</button>
</form>

<?php include "inc/footer.php" ?>