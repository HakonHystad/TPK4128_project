
<?php


include_once("keywords.php");

	if( $_SERVER[ "REQUEST_METHOD" ] == "POST" )// only respond to http POST request
	{	

		echo "POST received!\n";

		/*-------------------------------------- receive data  ---------------------------------------*/
		
   		$_POST = filter_input_array( INPUT_POST, $_POST );// sanatize input

		$block = (int)$_POST["block"];
		$byte1 = $_POST["byte1"];
		$byte2 = $_POST["byte2"];
		$byte3 = $_POST["byte3"];
		$byte4 = $_POST["byte4"];
		$byte5 = $_POST["byte5"];
		$byte6 = $_POST["byte6"];
		$byte7 = $_POST["byte7"];
		$byte8 = $_POST["byte8"];
		$byte9 = $_POST["byte9"];
		$byte10 = $_POST["byte10"];
		$byte11 = $_POST["byte11"];
		$byte12 = $_POST["byte12"];
		$byte13 = $_POST["byte13"];
		$byte14 = $_POST["byte14"];
		$byte15 = $_POST["byte15"];
		$byte16 = $_POST["byte16"];
		$ascii = $_POST["ascii"];
		$token = $_POST["pass"];// yay plaintext passwords..

		echo $block;


		/*-------------------------------------- connect to database  ---------------------------------------*/
		
		// connection parameters for database

		$server = "mysql.stud.ntnu.no";
		$user = "haakonhy_tpk4128";
		$db = "haakonhy_raspberryProject";

		// naive way of restricting data manipulation access


		if( $token == $GLOBALS['POSTpass'] )
		{
		    $psswd = $GLOBALS['pass'];
		}
		else
		{
		    $psswd = "wrong";
		}


		// make connection
		$connection = mysqli_connect($server, $user, $psswd, $db );

		if(!$connection)
		{
		    die("Connection failed: " . mysqli_connect_error() );
		}


		/*-------------------------------------- store data in database  ---------------------------------------*/
		
		// table is indexed by unique key blk
		$sql = "REPLACE INTO rfid_data SET blk='$block', b1='$byte1', b2='$byte2', b3='$byte3', b4='$byte4', b5='$byte5', b6='$byte6', b7='$byte7', b8='$byte8', b9='$byte9', b10='$byte10', b11='$byte11', b12='$byte12', b13='$byte13', b14='$byte14', b15='$byte15', b16='$byte16', chars='$ascii'";

		$result = mysqli_query($connection, $sql);

		var_dump($result);

		/*-------------------------------------- close connection to database  ---------------------------------------*/

		mysqli_close($connection);


	}// if POST


?>
