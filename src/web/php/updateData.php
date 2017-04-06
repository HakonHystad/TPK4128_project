
<?php

include_once("keywords.php");   

/*-------------------------------------- connect to database  ---------------------------------------*/

    //connection data
    $server = "mysql.stud.ntnu.no";
    $user = "haakonhy_tpk4128";
   
    $psswd = $GLOBALS['pass'];// hidden file, to publish on git
    $db = "haakonhy_raspberryProject";


    // make connection
    $connection = mysqli_connect($server, $user, $psswd, $db );

    if(!$connection)
    {
	die("Connection failed: " . mysqli_connect_error() );
    }


/*-------------------------------------- retrieve data   ---------------------------------------*/

   
   //select data
   $sql = "SELECT blk, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, chars FROM rfid_data";
   $result = mysqli_query($connection, $sql);

/*-------------------------------------- format table  ---------------------------------------*/

   if( mysqli_num_rows($result) > 0 )
   {
       echo "<table><tr><th>Block</th><th>0</th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th><th>7</th><th>8</th><th>9</th><th>10</th><th>11</th><th>12</th><th>13</th><th>14</th><th>15</th><th>Ascii</th></tr>";
  
       while( $row = mysqli_fetch_assoc($result) )
       {
	   echo "<tr><td>" . $row["blk"] . "</td><td>" . $row["b1"] . "</td><td>" . $row["b2"] . "</td><td>" . $row["b3"] . "</td><td>" . $row["b4"] . "</td><td>" 
	   		   . $row["b5"] . "</td><td>" . $row["b6"] . "</td><td>" . $row["b7"] . "</td><td>" . $row["b8"] . "</td><td>" . $row["b9"] . "</td><td>" 
			   . $row["b10"] . "</td><td>" . $row["b11"] . "</td><td>" . $row["b12"] . "</td><td>" . $row["b13"] . "</td><td>" . $row["b14"] . "</td><td>" 
			   . $row["b15"] . "</td><td>" . $row["b16"] . "</td><td>" . $row["chars"] . "</td></tr>";
       }
  
       echo "</table>";
   }

/*-------------------------------------- close connection  ---------------------------------------*/

    mysqli_close($connection);

?>
